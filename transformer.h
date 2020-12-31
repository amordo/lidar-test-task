#pragma once

#include "helper.h"
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#define WITHOUT_NUMPY
#include "matplotlibcpp.h"

using namespace std;
namespace plt = matplotlibcpp;

class Transformer {
private:
  helper::FileManager path;

  vector<double> data_parallel;
  vector<double> data_perpendic;
  vector<double> noise_parallel;
  vector<double> noise_perpendic;
  vector<double> result;

  // TODO: merge these in one func
  void SaveChart(const string &path, bool normalized = false);
  void SaveChartResult(const string &path, bool normalized = false);

  void RemoveNoise();
  void RemoveZeroPath();
  void Inverse();
  void Normalize();
  void Filter();
  void GetResult();

public:
  Transformer(const string &data_path, const string &noise_path)
      : path(helper::CreateDir(data_path)) {
    helper::Parse(data_path, data_parallel, data_perpendic);
    helper::Parse(noise_path, noise_parallel, noise_perpendic);

    SaveChart(path.init);

    RemoveNoise();
    RemoveZeroPath();
    Inverse();
    Normalize();
    Filter();
    GetResult();
  }
};