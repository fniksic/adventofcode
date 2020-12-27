#include "day24.h"

#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>

using namespace std;

struct Vec {
  int x;
  int y;
};

inline bool operator==(const Vec& lhs, const Vec& rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline Vec operator+(const Vec& lhs, const Vec& rhs) {
  return {.x = lhs.x + rhs.x, .y = lhs.y + rhs.y};
}

struct HashVec {
  inline size_t operator()(const Vec& vec) const noexcept {
    return ((vec.x & 0xFFFF) << 16) | (vec.y & 0xFFFF);
  }
};

struct Tile {
  bool is_black;
  int black_neighbors;

  inline bool ShouldFlip() const {
    return (is_black && (black_neighbors == 0 || black_neighbors > 2)) ||
           (!is_black && black_neighbors == 2);
  }
};

int ToDirection(char c) {
  switch (c) {
    case 'n':
    case 'e':
      return 1;
    case 's':
    case 'w':
      return -1;
    default:
      return 0;
  }
}

Vec ParseLine(string_view line) {
  Vec result{};
  for (int i = 0; i < line.size(); ++i) {
    if (line[i] == 'n' || line[i] == 's') {
      result.y += ToDirection(line[i]);
      result.x += ToDirection(line[i + 1]);
      ++i;
    } else {
      result.x += 2 * ToDirection(line[i]);
    }
  }
  return result;
}

using Grid = unordered_map<Vec, Tile, HashVec>;
using FlipSet = unordered_set<Vec, HashVec>;

void UpdateNeighbors(const Vec& vec, int update, Grid& grid,
                     FlipSet& affected) {
  static constexpr array<Vec, 6> kNeighborDirections = {
      Vec{1, 1}, Vec{2, 0}, Vec{1, -1}, Vec{-1, -1}, Vec{-2, 0}, Vec{-1, 1}};
  for (const Vec& neighbor_direction : kNeighborDirections) {
    Vec neighbor{vec + neighbor_direction};
    Tile& neighbor_tile{grid[neighbor]};
    neighbor_tile.black_neighbors += update;
    affected.insert(neighbor);
  }
}

pair<Grid, FlipSet> ReadInput(istream& in) {
  string line{};
  Grid grid{};
  FlipSet affected{};
  while (getline(in, line) && !line.empty()) {
    Vec vec{ParseLine(line)};
    Tile& tile{grid[vec]};
    tile.is_black ^= true;
    UpdateNeighbors(vec, tile.is_black ? 1 : -1, grid, affected);
    affected.insert(vec);
  }
  return {move(grid), move(affected)};
}

int CountBlack(const Grid& grid) {
  int result{};
  for (auto& [vec, tile] : grid) {
    if (tile.is_black) ++result;
  }
  return result;
}

void SimulateDays(int days, Grid& grid, FlipSet& affected) {
  for (int i = 0; i < days; ++i) {
    FlipSet flip{};
    for (const Vec& vec : affected) {
      if (grid[vec].ShouldFlip()) flip.insert(vec);
    }
    affected.clear();
    for (const Vec& vec : flip) {
      Tile& tile{grid[vec]};
      tile.is_black ^= true;
      UpdateNeighbors(vec, tile.is_black ? 1 : -1, grid, affected);
    }
  }
}

void day24(istream& in, ostream& out) {
  auto [grid, affected]{ReadInput(in)};
  out << "Part 1: " << CountBlack(grid) << endl;
  SimulateDays(100, grid, affected);
  out << "Part 2: " << CountBlack(grid) << endl;
}
