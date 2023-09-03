#ifndef OFDM
#define OFDM

#include <fftw3.h>

#include <map>
#include <string>
#include <random>
#include <unordered_map>

#include "constellation.h"

class Ofdm {
 public:
  // Constructor
  Ofdm(const std::map<std::string, int>& config);

  // Destructor
  ~Ofdm();

  // random number generator
  std::vector<int> generateRandomInt(const std::string& constl_type);

  std::vector<std::vector<std::complex<double>>> generateModulatedSignal(
      const std::vector<int>& integers, const std::string& constl_type);

  // Helper functions

  template <typename T>
  inline std::vector<T> columnMajorFlatten(
      const std::vector<std::vector<T>>& input) {
    std::vector<T> result;
    size_t numRows = input.size();
    size_t numCols = (numRows > 0) ? input[0].size() : 0;

    if (numRows == 0 || numCols == 0) {
      // Handle empty input case
      return result;
    }

    result.reserve(numRows * numCols);

    for (size_t col = 0; col < numCols; ++col) {
      for (size_t row = 0; row < numRows; ++row) {
        result.push_back(input[row][col]);
      }
    }

    return result;
  }
  
  template <typename T>
  inline std::vector<T> rowMajorFlatten(
      const std::vector<std::vector<T>>& input) {
    std::vector<T> flattened_vector;
    int rows = input.size();
    int cols = input[0].size();

    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        flattened_vector.push_back(input[i][j]);
      }
    }

    return flattened_vector;
  }

  std::vector<int> convertIntToBits(const std::vector<int>& integers,
                                    const std::string& constl_type);

  std::vector<std::vector<double>> transpose2DVector(
      const std::vector<std::vector<double>>& input);

  std::vector<std::vector<std::complex<double>>> transpose2DComplexVector(
      const std::vector<std::vector<std::complex<double>>>& input);

  std::vector<std::vector<std::complex<double>>> reshape(
      const std::vector<std::complex<double>>& input, int num_rows,
      int num_cols);

  std::vector<std::vector<std::complex<double>>> columnMajorReshape(
      const std::vector<std::complex<double>>& input, int num_rows,
      int num_cols);

  int symbolErrorCount(const std::vector<int>& vector1,
                       const std::vector<int>& vector2);

  std::complex<double> calculateMean(const std::vector<std::complex<double>>& input);
  std::complex<double> calculateStandardDeviation(const std::vector<std::complex<double>>& input);

  // FFT and IFFT
  std::vector<std::vector<std::complex<double>>> fft(
      const std::vector<std::vector<std::complex<double>>>& input);

  std::vector<std::complex<double>> fft(
      const std::vector<std::complex<double>>& input, int n);

  std::vector<std::vector<std::complex<double>>> ifft(
      const std::vector<std::vector<std::complex<double>>>& input);

  // Cyclic Prefix
  std::vector<std::vector<std::complex<double>>> addCyclicPrefix(
      std::vector<std::vector<std::complex<double>>> vec2D);

  std::vector<std::vector<std::complex<double>>> removeCyclicPrefix(
      std::vector<std::complex<double>> input);

  // AWGN Noise
  std::vector<std::complex<double>> addAWGN(
      const std::vector<std::complex<double>>& signal);
    
  std::vector<std::complex<double>> generateNoise(
      const double& mean, const double& stddev, size_t size);

  // Filter
  std::vector<std::complex<double>> filter(
      const std::vector<std::complex<double>>& signal,
      const std::vector<std::complex<double>>& filter_coeffs);

  // Decoding
  std::vector<std::vector<int>> decode(
      std::vector<std::vector<std::complex<double>>>& received,
      std::vector<std::complex<double>> channel_response,
      const std::string& constl_type);

 private:
  Constellation* constl_;
  std::unordered_map<std::string, std::vector<std::complex<double>>>
      constellations_;  // modulation schemes
  int B_;               // number of OFDM symbols per transmitted frame
  int L_;               // number of subcarriers in each OFDM symbol
  int CP_length_;       // cyclic prefix length
  int Nh_;              // channel order
  std::mt19937 generator;  // random number generator
  std::vector<int> convertBits(int value, int num_bits);
  double squareEuclideanDistance(const std::complex<double>& a,
                                 const std::complex<double>& b);
  std::complex<double> complexDivision(const std::complex<double>& a,
                                       const std::complex<double>& b);
  std::vector<int> findMinInd(const std::vector<std::vector<double>>& matrix);


  // You can also add private member functions here
};

#endif
