#include "day20.h"

#include <array>
#include <iostream>
#include <numeric>
#include <optional>
#include <regex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

constexpr array<pair<int, int>, 8> kDihedralGroup = {
    // Rotations by 0, 90, 180, and 270 degrees counterclockwise
    make_pair(0, 0), make_pair(1, 0), make_pair(2, 0), make_pair(3, 0),
    // Reflections by the horizontal, top-left-bottom-right, vertical, and
    // bottom-left-top-right line
    make_pair(0, 1), make_pair(1, 1), make_pair(2, 1), make_pair(3, 1)};

constexpr pair<int, int> operator*(const pair<int, int>& lhs,
                                   const pair<int, int>& rhs) {
  return make_pair(
      (lhs.second ? 4 + lhs.first - rhs.first : lhs.first + rhs.first) & 0b11,
      lhs.second ^ rhs.second);
}

constexpr pair<int, int> Inverse(const pair<int, int>& symmetry) {
  return make_pair(
      symmetry.second ? symmetry.first : (4 - symmetry.first) & 0b11,
      symmetry.second);
}

constexpr int ToIndex(const pair<int, int>& symmetry) {
  return symmetry.first + 4 * symmetry.second;
}

constexpr int Compose(const int r, const int s) {
  return ToIndex(kDihedralGroup[s] * kDihedralGroup[r]);
}

constexpr int Inverse(const int s) {
  return ToIndex(Inverse(kDihedralGroup[s]));
}

struct Vec {
  int x;
  int y;
};

using Matrix = pair<Vec, Vec>;

constexpr array<Matrix, 8> kMatrixRepresentation = {
    // Rotations by 0, 90, 180, and 270 degrees counterclockwise
    Matrix{{1, 0}, {0, 1}}, Matrix{{0, 1}, {-1, 0}}, Matrix{{-1, 0}, {0, -1}},
    Matrix{{0, -1}, {1, 0}},
    // Reflections by the horizontal, top-left-bottom-right, vertical, and
    // bottom-left-top-right line
    Matrix{{1, 0}, {0, -1}}, Matrix{{0, 1}, {1, 0}}, Matrix{{-1, 0}, {0, 1}},
    Matrix{{0, -1}, {-1, 0}}};

constexpr pair<int, int> TransformCoordinates(int i, int j, int size,
                                              int symmetry) {
  const auto& [a, b] = kMatrixRepresentation[symmetry];
  return make_pair((size - 1) * (1 + a.y - b.y) / 2 + b.y * i - a.y * j,
                   (size - 1) * (1 - a.x + b.x) / 2 + a.x * j - b.x * i);
}

constexpr array<pair<int, int>, 8> kRightSideAfterTransform = {
    make_pair(3, 0), make_pair(2, 1), make_pair(1, 1), make_pair(0, 0),
    make_pair(3, 1), make_pair(0, 1), make_pair(1, 0), make_pair(2, 0)};

constexpr array<pair<int, int>, 8> kBottomSideAfterTransform = {
    make_pair(2, 0), make_pair(1, 0), make_pair(0, 1), make_pair(3, 1),
    make_pair(0, 0), make_pair(1, 1), make_pair(2, 1), make_pair(3, 0)};

struct Tile {
  int id = 0;
  // For each side we keep normal and reversed binary encoding
  array<array<int, 2>, 4> sides = {};
  vector<string> image;
};

constexpr int GetBit(char c) { return c == '#' ? 1 : 0; }

optional<Tile> ReadTile(istream& in) {
  Tile tile;

  string line;
  getline(in, line);
  static const regex heading(R"(Tile (\d+):)");
  if (smatch match_result; regex_match(line, match_result, heading)) {
    tile.id = stoi(match_result[1]);
  } else {
    return nullopt;
  }

  for (int i = 0; getline(in, line) && !line.empty(); ++i) {
    if (i == 0 || i == line.size() - 1) {
      int side = i == 0 ? 0 : 2;  // top or bottom
      for (int j = 0; j < line.size(); ++j) {
        int bit = GetBit(line[j]);
        tile.sides[side][0] |= bit << (line.size() - j - 1);
        tile.sides[side][1] |= bit << j;
      }
    } else {
      tile.image.push_back(line.substr(1, line.size() - 2));
    }
    // left side
    int left_bit = GetBit(line.front());
    tile.sides[1][0] |= left_bit << (line.size() - i - 1);
    tile.sides[1][1] |= left_bit << i;
    // right side
    int right_bit = GetBit(line.back());
    tile.sides[3][0] |= right_bit << (line.size() - i - 1);
    tile.sides[3][1] |= right_bit << i;
  }

  return move(tile);
}

