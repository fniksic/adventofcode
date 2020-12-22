#include "day22.h"

#include <algorithm>
#include <iostream>
#include <queue>
#include <string>

using namespace std;

class Game {
 public:
  void ReadInput(istream& in) {
    string line;
    getline(in, line);  // Ignore "Player 1:"
    while (getline(in, line) && !line.empty()) {
      deck_1.push(stoi(line));
    }
    getline(in, line);  // Ignore "Player 2:"
    while (getline(in, line) && !line.empty()) {
      deck_2.push(stoi(line));
    }
  }

  void Play() {
    while (!(deck_1.empty() || deck_2.empty())) {
      if (deck_1.front() > deck_2.front()) {
        deck_1.push(deck_1.front());
        deck_1.push(deck_2.front());
      } else {
        deck_2.push(deck_2.front());
        deck_2.push(deck_1.front());
      }
      deck_1.pop();
      deck_2.pop();
    }
  }

  int GetScore() const {
    // Make a copy
    queue<int> deck = deck_1.empty() ? deck_2 : deck_1;
    int score = 0;
    for (int factor = deck.size(); !deck.empty(); --factor, deck.pop()) {
      score += deck.front() * factor;
    }
    return score;
  }

 private:
  queue<int> deck_1;
  queue<int> deck_2;
};

void day22(istream& in, ostream& out) {
  Game game;
  game.ReadInput(in);
  game.Play();
  out << "Part 1: " << game.GetScore() << endl;
}
