#include <iostream>
#include <vector>

#include "day11_common.h"

using ::std::cout;
using ::std::endl;
using ::std::vector;

constexpr int kNeighborSensitivity = 5;

void FindNeighbors(vector<vector<Cell>>& cells, int height, int width, int i, int j) {
  Cell& cell = cells[i][j];
  for (const Direction& d : kDirections) {
    for (int nb_i = i + d.i, nb_j = j + d.j; InLimits(nb_i, nb_j, height, width); nb_i += d.i, nb_j += d.j) {
      Cell& neighbor = cells[nb_i][nb_j];
      if (neighbor.is_seat) {
        cell.neighbors.emplace_back(nb_i, nb_j);
        neighbor.neighbors.emplace_back(i, j);
        break;
      }
    }
  }
}

int main() {
  vector<vector<Cell>> cells = GenerateCells(ReadInput(), FindNeighbors);
  FindEquilibrium(cells, kNeighborSensitivity);
  cout << "Part 2: " << CountOccupied(cells) << endl;
  return 0;
}