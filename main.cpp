#include <cstdlib>
#include "draw.h"
#include "boardlogic.h"
#include "math.h"

class GameData;

const int IdRole = Qt::UserRole;

class GameWindow : public QWidget
{
Q_OBJECT

public:
    GameWindow();

private slots:
    void playerChanged();

private:
    void initWindow();
    RenderBoard *renderBoard;
    GameData *gameData;
    QLabel* playerOptionLabel;
    QComboBox *playerSelectorBox;
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

    playerSelectorBox = new QComboBox;
    playerSelectorBox->addItem(tr("Black"), Black);
    playerSelectorBox->addItem(tr("White"), White);
    playerOptionLabel = new QLabel(tr("Select player"));

    auto *mainLayout = new QGridLayout;
//    this->setFixedSize(600, 600);
    mainLayout->setSizeConstraint(QLayout::SetMaximumSize);
    mainLayout->addWidget(renderBoard, 0, 0, 1, 2);
    mainLayout->addWidget(playerOptionLabel, 1, 3);
    mainLayout->addWidget(playerSelectorBox, 1, 4);
    setLayout(mainLayout);

    playerChanged();

    setWindowTitle(tr("GO game"));
}

void GameWindow::playerChanged()
{
    Player p = Player(playerSelectorBox->itemData(playerSelectorBox->currentIndex(), IdRole).toInt());
    renderBoard->setPlayer(p);
}

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    GameWindow gameWindow;
    gameWindow.show();
    return app.exec();
}

#include "main.moc"