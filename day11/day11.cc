#include "day11.h"

#include <iostream>
#include <vector>

#include "day11_common.h"

using namespace std;

constexpr int kNeighborSensitivity = 4;

void FindAdjacentSeats(vector<vector<Cell>>& cells, int height, int width,
                       int i, int j) {
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

void day11(istream& in, ostream& out) {
  vector<vector<Cell>> cells = GenerateCells(ReadInput(in), FindAdjacentSeats);
  FindEquilibrium(cells, kNeighborSensitivity);
  out << "Part 1: " << CountOccupied(cells) << endl;
}
