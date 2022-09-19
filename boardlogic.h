//
// Created by dbeau on 9/12/2022.
//

#ifndef QT_GO_BOARDLOGIC_H
#define QT_GO_BOARDLOGIC_H

#include <QApplication>
#include <QPushButton>
#include <QPainter>
#include <QGridLayout>
#include <QColor>
#include <QMouseEvent>
#include <iostream>
#include <set>


using namespace std;

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

enum Player {
    Black, White, EMPTY
};

const unordered_map<Player, string> player_map {
        { Black, "Player Black" },
        { White, "Player White" },
        { EMPTY, "No player" }
};

class Field {
public:
    Field() : _coords(QPoint(0, 0)), player{EMPTY}, id{0} {}
    Field(int x, int y, int id) : _coords{QPoint(x, y)}, player{EMPTY}, id{id}
    {
        _center = QPoint((x + 1) * 50, (y + 1) * 50);
    }
    QPoint pixelPosition() { return this->_center; }
    QPoint coords() { return this->_coords; }
    void setPlayer(Player p) { this->player = p; }
    Player getPlayer() { return player; }
    int x() { return this->_center.x(); }
    int y() { return this->_center.y(); }
    void setX(int x) { this->_center.setY(x); }
    void setY(int y) { this->_center.setY(y); }
    bool isEmpty() { if (this->getPlayer() == EMPTY) return true; else return false; }
    void setCoords(int x, int y) { this->_center.setY(y); this->_center.setX(x); }
    friend bool operator==(const Field& lhs, const Field& rhs)
    {
        if (lhs._coords.x() == rhs._coords.x() && lhs._coords.y() == rhs._coords.y())
        {
            return true;
        }
        else
            return false;
    }
    friend std::ostream& operator<<(std::ostream& os, const Field& field)
    {
        auto const pi = player_map.find(field.player);
        if (pi != player_map.end())
        {
            os << pi->second << " at coordinates: " << field._coords.x() << ", " << field._coords.y() << '\n';
        }

        return os;
    }

    int id;
    Player player;


private:

    QPoint _coords;
    QPoint _center;
};


class GameData
{
public:

    explicit GameData(int hw) : boardHeightWidth{hw}, _currentPlayer{Black} { populate_fields(); };

    void populate_fields()
    {
        for (int i = 0; i < boardHeightWidth; i++)
        {
            for (int j = 0; j < boardHeightWidth; j++)
            {
                this->_fields.emplace_back(j, i, (i * boardHeightWidth) + j);
            }
        }
    }
    size_t totalFields() const { return this->_fields.size(); };
    Field& getField(int id) { return _fields[id]; }
    Field& getField(int x, int y) { return _fields[(y * boardHeightWidth) + x]; };
    vector<Field> Fields() { return this->_fields; };
    void findGroup(std::set<Field, not_equal_to<>>& adjacent, std::vector<Field> pending);
    std::vector<Field> adjacentStones(Field& field);
    void placeStone(Field& field);


    int boardHeightWidth;
    Player _currentPlayer;
private:

    vector<Field> _fields;
};


#endif //QT_GO_BOARDLOGIC_H
