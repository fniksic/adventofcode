#include <iostream>
#include <vector>

#include "day11_common.h"

using namespace std;

constexpr int kNeighborSensitivity = 4;

void FindNeighbors(vector<vector<Cell>>& cells, int height, int width, int i,
                   int j) {
  Cell& cell = cells[i][j];
  for (const Direction& d : kDirections) {
    int nb_i = i + d.i;
    int nb_j = j + d.j;
    if (InLimits(nb_i, nb_j, height, width)) {
      Cell& neighbor = cells[nb_i][nb_j];
      if (neighbor.is_seat) {
        cell.neighbors.emplace_back(nb_i, nb_j);
        neighbor.neighbors.emplace_back(i, j);
      }
    }
  }
}

int main() {
  vector<vector<Cell>> cells = GenerateCells(ReadInput(), FindNeighbors);
  FindEquilibrium(cells, kNeighborSensitivity);
  cout << "Part 1: " << CountOccupied(cells) << endl;
  return 0;
}