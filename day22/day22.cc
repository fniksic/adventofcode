#include "day22.h"

#include <deque>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_set>

using namespace std;

struct HashPairOfDecks {
  inline size_t HashDeck(const deque<int>& deck) const noexcept {
    size_t seed{};
    hash<int> hasher{};
    for (int card : deck) {
      // Taken from boost::hash_range and boost::hash_combine.
      // The magic constant is floor(2^32/phi), where phi is the golden ratio.
      // Keyword: Fibonacci hashing
      seed ^= hasher(card) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
  }

  inline size_t operator()(
      const pair<deque<int>, deque<int>>& decks) const noexcept {
    const auto& [deck_1, deck_2] = decks;
    size_t seed{};
    seed ^= HashDeck(deck_1) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= HashDeck(deck_2) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    return seed;
  }
};

class Game {
 public:
  Game() = default;

  Game(deque<int>::const_iterator begin_1, deque<int>::const_iterator end_1,
       deque<int>::const_iterator begin_2, deque<int>::const_iterator end_2)
      : deck_1(begin_1, end_1), deck_2(begin_2, end_2) {}

  void ReadInput(istream& in) {
    string line{};
    getline(in, line);  // Ignore "Player 1:"
    while (getline(in, line) && !line.empty()) {
      deck_1.push_back(stoi(line));
    }
    getline(in, line);  // Ignore "Player 2:"
    while (getline(in, line) && !line.empty()) {
      deck_2.push_back(stoi(line));
    }
  }

  void PlayPart1() {
    while (!(deck_1.empty() || deck_2.empty())) {
      if (deck_1.front() > deck_2.front()) {
        deck_1.push_back(deck_1.front());
        deck_1.push_back(deck_2.front());
      } else {
        deck_2.push_back(deck_2.front());
        deck_2.push_back(deck_1.front());
      }
      deck_1.pop_front();
      deck_2.pop_front();
    }
  }

  // Returns the index (1 or 2) of the player who wins the game.
  int PlayPart2() {
    unordered_set<pair<deque<int>, deque<int>>, HashPairOfDecks> seen{};
    while (!(deck_1.empty() || deck_2.empty())) {
      if (seen.find(make_pair(deck_1, deck_2)) != seen.end()) {
        return 1;
      }
      seen.insert(make_pair(deck_1, deck_2));
      int round_winner;
      if (deck_1.front() < deck_1.size() && deck_2.front() < deck_2.size()) {
        Game sub_game{deck_1.begin() + 1, deck_1.begin() + deck_1.front() + 1,
                      deck_2.begin() + 1, deck_2.begin() + deck_2.front() + 1};
        round_winner = sub_game.PlayPart2();
      } else {
        round_winner = deck_1.front() > deck_2.front() ? 1 : 2;
      }
      if (round_winner == 1) {
        deck_1.push_back(deck_1.front());
        deck_1.push_back(deck_2.front());
      } else {
        deck_2.push_back(deck_2.front());
        deck_2.push_back(deck_1.front());
      }
      deck_1.pop_front();
      deck_2.pop_front();
    }
    return deck_1.empty() ? 2 : 1;
  }

  int GetScore() const {
    const deque<int>& deck = deck_1.empty() ? deck_2 : deck_1;
    int score{};
    int factor = deck.size();
    for (int card : deck) {
      score += card * factor;
      --factor;
    }
    return score;
  }

 private:
  deque<int> deck_1;
  deque<int> deck_2;
};

void day22(istream& in, ostream& out) {
  Game game_1{};
  game_1.ReadInput(in);
  Game game_2{game_1};
  game_1.PlayPart1();
  out << "Part 1: " << game_1.GetScore() << endl;
  game_2.PlayPart2();
  out << "Part 2: " << game_2.GetScore() << endl;
}
