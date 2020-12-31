#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

namespace helper {

void Parse(string data_path, vector<double> &v1, vector<double> &v2) {
  ifstream in(data_path.c_str());
  int par, per;
  char delim;
  while (in >> par >> delim >> per >> delim) {
    v1.push_back(par);
    v2.push_back(per);
  }
}

string CreateDir(string data_path) {
  size_t index = 0;
  index = data_path.find(".csv", index);
  if (index != std::string::npos)
    data_path.replace(index, 4, "_dot_csv");

  fs::create_directory(data_path);
  return data_path;
}

// void remove_noise(vector<double> &res, vector<double> &noise) {
//   for (int i = 0; i < res.size(); ++i)
//     res[i] -= noise[i];
// }
// void remove_zero_path(vector<double> &v) {
//   if (v.size() < 2)
//     return;
//   const double valuable_diff = 30;
//   for (auto cur = v.begin() + 1, prev = v.begin(); cur != v.end();
//        ++cur, ++prev) {
//     if (abs(*cur - *prev) > valuable_diff) {
//       v = vector(prev, v.end());
//       cerr << v.size() << endl;
//       return;
//     }
//   }
// }
// void inverse(vector<double> &v) {
//   double neg_val = 0;
//   for (auto &el : v) {
//     el = -el;
//     if (el < 0) {
//       neg_val = min(neg_val, el);
//     }
//   }
//   for (auto &el : v) { // up
//     el += -neg_val;
//   }
// }
double get_signal_max(vector<double> &v1, vector<double> &v2) {
  return max(*max_element(v1.begin(), v1.end()),
             *max_element(v2.begin(), v2.end()));
}
double get_signal_min(vector<double> &v1, vector<double> &v2) {
  return min(*min_element(v1.begin(), v1.end()),
             *min_element(v2.begin(), v2.end()));
}
double mediana(vector<double> v) {
  sort(v.begin(), v.end());
  return v[1];
}
void filter(vector<double> &orig) {
  vector<double> res(orig.size());
  res.front() = orig.front(), res.back() = orig.back();
  for (int i = 1; i < orig.size() - 1; ++i) {
    res[i] = mediana(vector<double>{orig[i - 1], orig[i], orig[i + 1]});
  }
  orig = res;
}
} // namespace helper