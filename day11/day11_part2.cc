#include <iostream>
#include <vector>

#include "day11.h"
#include "day11_common.h"

using namespace std;

constexpr int kNeighborSensitivity = 5;

void FindVisibleSeats(vector<vector<Cell>>& cells, int height, int width, int i,
                      int j) {
  Cell& cell = cells[i][j];
  for (const Direction& d : kDirections) {
    for (int nb_i = i + d.i, nb_j = j + d.j;
         InLimits(nb_i, nb_j, height, width); nb_i += d.i, nb_j += d.j) {
      Cell& neighbor = cells[nb_i][nb_j];
      if (neighbor.is_seat) {
        cell.neighbors.emplace_back(nb_i, nb_j);
        neighbor.neighbors.emplace_back(i, j);
        break;
      }
    }
  }
}

void day11_part2(std::istream& in, std::ostream& out) {
  vector<vector<Cell>> cells = GenerateCells(ReadInput(in), FindVisibleSeats);
  FindEquilibrium(cells, kNeighborSensitivity);
  out << "Part 2: " << CountOccupied(cells) << endl;
}