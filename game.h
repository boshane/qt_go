#ifndef QT_GO_GAME_H
#define QT_GO_GAME_H

#include <string>
#include <set>
#include <unordered_map>

enum Player {
    Black, White, EMPTY
};

const std::unordered_map<Player, std::string> player_map {
        { Black, "Player Black" },
        { White, "Player White" },
        { EMPTY, "No player" }
};

#endif //QT_GO_GAME_H
