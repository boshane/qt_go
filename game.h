#ifndef QT_GO_GAME_H
#define QT_GO_GAME_H

#include <string>
#include <set>
#include <unordered_map>

enum Player {
    Black, White, EMPTY
};

const std::unordered_map<Player, std::string> player_map {
        { Black, "Black" },
        { White, "White" },
        { EMPTY, "Empty" }
};

static std::vector<std::pair<int, int>> modifiers = {
        {-1, -1},
        {0, -1},
        {1, 1},
        {-1, 0},
        {1, 0},
        {1, -1},
        {0, 1},
        {-1, 1}
};

#endif //QT_GO_GAME_H
