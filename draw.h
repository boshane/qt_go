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

class RenderBoard : public QWidget
{
    Q_OBJECT

protected:
    void paintEvent(QPaintEvent* event) override;

signals:
    void appendStatus(Field field);
    void doDataText(std::vector<Field> group);

public:
    explicit RenderBoard(GameData &gameData, QWidget *parent);
    void newBoard(GameData &gd) { gameData = gd; }
    void paintTable(QPainter* painter);
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    int mouseOverField = -1;
private:
    int size;
    GameData& gameData;
};

#endif //QT_GO_DRAW_H
