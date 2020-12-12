#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

unordered_map<string, int> bag_index;
vector<vector<pair<int, int>>> contains;
vector<vector<int>> contained_in;

int GetBagIndex(const string& color) {
  auto iter = bag_index.find(color);
  if (iter == bag_index.end()) {
    bag_index[color] = contains.size();
    contains.emplace_back();
    contained_in.emplace_back();
    return contains.size() - 1;
  }
  return iter->second;
}

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
    contains[index].emplace_back(other_index, quantity);
    contained_in[other_index].push_back(index);

    // "(bag|bags)(,|.)"
    in >> token;
    if (token[token.size() - 1] == '.') break;
  }
}

int ContainedIn(int index, unordered_set<int>& visited) {
  int result = 0;
  for (int other_index : contained_in[index]) {
    if (visited.find(other_index) == visited.end()) {
      visited.insert(other_index);
      result += 1 + ContainedIn(other_index, visited);
    }
  }
  return result;
}

// Hopefully there are no cycles
int Contains(int index) {
  int result = 0;
  for (auto [other_index, quantity] : contains[index]) {
    result += quantity * (1 + Contains(other_index));
  }
  return result;
}

int main() {
  string line;
  while (getline(cin, line)) {
    if (!line.empty()) ConsumeLine(move(line));
  }

  int shiny_gold = GetBagIndex("shiny gold");
  unordered_set<int> visited = {shiny_gold};
  cout << "Part 1: " << ContainedIn(shiny_gold, visited) << endl;
  cout << "Part 2: " << Contains(shiny_gold) << endl;

  return 0;
}