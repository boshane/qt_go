#include "draw.h"
#include "boardlogic.h"
#include <sstream>

QSize RenderBoard::sizeHint() const
{
    return QSize(500, 500);
}

QSize RenderBoard::minimumSizeHint() const
{
    return QSize(500, 500);
}

RenderBoard::RenderBoard(GameData &gameData, QWidget *parent)
        : QWidget(parent), _gameData(gameData) {
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}


void RenderBoard::paintEvent(QPaintEvent *event)
{
    int boardSize = this->_gameData.boardHeightWidth;
    int spacing = 600 / boardSize;

    QPainter painter(this);
    paintTable(&painter);

    for (auto &i : _gameData.Fields())
    {
        Player currentPlayer = i.getPlayer();
        painter.setPen(QPen(Qt::black, 1));

        switch (currentPlayer)
        {
            case Black:
                painter.setBrush(Qt::black);
                painter.drawEllipse(i.pixelPosition(), 20, 20);
                break;
            case White:
                painter.setBrush(Qt::white);
                painter.drawEllipse(i.pixelPosition(), 20, 20);
                break;
            case EMPTY:
                break;
        }
    }
}

void RenderBoard::paintTable(QPainter* painter)
{
    int boardhw = _gameData.boardHeightWidth;
    int totalFields = this->_gameData.Fields().size();
    QPoint boardTopLeft = _gameData.Fields().front().pixelPosition();
    QPoint boardBottomRight = _gameData.Fields().back().pixelPosition();
    QRect board = QRect(boardTopLeft, boardBottomRight);
    setMouseTracking(true);

    painter->setBrush(QColor::fromRgb(217, 177, 140, 255));
    painter->drawRect(board);
    painter->setPen(QPen(Qt::black, 2));

    for (int i = 1; i < boardhw - 1; i++)
    {
        int cur = i * boardhw;

        QPoint from = _gameData.Fields()[cur].pixelPosition();
        QPoint to = _gameData.Fields()[(cur + boardhw) - 1].pixelPosition();
        painter->drawLine(from, to);

        cur = _gameData.totalFields() - (boardhw - i);

        from = _gameData.Fields()[i].pixelPosition();
        to = _gameData.Fields()[cur].pixelPosition();
        painter->drawLine(from, to);
    }

    if (mouseOverField != -1)
    {
        painter->setPen(QPen(Qt::green, 1));
        QColor white20 = Qt::white;
        white20.setAlphaF( 0.8 );
        painter->setBrush(white20);
        painter->drawEllipse(_gameData.Fields()[mouseOverField].pixelPosition(), 15, 15);
    }
}

void RenderBoard::mouseMoveEvent(QMouseEvent *event)
{
    QPoint mousePos = event->pos();

    for (auto &i : _gameData.Fields()) {
        if ((mousePos.x() > (i.pixelPosition().x() - 20)) && (mousePos.x() < (i.pixelPosition().x() + 20)) &&
            (mousePos.y() > (i.pixelPosition().y() - 20)) && (mousePos.y() < (i.pixelPosition().y() + 20))) {
            if (mouseOverField == i.id)
            {
                return;
            }
            else if (mouseOverField == -1 || mouseOverField != i.id)
            {
                mouseOverField = i.id;
                repaint();
                return;
            }
        }
    }
    if (mouseOverField != -1)
    {
        mouseOverField = -1;
        repaint();
        return;
    }
}

void RenderBoard::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && mouseOverField != -1)
    {
        auto& field = _gameData.getField(mouseOverField);
        _gameData.placeStone(field);

        emit appendStatus(field);

        repaint();
    }
}
