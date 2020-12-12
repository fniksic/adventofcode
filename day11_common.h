#ifndef ADVENTOFCODE__DAY11_COMMON_H_
#define ADVENTOFCODE__DAY11_COMMON_H_

#include <array>
#include <functional>
#include <string>
#include <utility>
#include <vector>

std::vector<std::string> ReadInput();

struct Cell {
  enum FinalState { kUnknown, kOccupied, kUnoccupied };
  bool is_seat;
  FinalState final_state;
  // Upper bound on the number of neighbors whose final state is occupied
  int occupied_neighbors;
  std::vector<std::pair<int, int>> neighbors;
};

struct Direction {
  int i;
  int j;
};

// Due to symmetry, we only need four directions
inline constexpr std::array kDirections = {Direction{-1, 1}, Direction{-1, 0}, Direction{-1, -1}, Direction{0, -1}};

inline bool InLimits(int i, int j, int height, int width) {
  return 0 <= i && i < height && 0 <= j && j < width;
}

using NeighborFinder = std::function<void(std::vector<std::vector<Cell>>&, int, int, int, int)>;

std::vector<std::vector<Cell>> GenerateCells(const std::vector<std::string>& input,
                                             const NeighborFinder& find_neighbors);

void FindEquilibrium(std::vector<std::vector<Cell>>& cells, int neighbor_sensitivity);

int CountOccupied(std::vector<std::vector<Cell>>& cells);

#endif //ADVENTOFCODE__DAY11_COMMON_H_
