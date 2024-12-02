#include "day16.h"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <queue>
#include <regex>
#include <set>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

using namespace std;

struct Interval {
  int field_id;
  int low;
  int high;
};

struct IntervalTreeNode;

using IntervalTree = unique_ptr<IntervalTreeNode>;

struct IntervalTreeNode {
  Interval interval;
  int max;
  IntervalTree left;
  IntervalTree right;
};

// Returns a balanced interval tree built from the intervals in the range given
// by the random-access iterators `begin` and `end`.
template <typename Iterator>
IntervalTree MakeIntervalTree(Iterator begin, Iterator end) {
  IntervalTree tree;
  if (auto d = distance(begin, end); d > 0) {
    Iterator mid = begin + d / 2;
    tree = make_unique<IntervalTreeNode>(
        IntervalTreeNode{.interval = *mid, .max = mid->high});
    tree->left = MakeIntervalTree(begin, mid);
    if (tree->left && tree->left->max > tree->max) {
      tree->max = tree->left->max;
    }
    tree->right = MakeIntervalTree(mid + 1, end);
    if (tree->right && tree->right->max > tree->max) {
      tree->max = tree->right->max;
    }
  }
  return tree;
}

bool Contains(const IntervalTreeNode* root, int x) {
  if (root == nullptr || x > root->max) return false;
  if (x < root->interval.low) return Contains(root->left.get(), x);
  if (x > root->interval.high) return Contains(root->right.get(), x);
  return true;
}

set<int> GetFieldIds(const IntervalTreeNode* root, int x) {
  set<int> field_ids;
  if (root == nullptr || x > root->max) return field_ids;
  if (root->interval.low <= x && x <= root->interval.high) {
    field_ids.insert(root->interval.field_id);
  }
  set<int> left_field_ids = GetFieldIds(root->left.get(), x);
  field_ids.insert(left_field_ids.begin(), left_field_ids.end());
  set<int> right_field_ids = GetFieldIds(root->right.get(), x);
  field_ids.insert(right_field_ids.begin(), right_field_ids.end());
  return field_ids;
}

using Ticket = vector<int>;

Ticket ParseTicket(const string& line) {
  static const regex comma(",");
  auto token_it =
      sregex_token_iterator(line.begin(), line.end(), comma, /*submatch=*/-1);
  auto end_it = sregex_token_iterator();
  Ticket ticket;
  while (token_it != end_it) ticket.push_back(stoi(*token_it++));
  return ticket;
}

pair<bool, int> IsValidAndErrorRate(const IntervalTreeNode* root,
                                    const Ticket& ticket) {
  int error_rate = 0;
  bool is_valid = true;
  for (int field_value : ticket) {
    if (!Contains(root, field_value)) {
      is_valid = false;
      error_rate += field_value;
    }
  }
  return {is_valid, error_rate};
}

pair<vector<string>, IntervalTree> ParseFieldSpec(istream& in) {
  string line;
  vector<Interval> intervals;
  vector<string> fields;
  while (getline(in, line) && !line.empty()) {
    static const regex field_regex(R"((.*): (\d+)-(\d+) or (\d+)-(\d+))");
    if (smatch match_result; regex_match(line, match_result, field_regex)) {
      int field_id = fields.size();
      fields.push_back(match_result[1]);
      intervals.push_back({.field_id = field_id,
                           .low = stoi(match_result[2]),
                           .high = stoi(match_result[3])});
      intervals.push_back({.field_id = field_id,
                           .low = stoi(match_result[4]),
                           .high = stoi(match_result[5])});
    }
  }
  sort(intervals.begin(), intervals.end(),
       [](const Interval& lhs, const Interval& rhs) {
         return lhs.low < rhs.low;
       });
  IntervalTree tree = MakeIntervalTree(intervals.begin(), intervals.end());
  return {move(fields), move(tree)};
}

tuple<vector<string>, IntervalTree, vector<Ticket>> ReadInput(istream& in) {
  auto [fields, interval_tree] = ParseFieldSpec(in);

  string line;
  getline(in, line);  // Ignore "your ticket:"
  getline(in, line);
  Ticket my_ticket = ParseTicket(line);

  getline(in, line);  // Ignore empty line
  getline(in, line);  // Ignore "nearby tickets:"
  vector<Ticket> tickets = {my_ticket};
  while (getline(in, line) && !line.empty()) {
    tickets.push_back(ParseTicket(line));
  }

  return {move(fields), move(interval_tree), move(tickets)};
}

void PropagateUnits(vector<set<int>>& potential_field_ids) {
  queue<pair<int, int>> worklist;
  for (int i = 0; i < potential_field_ids.size(); ++i) {
    if (potential_field_ids[i].size() == 1) {
      worklist.emplace(i, *potential_field_ids[i].begin());
    }
  }
  while (!worklist.empty()) {
    auto [i, field_id] = worklist.front();
    worklist.pop();
    for (int j = 0; j < potential_field_ids.size(); ++j) {
      if (i != j && potential_field_ids[j].erase(field_id) > 0 &&
          potential_field_ids[j].size() == 1) {
        worklist.emplace(j, *potential_field_ids[j].begin());
      }
    }
  }
}

void day16(istream& in, ostream& out) {
  auto [fields, tree, tickets] = ReadInput(in);
  int total_error_rate = 0;
  int total_fields = fields.size();
  vector<int> all_field_ids(total_fields);
  iota(all_field_ids.begin(), all_field_ids.end(), 0);
  vector<set<int>> potential_field_ids(
      total_fields, set(all_field_ids.begin(), all_field_ids.end()));

  for (Ticket& ticket : tickets) {
    if (auto [is_valid, error_rate] = IsValidAndErrorRate(tree.get(), ticket);
        !is_valid) {
      total_error_rate += error_rate;
      continue;
    }
    for (int i = 0; i < total_fields; ++i) {
      set<int> field_ids = GetFieldIds(tree.get(), ticket[i]);
      set<int> intersection;
      set_intersection(potential_field_ids[i].begin(),
                       potential_field_ids[i].end(), field_ids.begin(),
                       field_ids.end(),
                       inserter(intersection, intersection.end()));
      potential_field_ids[i] = intersection;
    }
  }
  PropagateUnits(potential_field_ids);

  int64_t product = 1;
  for (int i = 0; i < total_fields; ++i) {
    int field_id = *potential_field_ids[i].begin();
    constexpr string_view departure = "departure";
    if (fields[field_id].substr(
            0, min(departure.size(), fields[field_id].size())) == departure) {
      product *= tickets[0][i];
    }
  }
  out << "Part 1: " << total_error_rate << endl;
  out << "Part 2: " << product << endl;
}