unordered_map<int, Tile> ReadInput(istream& in) {
  optional<Tile> tile;
  unordered_map<int, Tile> tiles;
  while ((tile = ReadTile(in)).has_value()) {
    tiles.insert({tile->id, move(*tile)});
  }
  return tiles;
}

vector<int> GetCornerTileIds(const unordered_map<int, Tile>& tiles) {
  vector<int> corner_tile_ids;
  unordered_map<int, int> side_occurrence;
  for (auto& [tile_id, tile] : tiles) {
    for (const array<int, 2>& side : tile.sides) {
      ++side_occurrence[side[0]];
      ++side_occurrence[side[1]];
    }
  }
  for (auto& [tile_id, tile] : tiles) {
    int occurrences = 0;
    for (const array<int, 2>& side : tile.sides) {
      occurrences += side_occurrence[side[0]];
    }
    // `occurrences` should be 6, 7, or 8, depending on whether the tile is
    // a corner tile, a side tile, or an inner tile.
    if (occurrences == 6) corner_tile_ids.push_back(tile_id);
  }
  return corner_tile_ids;
}

// Maps side to pairs of tile pointer and the index of the side in the tile
using SideToTileMap =
    unordered_map<int, vector<pair<const Tile*, pair<int, int>>>>;

SideToTileMap GetSideToTileMap(const unordered_map<int, Tile>& tiles) {
  SideToTileMap result;
  for (auto& [tile_id, tile] : tiles) {
    for (int i = 0; i < tile.sides.size(); ++i) {
      result[tile.sides[i][0]].emplace_back(&tile, make_pair(i, 0));
      result[tile.sides[i][1]].emplace_back(&tile, make_pair(i, 1));
    }
  }
  return result;
}

optional<pair<const Tile*, int>> FindTopLeftCorner(
    const unordered_map<int, Tile>& tiles, const SideToTileMap& side_to_tile) {
  for (auto& [tile_id, tile] : tiles) {
    for (int i = 0; i < tile.sides.size(); ++i) {
      if (side_to_tile.at(tile.sides[i][0]).size() == 1 &&
          side_to_tile.at(tile.sides[(i + 1) & 0b11][0]).size() == 1) {
        // We need to rotate the tile so that the unmatched corner becomes
        // the top-left corner.
        return make_pair(&tile, Inverse(i));
      }
    }
  }
  return nullopt;
}

struct NeighborOptions {
  enum class Which { kRight, kBottom };
  int neighbor_of_id = 0;
  int side = 0;
  Which which_neighbor = Which::kRight;
};

optional<pair<const Tile*, int>> FindNeighbor(
    const unordered_map<int, Tile>& tiles, const SideToTileMap& side_to_tile,
    NeighborOptions neighbor_options) {
  for (auto [neighbor, nb_side] : side_to_tile.at(neighbor_options.side)) {
    if (neighbor->id != neighbor_options.neighbor_of_id) {
      auto [nb_side_i, nb_side_j] = nb_side;
      int rotation;
      int reflection;
      if (neighbor_options.which_neighbor == NeighborOptions::Which::kRight) {
        rotation = Inverse((4 + nb_side_i - 1) & 0b11);
        reflection =
            4 * (((nb_side_i == 0 || nb_side_i == 3) & (1 - nb_side_j)) |
                 ((nb_side_i == 1 || nb_side_i == 2) & nb_side_j));
      } else {
        rotation = Inverse(nb_side_i);
        reflection =
            6 * (((nb_side_i == 1 || nb_side_i == 2) & (1 - nb_side_j)) |
                 ((nb_side_i == 0 || nb_side_i == 3) & nb_side_j));
      }
      return make_pair(neighbor, Compose(rotation, reflection));
    }
  }
  return nullopt;
}

