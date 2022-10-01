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

// Is the group vector final enclosed by the attacking player?
bool GameData::isGroupEnclosed(std::vector<Field> &final)
{
    std::vector<int> xfields;
    Player attacker = opposingPlayer(final.front());

    // iterate through the y-value sorted vector 'final'
    do
    {
        int nrow = final.back().y();
        xfields.clear();

        // iterate through each x-value per row and add to an vector
        do
        {
            xfields.push_back(final.back().x());
            final.pop_back();
        } while (final.back().y() == nrow);

        sort(xfields.begin(), xfields.end());

        // if the last x-value minus the size of the total x-values in xfields is not equal to the first x-value,
        // then it means there is an empty space in that row, and cannot be captured.
        // TODO: follow up with the GO board rules for 'eyes'
        if (((xfields.back() - (int)xfields.size()) + 1) == *xfields.begin())
        {
            int xmin = xfields.front() - 1;
            int xmax = xfields.back() + 1;
            Player min = fields->get(xmin, nrow).getPlayer();
            Player max = fields->get(xmax, nrow).getPlayer();

            if ((min == EMPTY || max == EMPTY) ||
                (min != attacker || max != attacker))
            {
                return false;
            }

            // is the top row of xvalues covered by attackers?
            if (nrow == final.front().y())
            {
                for (int i : xfields)
                {
                    if (fields->get(i, final.front().y() - 1).getPlayer() == EMPTY)
                    {
                        return false;
                    }
                }
            }
            // is the bottom row of xvalues covered by attackers?
            if (nrow == final.back().y())
            {
                for (int i : xfields)
                {
                    if (fields->get(i, final.front().y() + 1).getPlayer() == EMPTY)
                    {
                        return false;
                    }
                }
            }

        }
        else return false;

        std::cout << std::endl;
    } while (final.begin() != final.end());

    std::cout << "Group is enclosed.";
    return true;
}

bool GameData::isGroupAdjacent(Field& field, std::vector<Field>& final)
{
    std::vector<Field> pending;

    pending.push_back(field);

    findGroup(final, pending);
    std::sort(final.begin(), final.end(), higherRow);

    if (final.size() > 0)
    {
        isGroupEnclosed(final);
        return true;
    }
    else return false;
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