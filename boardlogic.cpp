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
void GameData::findGroup(std::vector<Field>& final, std::vector<Field>& pending)
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
}

// Does the target row have players in the columns?
bool GameData::isRowSymmetrical(std::vector<int> columns, Matrix<Field>::Row target)
{
    for (int j : columns)
    {
        if (target[j].getPlayer() == EMPTY)
        {
            std::cout << target[j];
            return false;
        }
    }
    return true;
}

// Is the group vector final enclosed by the attacking player?
bool GameData::isGroupEnclosed(std::vector<Field> final)
{
    std::vector<int> xfields;
    std::vector<Field> pending = final;
    Player attacker = opposingPlayer(pending.front());
    int nrow;
    int firstrow = final.front().y();
    int lastrow = final.back().y();

    // iterate through the y-value sorted vector 'final'
    do
    {
        nrow = pending.back().y();
        xfields.clear();

        // iterate through each x-value per row and add to a vector
        do
        {
            xfields.push_back(pending.back().x());
            pending.pop_back();
        } while ((pending.begin() != pending.end()) && pending.back().y() == nrow);

        sort(xfields.begin(), xfields.end());

        if ((firstrow - 1 >= 0) && firstrow == nrow)
        {
            if (!isRowSymmetrical(xfields, fields->row(firstrow - 1)))
            {
                return false;
            }
        }

        if ((lastrow + 1 < fields->nRows()) && lastrow == nrow)
        {
            if (!isRowSymmetrical(xfields, fields->row(lastrow + 1)))
            {
                return false;
            }
        }

        // if the last x-value minus the size of the total x-values in xfields is not equal to the first x-value,
        // then it means there is an empty space in that row, and cannot be captured.
        // TODO: follow up with the GO board rules for 'eyes'

        if (((xfields.back() - (int)xfields.size()) + 1) == *xfields.begin())
        {
            int xmin = xfields.front() - 1;
            int xmax = xfields.back() + 1;
            Player min = fields->get(xmin, nrow).getPlayer();
            Player max = fields->get(xmax, nrow).getPlayer();

            // Check for xfields exceeding the column width or height. Find a more elegant way to do this.
            if (xmin < 0) {
                if ((max == EMPTY) || (max != attacker))
                    return false;
            }
            else if (xmax >= fields->nRows()) {
                if ((min == EMPTY) || (min != attacker))
                    return false;
            }
            else if (((min == EMPTY || max == EMPTY) ||
                      (min != attacker || max != attacker)))
            {
                return false;
            }
        }
        else return false;

        std::cout << std::endl;
    } while (pending.begin() != pending.end());

    std::cout << "Group is enclosed.";
    return true;
}

void GameData::removeStones(std::vector<Field>& stones) const
{
    for (auto i : stones)
    {
        fields->get(i.x(), i.y()).setPlayer(EMPTY);
    }
}

std::vector<Field>& GameData::isGroupAdjacent(Field& field, std::vector<Field>& final)
{
    std::vector<Field> pending;

    pending.push_back(field);

    findGroup(final, pending);
    std::sort(final.begin(), final.end(), higherRow);

    return final;
}

bool GameData::placeStone(Field& field)
{
    if (field.isEmpty())
    {
        field.player = this->currentPlayer;
        return true;
    }
    return false;
}