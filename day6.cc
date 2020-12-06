#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <set>
#include <utility>

using ::std::cin;
using ::std::cout;
using ::std::endl;
using ::std::function;
using ::std::getline;
using ::std::insert_iterator;
using ::std::inserter;
using ::std::string;
using ::std::set;
using ::std::set_intersection;
using ::std::set_union;

using SetOp = function<insert_iterator<set<char>>(set<char>::iterator,
                                                  set<char>::iterator,
                                                  set<char>::iterator,
                                                  set<char>::iterator,
                                                  insert_iterator<set<char>>)>;

const SetOp kSetUnion = set_union<set<char>::iterator, set<char>::iterator, insert_iterator<set<char>>>;
const SetOp kSetIntersection = set_intersection<set<char>::iterator, set<char>::iterator, insert_iterator<set<char>>>;

set<char> set_op(const set<char> &fst, const set<char> &snd, const SetOp &op) {
  set<char> result;
  op(fst.begin(), fst.end(), snd.begin(), snd.end(), inserter(result, result.end()));
  return result;
}

int main() {
  string line;
  set<char> group_union;
  set<char> group_intersection;
  bool first_in_group = true;
  int union_sum = 0;
  int intersection_sum = 0;

  while (getline(cin, line)) {
    if (line.empty()) {
      union_sum += group_union.size();
      group_union.clear();
      intersection_sum += group_intersection.size();
      group_intersection.clear();
      first_in_group = true;
      continue;
    }

    set<char> person(line.begin(), line.end());
    if (first_in_group) {
      group_union = person;
      group_intersection = std::move(person);
      first_in_group = false;
    } else {
      group_union = set_op(group_union, person, kSetUnion);
      group_intersection = set_op(group_intersection, person, kSetIntersection);
    }
  }

  if (!group_union.empty()) union_sum += group_union.size();
  if (!group_intersection.empty()) intersection_sum += group_intersection.size();
  cout << "Part 1: " << union_sum << endl;
  cout << "Part 2: " << intersection_sum << endl;

  return 0;
}