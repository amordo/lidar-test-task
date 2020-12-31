#include "transformer.h"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;
using namespace helper;

void Transformer::SaveChart(const string &path, vector<vector<double>> charts,
                            bool normalized) {

  plt::figure();
  const map<string, string> settings = {{"linewidth", "0.5"}};
  vector<double> x(charts.front().size());
  for (int i = 0; i < x.size(); ++i)
    x[i] = i;

  if (normalized) {
    double k_horiz = 12'000 * 1.0 / x.size();
    for (auto &el : x)
      el *= k_horiz;

    for (auto &chart : charts) {
      auto min_el = min(*min_element(chart.begin(), chart.end()), 0.0);
      for (auto &el : chart)
        el -= min_el;
    }
  }

  for (auto &chart : charts) {
    plt::plot(x, chart, settings);
  }

  plt::xlim(0, normalized ? 12'000 : (int)x.size());
  plt::ylim(normalized ? 0 : (int)MinSig(charts), (int)MaxSig(charts));

  // plt::legend();
  plt::save(path);
  plt::close();
}

void Transformer::RemoveNoise() {
  for (size_t i = 0; i < data_parallel.size(); ++i)
    data_parallel[i] -= noise_parallel[i];
  for (size_t i = 0; i < data_perpendic.size(); ++i)
    data_perpendic[i] -= noise_perpendic[i];
  // SaveChart(path.remove_noise);
  SaveChart(path.remove_noise, {data_parallel, data_perpendic});
}

double Transformer::MinSig(const vector<vector<double>> &vv) {
  auto res = vv.front().front();
  for (auto &v : vv)
    res = min(res, *min_element(v.begin(), v.end()));
  return res;
}
double Transformer::MaxSig(const vector<vector<double>> &vv) {
  auto res = vv.front().front();
  for (auto &v : vv)
    res = max(res, *max_element(v.begin(), v.end()));
  return res;
}

void Transformer::RemoveZeroPath() {
  // TODO: calculate; diff is rough
  const double diff = 30;
  int i;
  for (i = 0; i < data_parallel.size() - 1; ++i) {
    if (abs(data_parallel[i] - data_parallel[i + 1]) > diff)
      break;
  }
  data_parallel = {data_parallel.begin() + i, data_parallel.end()};
  data_perpendic = {data_perpendic.begin() + i, data_perpendic.end()};
  // SaveChart(path.remove_zero_path);
  SaveChart(path.remove_zero_path, {data_parallel, data_perpendic});
}

void Transformer::Inverse() {
  for (auto &el : data_parallel)
    el = -el;
  for (auto &el : data_perpendic)
    el = -el;
  // SaveChart(path.inverse);
  SaveChart(path.inverse, {data_parallel, data_perpendic});
}

void Transformer::Normalize() {
  // SaveChart(path.normalize, true);
  SaveChart(path.normalize, {data_parallel, data_perpendic}, true);
}

void Transformer::Filter() {
  filter(data_parallel);
  filter(data_perpendic);
  // SaveChart(path.filter, true);
  SaveChart(path.filter, {data_parallel, data_perpendic}, true);
}

void Transformer::GetResult() {
  auto it1 = data_parallel.begin();
  auto it2 = data_perpendic.begin();
  for (; it1 != data_parallel.end() && it2 != data_perpendic.end();
       ++it1, ++it2) {
    result.push_back(sqrt(*it1 * *it1) + sqrt(*it2 * *it2));
  }
  // SaveChartResult(path.result, true);
  SaveChart(path.result, {result}, true);
}