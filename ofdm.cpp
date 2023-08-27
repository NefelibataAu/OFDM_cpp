#include "ofdm.h"

#include <cassert>
#include <cmath>
#include <complex>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>

Ofdm::Ofdm(const std::map<std::string, int>& config) {
  auto it = config.find("B");
  B_ = (it != config.end()) ? it->second : 10;

  it = config.find("L");
  L_ = (it != config.end()) ? it->second : 16;

  it = config.find("CP_length");
  CP_length_ = (it != config.end()) ? it->second : 4;

  it = config.find("Nh");
  Nh_ = (it != config.end()) ? it->second : 4;

  // constellation
  constl_ = new Constellation();
  constellations_["bpsk"] = constl_->bpsk();
  constellations_["qpsk"] = constl_->qpsk();
  constellations_["qam16"] = constl_->qam(16);
}

Ofdm::~Ofdm() {
  delete constl_;
  constl_ = nullptr;
}

std::vector<int> Ofdm::generateRandomInt(const std::string& constl_type) {
  // TODO: generate bits such that all modulation compare the number of
  // iterations
  //       when they make the same number of errors

  std::vector<int> randomIntegers;

  randomIntegers.reserve(B_ * L_);
  for (int i = 0; i < B_ * L_; ++i) {
    randomIntegers.push_back(std::rand() % constellations_[constl_type].size());
  }

  return randomIntegers;
}

std::vector<std::vector<std::complex<double>>> Ofdm::generateModulatedSignal(
    const std::vector<int>& integers, const std::string& constl_type) {
  std::vector<std::vector<std::complex<double>>> modulated_signal;
  std::vector<std::complex<double>> constellation =
      constellations_[constl_type];

  int total_elements = B_ * L_;

  if (integers.size() != total_elements) {
    std::cout << "Cannot generate modulated signal. Input vector size does not "
                 "match target size.\n";
    return {};
  }

  modulated_signal.resize(B_, std::vector<std::complex<double>>(L_));

  for (int i = 0; i < B_; ++i) {
    for (int j = 0; j < L_; ++j) {
      modulated_signal[i][j] = constellation[integers[i * L_ + j]];
    }
  }

  return modulated_signal;
}

std::vector<std::complex<double>> Ofdm::flattenVector(
    const std::vector<std::vector<std::complex<double>>>& input) {
  std::vector<std::complex<double>> flattened_vector;
  int rows = input.size();
  int cols = input[0].size();

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      flattened_vector.push_back(input[i][j]);
    }
  }

  return flattened_vector;
}

std::vector<int> Ofdm::convertBits(int value, int num_bits) {
  std::vector<int> bits;
  for (int i = num_bits - 1; i >= 0; --i) {
    int bit = (value >> i) & 1;
    bits.push_back(bit);
  }
  return bits;
}

std::vector<int> Ofdm::convertIntToBits(const std::vector<int>& integers,
                                        const std::string& constl_type) {
  std::vector<int> bits;

  int num_bits_per_symbol = 0;
  if (constl_type == "bpsk") {
    num_bits_per_symbol = 1;
  } else if (constl_type == "qpsk") {
    num_bits_per_symbol = 2;
  } else if (constl_type == "qam16") {
    num_bits_per_symbol = 4;
  }

  for (int value : integers) {
    std::vector<int> value_bits = convertBits(value, num_bits_per_symbol);
    bits.insert(bits.end(), value_bits.begin(), value_bits.end());
  }

  return bits;
}

std::vector<std::vector<std::complex<double>>> Ofdm::transpose2DComplexVector(
    const std::vector<std::vector<std::complex<double>>>& input) {
  int rows = input.size();
  int cols = input[0].size();

  std::vector<std::vector<std::complex<double>>> transposed(
      cols, std::vector<std::complex<double>>(rows));

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      transposed[j][i] = input[i][j];
    }
  }

  return transposed;
}

std::vector<std::vector<std::complex<double>>> Ofdm::ifft(
    const std::vector<std::vector<std::complex<double>>>& input) {
  /*
    normalization factor sqrt(L) is incorporated in ifft
    " * sqrt(static_cast<double>(L_)); "
  */
  int num_rows = input.size();
  int num_cols = input[0].size();

  fftw_plan plan = fftw_plan_dft_1d(num_cols, nullptr, nullptr, FFTW_BACKWARD,
                                    FFTW_ESTIMATE);

  std::vector<std::vector<std::complex<double>>> output(
      num_rows, std::vector<std::complex<double>>(num_cols));

  for (int i = 0; i < num_rows; ++i) {
    std::vector<std::complex<double>> row_input(num_cols);
    std::vector<std::complex<double>> row_output(num_cols);

    for (int j = 0; j < num_cols; ++j) {
      row_input[j] = input[i][j];
    }

    fftw_complex* in = reinterpret_cast<fftw_complex*>(&row_input[0]);
    fftw_complex* out = reinterpret_cast<fftw_complex*>(&row_output[0]);

    fftw_execute_dft(plan, in, out);

    for (int j = 0; j < num_cols; ++j) {
      output[i][j] = row_output[j] / static_cast<double>(num_cols) *
                     sqrt(static_cast<double>(L_));
    }
  }

  fftw_destroy_plan(plan);

  return output;
}

std::vector<std::vector<std::complex<double>>> Ofdm::addCyclicPrefix(
    std::vector<std::vector<std::complex<double>>> vec2D) {
  int numRows = vec2D.size();
  int numCols = vec2D[0].size();

  if (CP_length_ >= numCols) {
    std::cerr << "Number of columns to copy is greater than or equal to the "
                 "total number of columns."
              << std::endl;
    return {};
  }

  for (int i = 0; i < numRows; ++i) {
    std::vector<std::complex<double>> newCols(vec2D[i].end() - CP_length_,
                                              vec2D[i].end());
    vec2D[i].insert(vec2D[i].begin(), newCols.begin(), newCols.end());
  }
  return vec2D;
}

std::vector<std::complex<double>> Ofdm::addAWGN(
    const std::vector<std::complex<double>>& signal) {
  std::default_random_engine generator;
  std::normal_distribution<double> distribution(0.0, std::sqrt(0.5 / L_));

  std::vector<std::complex<double>> noisy_signal;
  noisy_signal.reserve(signal.size());

  for (const std::complex<double>& value : signal) {
    double noise_real = distribution(generator);
    double noise_imag = distribution(generator);

    std::complex<double> noisy_value(value.real() + noise_real,
                                     value.imag() + noise_imag);
    noisy_signal.push_back(noisy_value);
  }

  return noisy_signal;
}

std::vector<std::complex<double>> Ofdm::filter(
    const std::vector<std::complex<double>>& signal,
    const std::vector<std::complex<double>>& filter_coeffs) {
  int signal_size = signal.size();
  int filter_size = filter_coeffs.size();

  std::vector<std::complex<double>> result(signal_size + filter_size - 1,
                                           std::complex<double>(0.0, 0.0));

  for (int i = 0; i < signal_size; ++i) {
    for (int j = 0; j < filter_size; ++j) {
      result[i + j] += signal[i] * filter_coeffs[j];
    }
  }

  return result;
}
