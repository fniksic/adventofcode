#include "day21.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <regex>
#include <set>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>

using namespace std;

using svregex_token_iterator =
    regex_token_iterator<string_view::const_iterator>;

pair<set<string>, unordered_set<string>> ParseLine(string_view line) {
  static const regex delimiters{R"([ ,()]+)"};
  set<string> ingredients{};
  unordered_set<string> allergens{};
  svregex_token_iterator it{line.begin(), line.end(), delimiters, -1};
  svregex_token_iterator end{};
  for (bool seen_contains = false; it != end; ++it) {
    if (*it == "contains") {
      seen_contains = true;
    } else if (seen_contains) {
      allergens.emplace(*it);
    } else {
      ingredients.emplace(*it);
    }
  }
  return {move(ingredients), move(allergens)};
}

tuple<map<string, set<string>>, unordered_map<string, int>, int> ReadInput(
    istream& in) {
  string line{};
  map<string, set<string>> contained_in{};
  unordered_map<string, int> ingredient_count{};
  int total_ingredient_count{};
  while (getline(in, line) && !line.empty()) {
    auto [ingredients, allergens] = ParseLine(line);
    for (auto& allergen : allergens) {
      if (auto it{contained_in.find(allergen)}; it == contained_in.end()) {
        contained_in.emplace(allergen, ingredients);
      } else {
        set<string> intersection{};
        set_intersection(it->second.begin(), it->second.end(),
                         ingredients.begin(), ingredients.end(),
                         inserter(intersection, intersection.end()));
        it->second = move(intersection);
      }
    }
    for (auto& ingredient : ingredients) ++ingredient_count[ingredient];
    total_ingredient_count += ingredients.size();
  }
  return {move(contained_in), move(ingredient_count), total_ingredient_count};
}

void PropagateIngredients(map<string, set<string>>& contained_in) {
  queue<string> worklist{};
  for (auto& [allergen, ingredients] : contained_in) {
    if (ingredients.size() == 1) worklist.push(allergen);
  }
  while (!worklist.empty()) {
    const string& ingredient{*contained_in[worklist.front()].begin()};
    for (auto& [allergen, ingredients] : contained_in) {
      if (allergen != worklist.front() && ingredients.erase(ingredient) > 0 &&
          ingredients.size() == 1) {
        worklist.push(allergen);
      }
    }
    worklist.pop();
  }
}

int CountAllergenFree(const map<string, set<string>>& contained_in,
                      unordered_map<string, int>& ingredient_count,
                      int total_ingredient_count) {
  for (auto& [allergen, ingredients] : contained_in) {
    for (auto& ingredient : ingredients) {
      if (auto it = ingredient_count.find(ingredient);
          it != ingredient_count.end()) {
        total_ingredient_count -= it->second;
        ingredient_count.erase(it);
      }
    }
  }
  return total_ingredient_count;
}

string GetCanonicalDangerousList(const map<string, set<string>>& contained_in) {
  string result{};
  string separator{};
  for (auto& [allergen, ingredients] : contained_in) {
    result.append(separator).append(*ingredients.begin());
    separator = ",";
  }
  return result;
}

void day21(istream& in, ostream& out) {
  auto [contained_in, ingredient_count, total_ingredient_count] = ReadInput(in);
  PropagateIngredients(contained_in);
  out << "Part 1: "
      << CountAllergenFree(contained_in, ingredient_count,
                           total_ingredient_count)
      << endl;
  out << "Part 2: " << GetCanonicalDangerousList(contained_in) << endl;
}
