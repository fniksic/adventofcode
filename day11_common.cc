#include "day11_common.h"

#include <iostream>
#include <queue>
#include <string>
#include <utility>
#include <vector>

using ::std::cin;
using ::std::pair;
using ::std::queue;
using ::std::string;
using ::std::vector;

vector<string> ReadInput() {
  string line;
  vector<string> input;
  while (cin >> line) input.push_back(move(line));
  return input;
}

vector<vector<Cell>> GenerateCells(const vector<string> &input, const NeighborFinder &find_neighbors) {
  int height = input.size();
  int width = input[0].size();
  vector<vector<Cell>> cells(height, vector<Cell>(width));
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      Cell &cell = cells[i][j];
      cell.is_seat = input[i][j] == 'L';
      if (cell.is_seat) find_neighbors(cells, height, width, i, j);
    }
  }
  return cells;
}

queue<pair<int, int>> InitWorklist(vector<vector<Cell>> &cells, int neighbor_sensitivity) {
  queue<pair<int, int>> worklist;
  for (int i = 0; i < cells.size(); ++i) {
    for (int j = 0; j < cells[i].size(); ++j) {
      Cell &cell = cells[i][j];
      cell.occupied_neighbors = cell.neighbors.size();
      if (cell.is_seat && cell.occupied_neighbors < neighbor_sensitivity) {
        cell.final_state = Cell::kOccupied;
        worklist.emplace(i, j);
      }
    }
  }
  return worklist;
}

void FindEquilibrium(vector<vector<Cell>> &cells, int neighbor_sensitivity) {
  queue<pair<int, int>> worklist = InitWorklist(cells, neighbor_sensitivity);
  while (!worklist.empty()) {
    auto[i, j] = worklist.front();
    Cell &cell = cells[i][j];
    worklist.pop();
    if (cell.final_state == Cell::kUnoccupied) {
      for (auto[nb_i, nb_j] : cell.neighbors) {
        Cell &neighbor = cells[nb_i][nb_j];
        if (neighbor.final_state == Cell::kUnknown && --neighbor.occupied_neighbors < neighbor_sensitivity) {
          neighbor.final_state = Cell::kOccupied;
          worklist.emplace(nb_i, nb_j);
        }
      }
    } else {  // cell.final_state == kOccupied
      for (auto[nb_i, nb_j] : cell.neighbors) {
        Cell &neighbor = cells[nb_i][nb_j];
        if (neighbor.final_state == Cell::kUnknown) {
          neighbor.final_state = Cell::kUnoccupied;
          worklist.emplace(nb_i, nb_j);
        }
      }
    }
  }
}

int CountOccupied(vector<vector<Cell>> &cells) {
  int count = 0;
  for (vector<Cell> &row : cells) {
    for (Cell &cell : row) {
      if (cell.final_state == Cell::kOccupied) ++count;
    }
  }
  return count;
}