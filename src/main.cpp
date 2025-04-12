#include "waits_predictor.hpp"
#include <chrono>
#include <format>
#include <fstream>
#include <iostream>
#include <limits>
#include <valarray>
#include <vector>

int main(int argc, char* argv[])
{
  using waits_predictor::MAX_TIDS;

  if (argc != 2) {
    std::cerr << std::format("Usage: {} [input file]\n", argv[0]);
    return EXIT_FAILURE;
  }

  std::ifstream fin(argv[1]);

  if (!fin) {
    std::cerr << "Input file does not exist\n";
    return EXIT_FAILURE;
  }

  waits_predictor::initialize(STATES_FILE_PATH);

  int m;
  std::vector<int> wall(MAX_TIDS, 4);
  std::vector<int> river(MAX_TIDS, 0);

  fin >> m;

  for (unsigned int i = 0u; i < MAX_TIDS; ++i) {
    fin.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
    fin >> wall[i] >> river[i];
  }

  std::array<waits_predictor::Value, 4u> values;

  const auto start = std::chrono::system_clock::now();

  try {
    values = waits_predictor::predict_waits(wall, river, m, true);
  }
  catch (std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  const auto end = std::chrono::system_clock::now();
  const auto props = values[0].props();

  std::cout << std::format("Total\t{:d}\n", values[0].all);
  std::cout << "Wait\tNormalHand\tSevenPairs\tThirteenOrphans\tProportion\n";

  for (unsigned int i = 0u; i < MAX_TIDS; ++i) {
    std::cout << std::format("{:d}\t{:d}\t{:d}\t{:d}\t{:6f}\n",
                             i,
                             values[1].each[i],
                             values[2].each[i],
                             values[3].each[i],
                             props[i]);
  }

  std::cout << std::format("Time (msec.): {:d}\n",
                           std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());

  return EXIT_SUCCESS;
}
