#include "transformer.h"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;
using namespace helper;

void Transformer::SaveChart(const string &path, bool normalized) {
  // plt::clf();
  // plt::figure_size(1200, 780);
  plt::figure();
  static const map<string, string> settings = {{"linewidth", "0.5"}};
  vector<double> x(data_parallel.size());
  if (normalized) {
    double k_horiz = 12'000 * 1.0 / x.size();
    for (int i = 0; i < x.size(); ++i)
      x[i] = i * k_horiz;

    // use data upped on min val
    auto min_par = *min_element(data_parallel.begin(), data_parallel.end());
    auto par_up = data_parallel;
    if (min_par < 0)
      for (auto &el : par_up)
        el -= min_par;

    auto min_per = *min_element(data_perpendic.begin(), data_perpendic.end());
    auto per_up = data_perpendic;
    if (min_per < 0)
      for (auto &el : per_up)
        el -= min_per;

    plt::xlim(0, 12'000);
    plt::ylim(0, (int)get_signal_max(par_up, per_up));

    plt::plot(x, par_up, settings);
    plt::plot(x, per_up, settings);
  } else {
    for (int i = 0; i < x.size(); ++i)
      x[i] = i;
    plt::xlim(0, (int)x.size());
    plt::ylim((int)get_signal_min(data_parallel, data_perpendic),
              (int)get_signal_max(data_parallel, data_perpendic));
    plt::plot(x, data_parallel, settings);
    plt::plot(x, data_perpendic, settings);
  }

  plt::save(path);
  plt::close();
}

void Transformer::SaveChartResult(const string &path, bool normalized) {
  // plt::clf();
  // plt::figure_size(1200, 780);
  plt::figure();
  static const map<string, string> settings = {{"linewidth", "0.5"}};
  vector<double> x(result.size());
  if (normalized) {
    double k_horiz = 12'000 * 1.0 / x.size();
    for (int i = 0; i < x.size(); ++i)
      x[i] = i * k_horiz;

    auto min_res = *min_element(result.begin(), result.end());
    auto res_up = result;
    if (min_res < 0)
      for (auto &el : result)
        el -= min_res;

    plt::xlim(0, 12'000);
    plt::ylim(0, (int)*max_element(res_up.begin(), res_up.end()));

    plt::plot(x, res_up, settings);
  } else {
    for (int i = 0; i < x.size(); ++i)
      x[i] = i;
    plt::xlim(0, (int)x.size());
    plt::ylim((int)*min_element(result.begin(), result.end()),
              (int)*max_element(result.begin(), result.end()));
    plt::plot(x, result, settings);
  }

  plt::save(path);
  plt::close();
}

void Transformer::RemoveNoise() {
  for (size_t i = 0; i < data_parallel.size(); ++i)
    data_parallel[i] -= noise_parallel[i];
  for (size_t i = 0; i < data_perpendic.size(); ++i)
    data_perpendic[i] -= noise_perpendic[i];
  SaveChart(path.remove_noise);
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
  SaveChart(path.remove_zero_path);
}

void Transformer::Inverse() {
  for (auto &el : data_parallel)
    el = -el;
  for (auto &el : data_perpendic)
    el = -el;
  SaveChart(path.inverse);
}

void Transformer::Normalize() { SaveChart(path.normalize, true); }

void Transformer::Filter() {
  filter(data_parallel);
  filter(data_perpendic);
  SaveChart(path.filter, true);
}

void Transformer::GetResult() {
  auto it1 = data_parallel.begin();
  auto it2 = data_perpendic.begin();
  for (; it1 != data_parallel.end() && it2 != data_perpendic.end();
       ++it1, ++it2) {
    result.push_back(sqrt(*it1 * *it1) + sqrt(*it2 * *it2));
  }
  SaveChartResult(path.result, true);
}