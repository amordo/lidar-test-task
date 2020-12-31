#pragma once

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

namespace helper {

struct FileManager {
  const string dir;
  const string init;
  const string remove_noise;
  const string remove_zero_path;
  const string inverse;
  const string normalize;
  const string filter;
  const string result;
  FileManager(const string &dir)
      : dir(dir), init(dir + "/step0_init.png"s),
        remove_noise(dir + "/step1_remove_noise.png"s),
        remove_zero_path(dir + "/step2_remove_zero_path.png"s),
        inverse(dir + "/step3_inverse.png"s),
        normalize(dir + "/step4_normalize.png"s),
        filter(dir + "/step5_filter.png"s), result(dir + "/step6_result.png"s) {
  }
};

void Parse(string data_path, vector<double> &v1, vector<double> &v2);
string CreateDir(string data_path);

// void remove_noise(vector<double> &res, vector<double> &noise);
// void remove_zero_path(vector<double> &v);
// void inverse(vector<double> &v);
double get_signal_max(vector<double> &v1, vector<double> &v2);
double get_signal_min(vector<double> &v1, vector<double> &v2);
double mediana(vector<double> v);
void filter(vector<double> &orig);
} // namespace helper