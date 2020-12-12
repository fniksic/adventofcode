#include <cmath>
#include <array>
#include <iostream>
#include <string>
#include <utility>

using namespace std;

struct Vec { int x; int y; };

using Rot = pair<Vec, Vec>;

constexpr array kRotations = {Rot{{1, 0}, {0, 1}},
                              Rot{{0, -1}, {1, 0}},
                              Rot{{-1, 0}, {0, -1}},
                              Rot{{0, 1}, {-1, 0}}};

int main() {
  Vec position{0, 0};
  Vec direction{1, 0};

  Vec wp_position{0, 0};
  Vec waypoint{10, 1};

  string command;
  while (cin >> command) {
    char action = command[0];
    int value = stoi(command.substr(1));

    if (action == 'R') {
      action = 'L';
      value = 360 - value;
    }

    switch (action) {
      case 'N': {
        position.y += value;
        waypoint.y += value;
        break;
      }
      case 'S': {
        position.y -= value;
        waypoint.y -= value;
        break;
      }
      case 'E': {
        position.x += value;
        waypoint.x += value;
        break;
      }
      case 'W': {
        position.x -= value;
        waypoint.x -= value;
        break;
      }
      case 'L': {
        auto[a, b] = kRotations[value / 90];

        Vec temp = direction;
        direction.x = a.x * temp.x + a.y * temp.y;
        direction.y = b.x * temp.x + b.y * temp.y;

        temp = waypoint;
        waypoint.x = a.x * temp.x + a.y * temp.y;
        waypoint.y = b.x * temp.x + b.y * temp.y;
        break;
      }
      case 'F':
      default: {
        position.x += direction.x * value;
        position.y += direction.y * value;
        wp_position.x += waypoint.x * value;
        wp_position.y += waypoint.y * value;
        break;
      }
    }
  }

  cout << "Part 1: " << abs(position.x) + abs(position.y) << endl;
  cout << "Part 2: " << abs(wp_position.x) + abs(wp_position.y) << endl;

  return 0;
}