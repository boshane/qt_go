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
#include <sstream>
#include "game.h"

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

class Field {
public:

    Field() : _coords(QPoint(0, 0)), player{EMPTY}, id{0} {}
    Field(int x, int y, int id) : _coords{QPoint(x, y)}, player{EMPTY}, id{id}
    {
        _center = QPoint((x + 1) * 40, (y + 1) * 40);
    }
    QPoint pixelPosition() { return this->_center; }
    QPoint coords() { return this->_coords; }
    int xScreenPos() { return _center.x(); }
    int yScreenPos() { return _center.y(); }
    int x() const { return _coords.x(); }
    int y() const { return _coords.y(); }
    Player getPlayer() { return player; }
    bool isEmpty() { if (this->getPlayer() == EMPTY) return true; else return false; }
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
            os << pi->second << " at coordinates: " << field.x() << ", " << field.y() << '\n';
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
    void findGroup(std::vector<Field>& final, std::vector<Field>& pending);
    void doesFieldCapture(Field& field);
    bool fieldContainsOpponent(Field& field) const;
    std::vector<Field> adjacentStones(Field& field);
    void placeStone(Field& field);
    Player opposingPlayer() { return (_currentPlayer == Black) ? White : Black; }
    Player opposingPlayer(Field& field) { return (field.getPlayer() == Black) ? White : Black; }

    int boardHeightWidth;
    Player _currentPlayer;

private:

    vector<Field> _fields;
};


#endif //QT_GO_BOARDLOGIC_H