vector<string> TransformTile(const Tile& tile, int symmetry) {
  vector<string> result;
  for (int i = 0; i < tile.image.size(); ++i) {
    result.emplace_back();
    for (int j = 0; j < tile.image.size(); ++j) {
      auto [i_prime, j_prime] =
          TransformCoordinates(i, j, tile.image.size(), Inverse(symmetry));
      result.back().push_back(tile.image[i_prime][j_prime]);
    }
  }
  return result;
}

vector<string> GetImage(const unordered_map<int, Tile>& tiles,
                        const SideToTileMap& side_to_tile, const Tile* top_left,
                        int top_left_symmetry) {
  vector<string> image;
  const Tile* first_tile = top_left;
  int first_symmetry = top_left_symmetry;
  while (true) {
    const Tile* next_tile = first_tile;
    int next_symmetry = first_symmetry;
    vector<string> row(next_tile->image.size(), string());
    while (true) {
      vector<string> transformed = TransformTile(*next_tile, next_symmetry);
      for (int i = 0; i < row.size(); ++i) {
        row[i].append(transformed[i]);
      }
      // next tile in the row
      auto [right_i, right_j] = kRightSideAfterTransform[next_symmetry];
      auto neighbor =
          FindNeighbor(tiles, side_to_tile,
                       {.neighbor_of_id = next_tile->id,
                        .side = next_tile->sides[right_i][right_j],
                        .which_neighbor = NeighborOptions::Which::kRight});
      if (!neighbor.has_value()) break;
      next_tile = neighbor->first;
      next_symmetry = neighbor->second;
    }
    image.insert(image.end(), row.begin(), row.end());
    // next tile in the column
    auto [bottom_i, bottom_j] = kBottomSideAfterTransform[first_symmetry];
    auto neighbor =
        FindNeighbor(tiles, side_to_tile,
                     {.neighbor_of_id = first_tile->id,
                      .side = first_tile->sides[bottom_i][bottom_j],
                      .which_neighbor = NeighborOptions::Which::kBottom});
    if (!neighbor.has_value()) break;
    first_tile = neighbor->first;
    first_symmetry = neighbor->second;
  }
  return image;
}

constexpr array<string_view, 3> kMonsterPattern = {
    "                  # ", "#    ##    ##    ###", " #  #  #  #  #  #   "};
constexpr int kMonsterHashes = 15;

bool MatchMonster(const vector<string>& image, int i, int j, int symmetry) {
  for (int k = 0; k < 3; ++k) {
    for (int l = 0; l < kMonsterPattern[k].size(); ++l) {
      auto [ik, jl] =
          TransformCoordinates(i + k, j + l, image.size(), symmetry);
      if (kMonsterPattern[k][l] == '#' && image[ik][jl] != '#') return false;
    }
  }
  return true;
}

int CountMonsters(const vector<string>& image) {
  for (int symmetry = 0; symmetry < kDihedralGroup.size(); ++symmetry) {
    int count = 0;
    for (int i = 1; i < image.size() - 1; ++i) {
      for (int j = 0; j < image.size() - kMonsterPattern[1].size(); ++j) {
        if (MatchMonster(image, i, j, symmetry)) ++count;
      }
    }
    if (count > 0) return count;
  }
  return 0;
}

int CountHashes(const vector<string>& image) {
  int count = 0;
  for (const string& row : image) {
    for (char c : row) {
      if (c == '#') ++count;
    }
  }
  return count;
}

void day20(istream& in, ostream& out) {
  unordered_map<int, Tile> tiles = ReadInput(in);
  vector<int> corner_tile_ids = GetCornerTileIds(tiles);
  out << "Part 1: "
      << accumulate(corner_tile_ids.begin(), corner_tile_ids.end(), 1LL,
                    multiplies<>())
      << endl;

  SideToTileMap side_to_tile = GetSideToTileMap(tiles);
  auto optional_top_left = FindTopLeftCorner(tiles, side_to_tile);
  if (!optional_top_left.has_value()) return;
  auto [top_left, top_left_symmetry] = optional_top_left.value();
  vector<string> image =
      GetImage(tiles, side_to_tile, top_left, top_left_symmetry);
  out << "Part 2: "
      << CountHashes(image) - kMonsterHashes * CountMonsters(image) << endl;
}
