#include "transformer.h"

int main() {
  { Transformer t("../2017-04-11_-_14-06-06_4_raw.csv", "../noise.csv"); }
  { Transformer t("../2017-04-11_-_14-56-03_171_raw.csv", "../noise.csv"); }
  { Transformer t("../2017-04-12_-_11-19-39_6_raw.csv", "../noise.csv"); }
  return 0;
}