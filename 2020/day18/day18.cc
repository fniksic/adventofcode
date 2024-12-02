#include "day18.h"

#include <cstdint>
#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <string_view>
#include <utility>
#include <variant>

using namespace std;

struct Expression;

using ExpressionPtr = unique_ptr<Expression>;

struct NumExpr {
  int64_t value;
};

struct AddExpr {
  ExpressionPtr left;
  ExpressionPtr right;
};

struct MulExpr {
  ExpressionPtr left;
  ExpressionPtr right;
};

template <typename... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};

template <typename... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

struct Expression : public variant<NumExpr, AddExpr, MulExpr> {
  using variant<NumExpr, AddExpr, MulExpr>::variant;

  template <typename CaseNum, typename CaseAdd, typename CaseMul>
  auto match(CaseNum case_num, CaseAdd case_add, CaseMul case_mul) {
    return visit(overloaded{case_num, case_add, case_mul},
                 *static_cast<variant<NumExpr, AddExpr, MulExpr>*>(this));
  }
};

ExpressionPtr Num(int64_t value) {
  return make_unique<Expression>(NumExpr{value});
}

ExpressionPtr Add(ExpressionPtr left, ExpressionPtr right) {
  return make_unique<Expression>(AddExpr{move(left), move(right)});
}

ExpressionPtr Mul(ExpressionPtr left, ExpressionPtr right) {
  return make_unique<Expression>(MulExpr{move(left), move(right)});
}

int64_t Evaluate(Expression& expr) {
  return expr.match(
      [](const NumExpr& num_expr) { return num_expr.value; },
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

// Discharges ops with precedence >= `precedence`.
void DischargeOps(stack<ExpressionPtr>& expr_stack, stack<char>& op_stack,
                  int precedence, PrecedenceOption precedence_option) {
  while (!op_stack.empty() &&
         GetPrecedence(op_stack.top(), precedence_option) >= precedence) {
    ExpressionPtr rhs = move(expr_stack.top());
    expr_stack.pop();
    ExpressionPtr lhs = move(expr_stack.top());
    expr_stack.pop();
    auto op = op_stack.top() == '+' ? Add : Mul;
    op_stack.pop();
    expr_stack.push(op(move(lhs), move(rhs)));
  }
}

// Assumes `line` is a well-formed arithmetic expression.
ExpressionPtr ParseExpression(string_view line,
                              PrecedenceOption precedence_option) {
  stack<ExpressionPtr> expr_stack;
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
