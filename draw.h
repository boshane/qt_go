#ifndef QT_GO_DRAW_H
#define QT_GO_DRAW_H

#include "boardlogic.h"

using namespace std;

class RenderBoard : public QWidget
{
    Q_OBJECT

protected:
    void paintEvent(QPaintEvent* event) override;

public:
    explicit RenderBoard(GameData &gameData, QWidget *parent);
    void paintTable(QPainter* painter);
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

    int mouseOverField = -1;

private:
    int _size;
    GameData& _gameData;
};

#endif //QT_GO_DRAW_H
