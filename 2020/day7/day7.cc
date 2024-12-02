#include "day7.h"

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

class BagGraph {
 public:
  void ConsumeLine(string&& line) {
    istringstream in(forward<string>(line));
    string token, extra_token, extra_extra_token;

    // Bag color: "adjective1 adjective2"
    in >> token >> extra_token;
    string color = token + ' ' + extra_token;
    int index = GetBagIndex(color);

    // Ignore: "bags contain"
    in >> token >> extra_token;

    while (true) {
      // "quantity adjective1 adjective2" or "no other bags."
      in >> token >> extra_token >> extra_extra_token;
      if (token == "no") break;
      int quantity = stoi(token);
      string other_color = extra_token + ' ' + extra_extra_token;
      int other_index = GetBagIndex(other_color);
      contains_[index].emplace_back(other_index, quantity);
      contained_in_[other_index].push_back(index);

      // "(bag|bags)(,|.)"
      in >> token;
      if (token[token.size() - 1] == '.') break;
    }
  }

  int ContainedIn(const string& bag_color) {
    int bag_index = GetBagIndex(bag_color);
    unordered_set<int> visited = {bag_index};
    return ContainedIn(bag_index, visited);
  }

  int Contains(const string& bag_color) {
    int bag_index = GetBagIndex(bag_color);
    return Contains(bag_index);
  }

 private:
  int GetBagIndex(const string& color) {
    auto iter = bag_index_.find(color);
    if (iter == bag_index_.end()) {
      bag_index_[color] = contains_.size();
      contains_.emplace_back();
      contained_in_.emplace_back();
      return contains_.size() - 1;
    }
    return iter->second;
  }

  int ContainedIn(int index, unordered_set<int>& visited) const {
    int result = 0;
    for (int other_index : contained_in_[index]) {
      if (visited.find(other_index) == visited.end()) {
        visited.insert(other_index);
        result += 1 + ContainedIn(other_index, visited);
      }
    }
    return result;
  }

  // Hopefully there are no cycles
  int Contains(int index) const {
    int result = 0;
    for (auto [other_index, quantity] : contains_[index]) {
      result += quantity * (1 + Contains(other_index));
    }
    return result;
  }

  unordered_map<string, int> bag_index_;
  vector<vector<pair<int, int>>> contains_;
  vector<vector<int>> contained_in_;
};

void day7(istream& in, ostream& out) {
  BagGraph bag_graph;

  string line;
  while (getline(in, line)) {
    if (!line.empty()) bag_graph.ConsumeLine(move(line));
  }

  string shiny_gold = "shiny gold";
  out << "Part 1: " << bag_graph.ContainedIn(shiny_gold) << endl;
  out << "Part 2: " << bag_graph.Contains(shiny_gold) << endl;
}
