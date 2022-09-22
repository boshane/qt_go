#include "boardlogic.h"

bool higherRow(Field f0, Field f1)
{
    return (f0.coords().y() < f1.coords().y());
}

std::vector<Field> GameData::adjacentStones(Field& field)
{
    std::vector<Field> adj;

    for (auto &i: modifiers) {
        int adjx = field.coords().x() + i.first;
        int adjy = field.coords().y() + i.second;

        Field current = getField(adjx, adjy);

        if (current.getPlayer() == opposingPlayer())
        {
            adj.push_back(current);
        }
    }
    return adj;
}

bool GameData::fieldContainsOpponent(Field& field) const
{
    if (field.getPlayer() != _currentPlayer && field.getPlayer() != EMPTY)
    {
        if (field.coords().x() < boardHeightWidth && field.coords().y() < boardHeightWidth)
        {
            return true;
        }
    }
    return false;
}

void GameData::findGroup(vector<Field>& final, vector<Field>& pending)
{
    Field target = pending.back();
    pending.pop_back();

    for (auto& i : modifiers)
    {
        int adjx = target.coords().x() + i.first;
        int adjy = target.coords().y() + i.second;
        Field cur = getField(adjx, adjy);

        if (fieldContainsOpponent(cur))
        {
            if (!(std::find(final.begin(), final.end(), cur) != final.end()))
            {
                pending.push_back(cur);
                final.push_back(cur);
            }
        }
    }
    if (!pending.empty())
    {
        findGroup(final, pending);
    }
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
        field.player = this->_currentPlayer;
        doesFieldCapture(field);
    }
}

