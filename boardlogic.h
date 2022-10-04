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
#include "math.h"

class Field {

public:

    Field() : boardPos(QPoint(0, 0)), player{EMPTY}, id{0} {}
    Field(int x, int y, int boardSize) : boardPos{QPoint(x, y)}, player{EMPTY}, id{(y * boardSize) + x}
    {
        int spacing = 900 / boardSize;
        centerPixel = QPoint((x * spacing) + 20, (y * spacing) + 20);
    }
    QPoint pixelPosition() { return centerPixel; }
    QPoint coords() { return boardPos; }
    int xScreenPos() { return centerPixel.x(); }
    int yScreenPos() { return centerPixel.y(); }
    int x() const { return boardPos.x(); }
    int y() const { return boardPos.y(); }
    Player& getPlayer() { return player; }
    void setPlayer(Player newPlayer) { player = newPlayer; }
    bool isEmpty() { if (this->getPlayer() == EMPTY) return true; else return false; }
    friend bool operator==(const Field& lhs, const Field& rhs)
    {
        if (lhs.boardPos.x() == rhs.boardPos.x() && lhs.boardPos.y() == rhs.boardPos.y())
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
    operator int() { return static_cast<int>(getPlayer()); }

    int id;
    Player player;

private:
    QPoint boardPos;
    QPoint centerPixel;
};


class GameData
{
public:

    explicit GameData() : boardHeightWidth{10}, currentPlayer{Black} { initMatrix(); };
    explicit GameData(int hw) : boardHeightWidth{hw}, currentPlayer{Black} { initMatrix(); };

    void initMatrix()
    {
        fields = new Matrix<Field>(boardHeightWidth, boardHeightWidth);
        for (int i = 0; i < boardHeightWidth; i++)
        {
            for (int j = 0; j < boardHeightWidth; j++)
            {
                fields->get(j, i) = Field {j, i, boardHeightWidth};
            }
        }
    }
    size_t totalFields() const { return fields->size(); };
    Field& getField(int x, int y) { return fields->get(x, y); }
    void findGroup(std::vector<Field>& final, std::vector<Field>& pending);
    std::vector<Field>& isGroupAdjacent(Field& field, std::vector<Field>& final);
    bool isGroupEnclosed(std::vector<Field> final);
    bool isRowSymmetrical(std::vector<int> columns, Matrix<Field>::Row target);
    void removeStones(std::vector<Field>& final) const;
    bool fieldContainsOpponent(Field& field) const;
    bool placeStone(Field& field);
    Player opposingPlayer() { return (currentPlayer == Black) ? White : Black; }
    Player opposingPlayer(Field& field) { return (field.getPlayer() == Black) ? White : Black; }
    int numberOfFields() { return (boardHeightWidth * boardHeightWidth) - 1; }
    Field& firstField() { return fields->data.front(); }
    Field& lastField() { return fields->data.back(); }

    int boardHeightWidth;
    Player currentPlayer;
    Matrix<Field>* fields;
private:
};


#endif //QT_GO_BOARDLOGIC_H
