#include "day11.h"

#include <array>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <utility>
#include <vector>

using namespace std;

struct Cell {
  enum FinalState { kUnknown, kOccupied, kUnoccupied };
  bool is_seat;
  FinalState final_state;
  // Upper bound on the number of neighbors whose final state is occupied
  int occupied_neighbors;
  vector<pair<int, int>> neighbors;
};

struct Direction {
  int i;
  int j;
};

// Due to symmetry, we only need four directions
constexpr array kDirections = {Direction{-1, 1}, Direction{-1, 0},
                               Direction{-1, -1}, Direction{0, -1}};

inline bool InLimits(int i, int j, int height, int width) {
  return 0 <= i && i < height && 0 <= j && j < width;
}

vector<string> ReadInput(istream& in) {
  string line;
  vector<string> input;
  while (in >> line) input.push_back(move(line));
  return input;
}

using NeighborFinder =
    function<void(vector<vector<Cell>>&, int, int, int, int)>;

vector<vector<Cell>> GenerateCells(const vector<string>& input,
                                   const NeighborFinder& find_neighbors) {
  int height = input.size();
  int width = input[0].size();
  vector<vector<Cell>> cells(height, vector<Cell>(width));
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      Cell& cell = cells[i][j];
      cell.is_seat = input[i][j] == 'L';
      if (cell.is_seat) find_neighbors(cells, height, width, i, j);
    }
  }
  return cells;
}

queue<pair<int, int>> InitWorklist(vector<vector<Cell>>& cells,
                                   int neighbor_sensitivity) {
  queue<pair<int, int>> worklist;
  for (int i = 0; i < cells.size(); ++i) {
    for (int j = 0; j < cells[i].size(); ++j) {
      Cell& cell = cells[i][j];
      cell.occupied_neighbors = cell.neighbors.size();
      if (cell.is_seat && cell.occupied_neighbors < neighbor_sensitivity) {
        cell.final_state = Cell::kOccupied;
        worklist.emplace(i, j);
      }
    }
  }
  return worklist;
}

void FindEquilibrium(vector<vector<Cell>>& cells, int neighbor_sensitivity) {
  queue<pair<int, int>> worklist = InitWorklist(cells, neighbor_sensitivity);
  while (!worklist.empty()) {
    auto [i, j] = worklist.front();
    Cell& cell = cells[i][j];
    worklist.pop();
    if (cell.final_state == Cell::kUnoccupied) {
      for (auto [nb_i, nb_j] : cell.neighbors) {
        Cell& neighbor = cells[nb_i][nb_j];
        if (neighbor.final_state == Cell::kUnknown &&
            --neighbor.occupied_neighbors < neighbor_sensitivity) {
          neighbor.final_state = Cell::kOccupied;
          worklist.emplace(nb_i, nb_j);
        }
      }
    } else {  // cell.final_state == kOccupied
      for (auto [nb_i, nb_j] : cell.neighbors) {
        Cell& neighbor = cells[nb_i][nb_j];
        if (neighbor.final_state == Cell::kUnknown) {
          neighbor.final_state = Cell::kUnoccupied;
          worklist.emplace(nb_i, nb_j);
        }
      }
    }
  }
}

int CountOccupied(vector<vector<Cell>>& cells) {
  int count = 0;
  for (vector<Cell>& row : cells) {
    for (Cell& cell : row) {
      if (cell.final_state == Cell::kOccupied) ++count;
    }
  }
  return count;
}

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
  FindEquilibrium(cells, /*neighbor_sensitivity=*/4);
  out << "Part 1: " << CountOccupied(cells) << endl;
}

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
  FindEquilibrium(cells, /*neighbor_sensitivity=*/5);
  out << "Part 2: " << CountOccupied(cells) << endl;
}