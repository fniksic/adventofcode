#include "day17.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>

using namespace std;

enum Dimension { k3D = 3, k4D };

using Point = array<int, 4>;

inline Point operator+(const Point& lhs, const Point& rhs) {
  Point result = lhs;
  for (int i = 0; i < 4; ++i) result[i] += rhs[i];
  return result;
}

struct HashPoint {
  inline size_t operator()(const Point& c) const {
    return ((c[0] & 0xFF) << 24) | ((c[1] & 0xFF) << 16) |
           ((c[2] & 0xFF) << 8) | (c[3] & 0xFF);
  }
};

struct Cube {
  enum State { kInactive, kActive };
  State state;
  int active_neighbors;
};

using CubeMap = unordered_map<Point, Cube, HashPoint>;
using NeighborUpdateMap = unordered_map<Point, int, HashPoint>;

void UpdateNeighbors(NeighborUpdateMap& active_neighbor_diff, const Point& p,
                     int diff, Dimension dimension) {
  Point dp = {0};
  fill_n(dp.begin(), dimension, -1);
  constexpr Point zero = {0};
  while (true) {
    active_neighbor_diff[p + dp] += diff;
    int next_coord = 0;
    while (next_coord < dimension && dp[next_coord] == 1) {
      dp[next_coord] = -1;
      ++next_coord;
    }
    if (next_coord >= dimension) break;
    ++dp[next_coord];
    if (dp == zero) ++dp[0];
  }
}

tuple<CubeMap, CubeMap, NeighborUpdateMap, NeighborUpdateMap> ReadInput(
    istream& in) {
  string line;
  CubeMap cubes_3D;
  CubeMap cubes_4D;
  NeighborUpdateMap active_neighbor_diff_3D;
  NeighborUpdateMap active_neighbor_diff_4D;
  for (int x = 0; getline(in, line) && !line.empty(); ++x) {
    for (int y = 0; y < line.size(); ++y) {
      if (line[y] == '#') {
        Point x_y_0_0 = {x, y, 0, 0};
        cubes_3D[x_y_0_0].state = Cube::kActive;
        cubes_4D[x_y_0_0].state = Cube::kActive;
        UpdateNeighbors(active_neighbor_diff_3D, x_y_0_0, /*diff=*/1, k3D);
        UpdateNeighbors(active_neighbor_diff_4D, x_y_0_0, /*diff=*/1, k4D);
      }
    }
  }
  for (auto& [p, cube] : cubes_3D) active_neighbor_diff_3D[p] += 0;
  for (auto& [p, cube] : cubes_4D) active_neighbor_diff_4D[p] += 0;
  return {move(cubes_3D), move(cubes_4D), move(active_neighbor_diff_3D),
          move(active_neighbor_diff_4D)};
}

NeighborUpdateMap SimulateCycle(CubeMap& cubes,
                                const NeighborUpdateMap& active_neighbor_diff,
                                Dimension dimension) {
  NeighborUpdateMap next_active_neighbor_diff;
  for (auto& [p, diff] : active_neighbor_diff) {
    Cube& cube = cubes[p];
    cube.active_neighbors += diff;
    if (cube.state == Cube::kActive &&
        !(cube.active_neighbors == 2 || cube.active_neighbors == 3)) {
      cube.state = Cube::kInactive;
      UpdateNeighbors(next_active_neighbor_diff, p, /*diff=*/-1, dimension);
    } else if (cube.state == Cube::kInactive && cube.active_neighbors == 3) {
      cube.state = Cube::kActive;
      UpdateNeighbors(next_active_neighbor_diff, p, /*diff=*/1, dimension);
    }
  }
  return next_active_neighbor_diff;
}

int CountActive(const CubeMap& cubes) {
  int count = 0;
  for (auto& [p, cube] : cubes) {
    if (cube.state == Cube::kActive) ++count;
  }
  return count;
}

void day17(istream& in, ostream& out) {
  auto [cubes_3D, cubes_4D, active_neighbor_diff_3D, active_neighbor_diff_4D] =
      ReadInput(in);
  for (int i = 0; i < 6; ++i) {
    active_neighbor_diff_3D =
        SimulateCycle(cubes_3D, active_neighbor_diff_3D, k3D);
    active_neighbor_diff_4D =
        SimulateCycle(cubes_4D, active_neighbor_diff_4D, k4D);
  }
  out << "Part 1: " << CountActive(cubes_3D) << endl;
  out << "Part 2: " << CountActive(cubes_4D) << endl;
}
