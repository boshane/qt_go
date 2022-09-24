#ifndef QT_GO_DRAW_H
#define QT_GO_DRAW_H

#include <QLabel>
#include <QComboBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QButtonGroup>
#include <Qt>
#include <QTextEdit>
#include <QPushButton>

#include "boardlogic.h"

using namespace std;

class RenderBoard : public QWidget
{
    Q_OBJECT

protected:
    void paintEvent(QPaintEvent* event) override;

signals:
    void appendStatus(Field field);

public:
    explicit RenderBoard(GameData &gameData, QWidget *parent);
    void paintTable(QPainter* painter);
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    int mouseOverField = -1;
private:
    int _size;
    GameData& _gameData;
};

#endif //QT_GO_DRAW_H
