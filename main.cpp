#include "draw.h"
#include "math.h"
#include <cstdlib>

class GameWindow : public QWidget
{
Q_OBJECT

public:
    GameWindow();

private:
    void initWindow();
    RenderBoard *renderBoard;
    GameData *gameData;
};

GameWindow::GameWindow()
{
    initWindow();
}

void GameWindow::initWindow()
{
    int boardSize = 10;
    gameData = new GameData(boardSize);
    renderBoard = new RenderBoard(*gameData, nullptr);

    auto *mainLayout = new QGridLayout;
    this->setFixedSize(600, 600);
    mainLayout->setSizeConstraint(QLayout::SetMaximumSize);
    mainLayout->addWidget(renderBoard, 0, 0, 1, 4);
    setLayout(mainLayout);

    setWindowTitle(tr("GO game"));
}

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    GameWindow gameWindow;
    gameWindow.show();
    return app.exec();
}

#include "main.moc"