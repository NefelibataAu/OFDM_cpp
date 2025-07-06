#include <cassert>
#include <cmath>
#include <complex>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>
#include <eigen/Dense>

#include "constellation.h"
#include "ofdm.h"
#include "ErrorCorrection/hammingCode.h"

// -------------------- CSV 写文件函数 --------------------
void write2DVectorToCSV(const std::vector<std::vector<double>>& data, const std::string& filename) {
    std::ofstream fout(filename);
    if (!fout.is_open()) {
        std::cerr << "Cannot open file " << filename << " for writing." << std::endl;
        return;
    }
    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            fout << row[i];
            if (i != row.size() - 1) fout << ",";
        }
        fout << "\n";
    }
    fout.close();
}

// -------------------- 其他辅助函数 --------------------
void print1DVector(const std::vector<int>& vec) {
    for (const int& num : vec) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

void output1DVector(std::ofstream& outputFile, const std::vector<int>& vec) {
    if (!outputFile.is_open()) {
        std::cerr << "Output file is not open for writing." << std::endl;
        return;
    }
    for (int value : vec) {
        outputFile << value << " ";
    }
    outputFile << std::endl;
}

void printComplexVector2D(const std::vector<std::vector<std::complex<double>>>& vec2D) {
    for (const auto& row : vec2D) {
        for (const std::complex<double>& value : row) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
}

void output1DComplexVector(std::ofstream& outputFile, const std::vector<std::complex<double>>& vec) {
    if (!outputFile.is_open()) {
        std::cerr << "Output file is not open for writing." << std::endl;
        return;
    }
    for (const auto& value : vec) {
        outputFile << "(" << value.real() << ", " << value.imag() << ") ";
    }
    outputFile << std::endl;
}

void output2DComplexVector(std::ofstream& outputFile, const std::vector<std::vector<std::complex<double>>>& vec2D) {
    if (!outputFile.is_open()) {
        std::cerr << "Output file is not open for writing." << std::endl;
        return;
    }
    for (const auto& row : vec2D) {
        for (const std::complex<double>& value : row) {
            outputFile << "(" << value.real() << ", " << value.imag() << ") ";
        }
        outputFile << std::endl;
    }
}

std::vector<std::complex<double>> normalize(const std::vector<std::complex<double>>& input) {
    double sum_of_abs_squares = 0.0;
    for (const std::complex<double>& value : input) {
        sum_of_abs_squares += std::norm(value);
    }
    double normalization_factor = std::sqrt(sum_of_abs_squares);

    std::vector<std::complex<double>> normalized_vector;
    for (const std::complex<double>& value : input) {
        normalized_vector.push_back(value / normalization_factor);
    }
    return normalized_vector;
}

std::vector<std::complex<double>> addComplexVectors(
    const std::vector<std::complex<double>>& vector1,
    const std::vector<std::complex<double>>& vector2) {
    if (vector1.size() != vector2.size()) {
        throw std::invalid_argument("Vector sizes must match for addition.");
    }
    std::vector<std::complex<double>> result;
    result.reserve(vector1.size());
    for (size_t i = 0; i < vector1.size(); ++i) {
        result.push_back(vector1[i] + vector2[i]);
    }
    return result;
}

std::vector<std::complex<double>> generateRayleighChannel(int numTaps, double standardDeviation) {
    std::vector<std::complex<double>> channel;
    std::mt19937 generator(74);  // 固定种子，方便复现
    std::normal_distribution<double> distribution(0.0, standardDeviation);

    for (int i = 0; i < numTaps; ++i) {
        double realPart = distribution(generator);
        double imagPart = distribution(generator);
        std::complex<double> coefficient(realPart, imagPart);
        channel.push_back(coefficient);
    }
    return channel;
}

// -------------------- 主函数 --------------------
int main() {
    // 输出文件路径
    std::string outputFilePath = "../output/";
    std::ofstream outputFile(outputFilePath + "output.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open the file for writing." << std::endl;
        return 1;
    }

    // OFDM 配置参数
    std::map<std::string, int> config;
    config["B"] = 10;          // OFDM 符号数（每帧）
    config["L"] = 16;          // 子载波数
    config["CP_length"] = 4;   // 循环前缀长度
    config["Nh"] = 4;          // 信道阶数
    config["Pilot"] = 1;       // 导频配置（占位）

    Ofdm ofdm(config);
    HammingCode hmc;

    int mc_N = 5000;           // 蒙特卡洛最大迭代次数

    // SNR 范围 (dB)
    double SNR_dB_start = 0.0;
    double SNR_dB_end = 20.0;
    double SNR_dB_step = 2.0;
    std::vector<double> SNR_dB;
    std::vector<double> SNR;
    for (double i = SNR_dB_start; i <= SNR_dB_end; i += SNR_dB_step) {
        SNR_dB.push_back(i);
        SNR.push_back(pow(10.0, i / 10.0));
    }

    // 保存误码率
    std::vector<std::vector<double>> Pe_symbol;
    std::vector<std::vector<double>> Pe_bit;

    std::vector<std::string> modulation_schemes = {"bpsk", "qpsk", "qam16"};
    std::unordered_map<std::string, int> bitsPerSymbol = {{"bpsk", 1}, {"qpsk", 2}, {"qam16", 4}};

    for (const std::string& mod : modulation_schemes) {
        outputFile << "For modulation: " << mod << ": --------------------------------------" << std::endl;
        std::vector<double> Pe_sym_mod;
        std::vector<double> Pe_bit_mod;
        for (int i = 0; i < (int)SNR.size(); i++) {
            double rho = SNR[i];
            outputFile << "For SNR = " << rho << ": " << std::endl;
            int biterr_cnt = 0;
            int symerr_cnt = 0;
            for (int mc_loop = 0; mc_loop < mc_N; mc_loop++) {
                // 生成随机整数数据
                std::vector<int> data_int = ofdm.generateRandomInt(mod);

                // 整数转换为比特
                std::vector<int> data_bits = ofdm.convertIntToBits(data_int, mod);

                // 编码 (Hamming码)
                Eigen::RowVectorXi eigen_bits = Eigen::Map<const Eigen::VectorXi>(data_bits.data(), (int)data_bits.size());
                Eigen::RowVectorXi coded_bits = hmc.encodeHammingCode(eigen_bits);

                // 调制信号
                std::vector<std::vector<std::complex<double>>> data_sym = ofdm.generateModulatedSignal(data_int, mod);

                // IFFT + 加循环前缀
                auto data_ifft = ofdm.ifft(data_sym);
                auto data_cp = ofdm.addCyclicPrefix(data_ifft);

                // 发射信号展平为一维复数向量
                std::vector<std::complex<double>> data_tx = ofdm.columnMajorFlatten(ofdm.transpose2DComplexVector(data_cp));

                // Rayleigh信道
                std::vector<std::complex<double>> h = generateRayleighChannel(config["Nh"] + 1, std::sqrt(0.5 / (config["Nh"] + 1)));
                std::vector<std::complex<double>> h_normalized = normalize(h);

                // 信号经过信道
                std::vector<std::complex<double>> rec = ofdm.filter(data_tx, h_normalized);
                for (auto& v : rec) v *= std::sqrt(rho);

                // 加噪声
                std::vector<std::complex<double>> noise = ofdm.generateNoise(0.0, std::sqrt(0.5 / config["L"]), rec.size());
                std::vector<std::complex<double>> received = addComplexVectors(rec, noise);

                // 去循环前缀
                auto rec_sans_cp = ofdm.removeCyclicPrefix(received);

                // FFT
                auto rec_f = ofdm.fft(rec_sans_cp);

                // 信道估计 (1D MMSE)
                auto H_hat_MMSE_1D = ofdm.estimateChannel1DMMSE(rec_f[0]);

                // 解码
                auto dec_sym = ofdm.decode(rec_f, H_hat_MMSE_1D, mod, "full");

                std::vector<int> recsym_flatten = ofdm.columnMajorFlatten(dec_sym);

                std::vector<int> rec_bits = ofdm.convertIntToBits(recsym_flatten, mod);

                // 错误统计
                symerr_cnt += ofdm.symbolErrorCount(data_int, recsym_flatten);
                biterr_cnt += ofdm.symbolErrorCount(data_bits, rec_bits);
            }
            Pe_sym_mod.push_back(static_cast<double>(symerr_cnt) / ofdm.totalTxSymbols(mc_N));
            Pe_bit_mod.push_back(static_cast<double>(biterr_cnt) / ofdm.totalTxBits(mc_N, mod));

            outputFile << "Symbol error after " << mc_N << " iterations: " << symerr_cnt << std::endl;
            outputFile << "Bit error after " << mc_N << " iterations: " << biterr_cnt << std::endl;
        }
        Pe_symbol.push_back(Pe_sym_mod);
        Pe_bit.push_back(Pe_bit_mod);
    }

    // 写 CSV 文件替代之前的 MAT 文件保存
    write2DVectorToCSV(Pe_symbol, outputFilePath + "rayleigh_Pesym_MMSE.csv");
    write2DVectorToCSV(Pe_bit, outputFilePath + "rayleigh_Pebit_MMSE.csv");

    outputFile.close();
    return 0;
}
