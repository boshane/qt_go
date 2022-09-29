#include "boardlogic.h"

bool higherRow(Field f0, Field f1)
{
    return (f0.coords().y() < f1.coords().y());
}

bool GameData::fieldContainsOpponent(Field& field) const
{
    if (field.getPlayer() != currentPlayer && field.getPlayer() != EMPTY)
    {
        if (field.coords().x() < boardHeightWidth && field.coords().y() < boardHeightWidth)
        {
            return true;
        }
    }
    return false;
}

// If a placed stone has an adjacent opponent stone or stones in pending vector, add those stones to the final vector,
// and recursively call the function to search for unfound pending stones.
void GameData::findGroup(vector<Field>& final, vector<Field>& pending)
{
    Field target = pending.back();
    pending.pop_back();

    for (auto& i : modifiers)
    {
        int adjx = target.coords().x() + i.first;
        int adjy = target.coords().y() + i.second;

        if ((adjx < boardHeightWidth && adjx >= 0) &&
            (adjy < boardHeightWidth && adjy >= 0))
        {
            Field cur = getField(adjx, adjy);

            auto const pi = player_map.find(cur.getPlayer());
            if (pi != player_map.end()) {
                std::cout << pi->second << " @ " << cur.coords().x() << " " << cur.coords().y() << '\n';
            }

            if (fieldContainsOpponent(cur)) {
                if (!(std::find(final.begin(), final.end(), cur) != final.end())) {
                    pending.push_back(cur);
                    final.push_back(cur);
                }
            }
        }
    }
    if (!pending.empty())
    {
        findGroup(final, pending);
    }
    std::cout << '\n';
}

void GameData::doesFieldCapture(Field& field)
{
    std::vector<Field> final;
    std::vector<Field> pending;

    pending.push_back(field);

    findGroup(final, pending);
    std::sort(final.begin(), final.end(), higherRow);

    Player attacker = opposingPlayer(field);

    for (auto& i : final)
    {
        cout << i;
    }
}

void GameData::placeStone(Field& field)
{
    if (field.isEmpty())
    {
        field.player = this->currentPlayer;
        doesFieldCapture(field);
    }
}

