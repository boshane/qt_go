#include "draw.h"
#include "boardlogic.h"
#include "math.h"
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
        : QWidget(parent), gameData(gameData) {
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}


void RenderBoard::paintEvent(QPaintEvent *event)
{
    int boardSize = this->gameData.boardHeightWidth;
    int spacing = 600 / boardSize;

    QPainter painter(this);
    paintTable(&painter);

    for (auto &i : gameData.fields->data)
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
    int boardhw = gameData.boardHeightWidth;
    int totalFields = this->gameData.numberOfFields();
    QPoint fieldTopLeft = gameData.firstField().pixelPosition();
    QPoint fieldBottomRight = gameData.lastField().pixelPosition();

    QRect board = QRect(fieldTopLeft, fieldBottomRight);
    setMouseTracking(true);

    painter->setBrush(QColor::fromRgb(217, 177, 140, 255));
    painter->drawRect(board);
    painter->setPen(QPen(Qt::black, 2));

    for (int i = 1; i < boardhw - 1; i++) {

        Matrix<Field>::Row currentRow = gameData.fields->row(i);
        Matrix<Field>::Col currentCol = gameData.fields->col(i);

        Field from = *currentRow.rowvec.front();
        Field to = *currentRow.rowvec.back();
        painter->drawLine(from.pixelPosition(), to.pixelPosition());

        from = *currentCol.colvec.front();
        to = *currentCol.colvec.back();
        painter->drawLine(from.pixelPosition(), to.pixelPosition());
    }

    if (mouseOverField != -1)
    {
        painter->setPen(QPen(Qt::green, 1));
        QColor white20 = Qt::white;
        white20.setAlphaF(0.8);
        painter->setBrush(white20);
        painter->drawEllipse(gameData.fields->data[mouseOverField].pixelPosition(), 15, 15);
    }
}

void RenderBoard::mouseMoveEvent(QMouseEvent *event)
{
    QPoint mousePos = event->pos();

    for (auto &i : gameData.fields->data) {
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
        auto& field = gameData.fields->data[mouseOverField];

        if (gameData.placeStone(field))
        {
            emit appendStatus(field);
            std::vector<Field> final;
            if (gameData.isGroupAdjacent(field, final))
            {
                emit doDataText(final);
            }
        }
        repaint();
    }
}
