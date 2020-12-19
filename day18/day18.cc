#include "day18.h"

#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <string_view>
#include <utility>
#include <variant>

using namespace std;

class NumExpr;
class AddExpr;
class MulExpr;

using Expression = variant<NumExpr, AddExpr, MulExpr>;

struct NumExpr {
  int64_t value;
};

struct AddExpr {
  unique_ptr<Expression> left;
  unique_ptr<Expression> right;
};

struct MulExpr {
  unique_ptr<Expression> left;
  unique_ptr<Expression> right;
};

unique_ptr<Expression> Num(int64_t value) {
  return make_unique<Expression>(NumExpr{value});
}

unique_ptr<Expression> Add(unique_ptr<Expression> left,
                           unique_ptr<Expression> right) {
  return make_unique<Expression>(AddExpr{move(left), move(right)});
}

unique_ptr<Expression> Mul(unique_ptr<Expression> left,
                           unique_ptr<Expression> right) {
  return make_unique<Expression>(MulExpr{move(left), move(right)});
}

template <typename... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};

template <typename... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

template <typename CaseNum, typename CaseAdd, typename CaseMul>
auto match(const Expression& expr, CaseNum case_num, CaseAdd case_add,
           CaseMul case_mul) {
  return visit(overloaded{case_num, case_add, case_mul}, expr);
}

int64_t Evaluate(const Expression& expr) {
  return match(
      expr, [](const NumExpr& num_expr) { return num_expr.value; },
      [](const AddExpr& add_expr) {
        return Evaluate(*add_expr.left) + Evaluate(*add_expr.right);
      },
      [](const MulExpr& mul_expr) {
        return Evaluate(*mul_expr.left) * Evaluate(*mul_expr.right);
      });
}

string_view ConsumeWhitespace(string_view line) {
  auto first_non_space = line.find_first_not_of(' ');
  if (first_non_space == string_view::npos) first_non_space = line.size();
  return line.substr(first_non_space);
}

enum class PrecedenceOption { kEqual, kAddBeforeMul };

int GetPrecedence(char op, PrecedenceOption precedence_option) {
  switch (op) {
    case '+':
      return precedence_option == PrecedenceOption::kAddBeforeMul ? 2 : 1;
    case '*':
      return 1;
    case '(':
    default:
      return 0;
  }
}

using Op = function<unique_ptr<Expression>(unique_ptr<Expression>,
                                           unique_ptr<Expression>)>;

// Discharges ops with precedence >= `precedence`.
void DischargeOps(stack<unique_ptr<Expression>>& expr_stack,
                  stack<char>& op_stack, int precedence,
                  PrecedenceOption precedence_option) {
  while (!op_stack.empty() &&
         GetPrecedence(op_stack.top(), precedence_option) >= precedence) {
    unique_ptr<Expression> rhs = move(expr_stack.top());
    expr_stack.pop();
    unique_ptr<Expression> lhs = move(expr_stack.top());
    expr_stack.pop();
    Op op = op_stack.top() == '+' ? Add : Mul;
    op_stack.pop();
    expr_stack.push(op(move(lhs), move(rhs)));
  }
}

// Assumes `line` is a well-formed arithmetic expression.
unique_ptr<Expression> ParseExpression(string_view line,
                                       PrecedenceOption precedence_option) {
  stack<unique_ptr<Expression>> expr_stack;
  stack<char> op_stack;
  while (!line.empty()) {
    switch (line[0]) {
      case '(':
        op_stack.push('(');
        line = ConsumeWhitespace(line.substr(1));
        break;
      case ')':
        DischargeOps(expr_stack, op_stack, /*precedence=*/1, precedence_option);
        op_stack.pop();  // Remove matching '('
        line = ConsumeWhitespace(line.substr(1));
        break;
      case '+': {
        int add_precedence = GetPrecedence('+', precedence_option);
        DischargeOps(expr_stack, op_stack, add_precedence, precedence_option);
        op_stack.push('+');
        line = ConsumeWhitespace(line.substr(1));
        break;
      }
      case '*': {
        int mul_precedence = GetPrecedence('*', precedence_option);
        DischargeOps(expr_stack, op_stack, mul_precedence, precedence_option);
        op_stack.push('*');
        line = ConsumeWhitespace(line.substr(1));
        break;
      }
      default: {
        auto first_non_digit = line.find_first_not_of("0123456789");
        if (first_non_digit == string_view::npos) first_non_digit = line.size();
        expr_stack.push(Num(stoll(string(line.substr(0, first_non_digit)))));
        line = ConsumeWhitespace(line.substr(first_non_digit));
      }
    }
  }
  DischargeOps(expr_stack, op_stack, /*precedence=*/1, precedence_option);
  return move(expr_stack.top());
}

void day18(istream& in, ostream& out) {
  int64_t sum_equal = 0;
  int64_t sum_add_before_mul = 0;
  string line;
  while (getline(in, line) && !line.empty()) {
    sum_equal += Evaluate(*ParseExpression(line, PrecedenceOption::kEqual));
    sum_add_before_mul +=
        Evaluate(*ParseExpression(line, PrecedenceOption::kAddBeforeMul));
  }
  out << "Part 1: " << sum_equal << endl;
  out << "Part 2: " << sum_add_before_mul << endl;
}
