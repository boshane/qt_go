#include "boardlogic.h"

bool higherRow(Field f0, Field f1)
{
    return (f0.coords().y() < f1.coords().y());
}

std::vector<Field> adjacentStones(Field& field)
{

}

void GameData::findGroup(std::set<Field, not_equal_to<>>& adjacent, std::vector<Field> pending)
{
    if (!pending.empty()) {
        Field field = pending.back();
        pending.pop_back();

        for (auto &i: modifiers) {
            int adjx = field.coords().x() + i.first;
            int adjy = field.coords().y() + i.second;

            Player p = getField(adjx, adjy).getPlayer();

            if (p != field.getPlayer() && p != EMPTY) {
                Field cur = getField(adjx, adjy);

                adjacent.insert(cur);

                if (std::find(pending.begin(), pending.end(), cur) != pending.end()) {
                    pending.push_back(cur);
                }
            }
        }
        findGroup(adjacent, pending);
    }
}

void GameData::placeStone(Field& field)
{
    if (field.isEmpty())
    {
        field.player = this->_currentPlayer;

        if (this->_currentPlayer == Black)
        {
            this->_currentPlayer = White;
        }
        else
        {
            this->_currentPlayer = Black;
        }
        std::set<Field, not_equal_to<>> adjacent;
        std::vector<Field> pending;
        pending.push_back(field);

        findGroup(adjacent, pending);

        // FIXME: Figure out how to use vector from the beginning to avoid this conversion
        std::vector<Field> final(adjacent.begin(), adjacent.end());
        std::sort(final.begin(), final.end(), higherRow);

        for (auto& i : final)
        {
            std::cout << i;
        }
    }
}

