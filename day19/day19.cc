#include "day19.h"

#include <iostream>
#include <optional>
#include <regex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

struct Rule {
  int id = 0;
  optional<string> regex;
  // Special case for Part 2
  optional<string> regex2;
  int length_lower_bound = 0;
  vector<vector<int>> alternatives;
};

Rule ParseRule(const string& line) {
  static const regex terminal(R"re((\d+): "(.*)")re");
  if (smatch match_result; regex_match(line, match_result, terminal)) {
    return {.id = stoi(match_result[1]),
            .regex = match_result[2],
            .regex2 = match_result[2],
            .length_lower_bound = 1};
  }

  Rule rule;
  static const regex number_or_pipe(R"re(\d+|\|)re");
  auto it = sregex_iterator(line.begin(), line.end(), number_or_pipe);
  auto end = sregex_iterator();

  rule.id = stoi(it->str());
  rule.alternatives.emplace_back();
  ++it;

  while (it != end) {
    if (it->str() == "|") {
      rule.alternatives.emplace_back();
    } else {
      rule.alternatives.back().push_back(stoi(it->str()));
    }
    ++it;
  }
  return rule;
}

unordered_map<int, Rule> ReadRules(istream& in) {
  unordered_map<int, Rule> rules;
  string line;
  while (getline(in, line) && !line.empty()) {
    Rule rule = ParseRule(line);
    rules.insert({rule.id, rule});
  }
  return rules;
}

void Visit(unordered_map<int, Rule>& rules, Rule& rule,
           int longest_message_size) {
  if (rule.regex.has_value()) return;
  rule.regex = string();
  rule.regex2 = string();
  if (rule.alternatives.size() > 1) {
    rule.regex.value().append("(");
    rule.regex2.value().append("(");
  }
  string pipe;
  for (vector<int>& alternative : rule.alternatives) {
    rule.regex.value().append(pipe);
    rule.regex2.value().append(pipe);
    int length_lower_bound = 0;
    for (int sub_id : alternative) {
      Rule& sub_rule = rules[sub_id];
      Visit(rules, sub_rule, longest_message_size);
      rule.regex.value().append(sub_rule.regex.value());
      rule.regex2.value().append(sub_rule.regex2.value());
      length_lower_bound += sub_rule.length_lower_bound;
    }
    if (rule.length_lower_bound == 0 ||
        rule.length_lower_bound > length_lower_bound) {
      rule.length_lower_bound = length_lower_bound;
    }
    pipe = "|";
  }
  if (rule.alternatives.size() > 1) {
    rule.regex.value().append(")");
    rule.regex2.value().append(")");
  }

  // Special cases for Part 2.
  if (rule.id == 8 && rule.alternatives.size() == 1 &&
      rule.alternatives[0] == vector<int>{42}) {
    rule.regex2.value().append("+");
  } else if (rule.id == 11 && rule.alternatives.size() == 1 &&
             rule.alternatives[0] == vector<int>{42, 31}) {
    string rule_42 = rules[42].regex.value();
    string rule_31 = rules[31].regex.value();
    rule.regex2 = "(" + rule.regex.value();
    int length_lower_bound = rule.length_lower_bound;
    int count = 1;
    while (length_lower_bound < longest_message_size) {
      length_lower_bound += rule.length_lower_bound;
      ++count;
      string repeated_42 = rule_42;
      repeated_42.append("{").append(to_string(count)).append("}");
      string repeated_31 = rule_31;
      repeated_31.append("{").append(to_string(count)).append("}");
      rule.regex2.value().append("|").append(repeated_42).append(repeated_31);
    }
    rule.regex2.value().append(")");
  }
}

void PopulateRegexes(unordered_map<int, Rule>& rules,
                     int longest_message_size) {
  for (auto& [id, rule] : rules) {
    Visit(rules, rule, longest_message_size);
  }
}

pair<vector<string>, int> ReadMessages(istream& in) {
  string line;
  vector<string> messages;
  int longest_message_size = 0;
  while (getline(in, line) && !line.empty()) {
    if (line.size() > longest_message_size) longest_message_size = line.size();
    messages.push_back(move(line));
  }
  return {move(messages), longest_message_size};
}

pair<int, int> Count(const vector<string>& messages, const regex& zero_regex,
                     const regex& zero_regex_part2) {
  int count = 0;
  int count_part2 = 0;
  for (const string& message : messages) {
    if (regex_match(message, zero_regex)) ++count;
    if (regex_match(message, zero_regex_part2)) ++count_part2;
  }
  return {count, count_part2};
}

void day19(istream& in, ostream& out) {
  unordered_map<int, Rule> rules = ReadRules(in);
  auto [messages, longest_message_size] = ReadMessages(in);
  PopulateRegexes(rules, longest_message_size);
  regex zero_regex = regex(rules[0].regex.value());
  regex zero_regex_part2 =
      regex(rules[0].regex2.value(), regex::ECMAScript | regex::nosubs);
  auto [count, count_part2] = Count(messages, zero_regex, zero_regex_part2);
  out << "Part 1: " << count << endl;
  out << "Part 2: " << count_part2 << endl;
}
