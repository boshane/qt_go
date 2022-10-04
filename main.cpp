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
    void radioSetWhite(bool checked);
    void radioSetBlack(bool checked);
    void radioSetTen(bool checked);
    void radioSetTwenty(bool checked);
    void appendStatusText(Field);
    void doDataText(std::vector<Field> group);
    void newGame();

private:
    void initWindow();
    RenderBoard *renderBoard;
    GameData *gameData;
    QLabel *playerOptionLabel; // player selection group
    QGroupBox *playerSelect;
    QRadioButton *radioBlack;
    QRadioButton *radioWhite;
    QButtonGroup *playerButtons;
    QLabel *boardOptionLabel;
    QGroupBox *sizeSelect; // board size selection group
    QRadioButton *radioTen;
    QRadioButton *radioTwenty;
    QButtonGroup *sizeButtons;
    QTextEdit *statusText;
    QTextEdit *dataText;
    QScrollBar *statusScrollBar; // TODO: Complete the scrollbar moving to the end
    QPushButton *resetGame;
    QVBoxLayout *mainLayout;
};

GameWindow::GameWindow()
{
    initWindow();
}

void GameWindow::newGame()
{
    gameData->initMatrix();
    renderBoard->update();
}

void GameWindow::radioSetBlack(bool checked)
{
    if (checked)
    {
        gameData->currentPlayer = Player::Black;
    }
}

void GameWindow::radioSetWhite(bool checked)
{
    if (checked)
    {
        gameData->currentPlayer = Player::White;
    }
}

void GameWindow::radioSetTen(bool checked)
{
    if (checked)
    {
        gameData->boardHeightWidth = 10;
        gameData->initMatrix();
        renderBoard->update();
    }
}

void GameWindow::radioSetTwenty(bool checked)
{
    if (checked)
    {
        gameData->boardHeightWidth = 20;
        gameData->initMatrix();
        renderBoard->update();
    }
}

void GameWindow::initWindow()
{
    QHBoxLayout* displayLayout = new QHBoxLayout;
    QVBoxLayout* displaySubLayout = new QVBoxLayout;
    QHBoxLayout* controlLayout = new QHBoxLayout;
    QVBoxLayout* controlPlayerLayout = new QVBoxLayout;
    QVBoxLayout* controlBoardLayout = new QVBoxLayout;

    gameData = new GameData(10);
    renderBoard = new RenderBoard(*gameData, this);
    dataText = new QTextEdit("Game data...");
    resetGame = new QPushButton(tr("Reset"));

    statusText = new QTextEdit("Starting game...");
    playerButtons = new QButtonGroup();
    playerSelect = new QGroupBox(tr("Player select"));
    radioBlack = new QRadioButton(tr("Black"));
    radioWhite = new QRadioButton(tr("White"));

    radioBlack->setChecked(true);
    playerButtons->addButton(radioBlack);
    playerButtons->addButton(radioWhite);

    sizeButtons = new QButtonGroup();
    sizeSelect = new QGroupBox(tr("Board size"));
    radioTen = new QRadioButton(tr("Ten"));
    radioTwenty = new QRadioButton(tr("Twenty"));

    radioTen->setChecked(true);
    sizeButtons->addButton(radioTen);
    sizeButtons->addButton(radioTwenty);

    connect(radioBlack, SIGNAL(clicked(bool)), SLOT(radioSetBlack(bool)));
    connect(radioWhite, SIGNAL(clicked(bool)), SLOT(radioSetWhite(bool)));
    connect(radioTen, SIGNAL(clicked(bool)), SLOT(radioSetTen(bool)));
    connect(radioTwenty, SIGNAL(clicked(bool)), SLOT(radioSetTwenty(bool)));
    connect(renderBoard, SIGNAL(appendStatus(Field)), SLOT(appendStatusText(Field)));
    connect(renderBoard, SIGNAL(doDataText(std::vector<Field>)), SLOT(doDataText(std::vector<Field>)));
    connect(resetGame, SIGNAL(clicked(bool)), SLOT(newGame()));

    playerOptionLabel = new QLabel(tr("Select player"));
    boardOptionLabel = new QLabel(tr("Select board size"));

    mainLayout = new QVBoxLayout;
    displayLayout->addWidget(renderBoard);
    displaySubLayout->addWidget(statusText);
    displaySubLayout->addWidget(dataText);
    displayLayout->addLayout(displaySubLayout);

    controlPlayerLayout->addWidget(playerOptionLabel);
    controlPlayerLayout->addWidget(radioBlack);
    controlPlayerLayout->addWidget(radioWhite);
    controlBoardLayout->addWidget(boardOptionLabel);
    controlBoardLayout->addWidget(radioTen);
    controlBoardLayout->addWidget(radioTwenty);
    controlLayout->addWidget(resetGame);
    controlLayout->addLayout(controlPlayerLayout);
    controlLayout->addLayout(controlBoardLayout);

    playerSelect->setLayout(mainLayout);
    sizeSelect->setLayout(mainLayout);
    mainLayout->addLayout(displayLayout);
    mainLayout->addLayout(controlLayout);

    setLayout(mainLayout);

    setWindowTitle(tr("GO game"));
}


void GameWindow::appendStatusText(Field field)
{
    std::ostringstream out;
    auto const pi = player_map.find(field.player);

    if (pi != player_map.end())
    {
        out << pi->second << " at coordinates: " << field.x() << ", " << field.y();
    }

    statusText->append(QString::fromStdString(out.str()));
}

void GameWindow::doDataText(std::vector<Field> group)
{
    dataText->clear();
    auto const pi = player_map.find(group.begin()->getPlayer());
    std::ostringstream header;

    header << "Removing " << pi->second << " stones at these coordinates:" << '\n';
    dataText->append(QString::fromStdString(header.str()));

    for (auto &i: group)
    {
        std::ostringstream out;
        out << "[" << i.x() << ", " << i.y() << "] ";
        dataText->append(QString::fromStdString(out.str()));
    }
}

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    GameWindow gameWindow;
    gameWindow.show();
    return app.exec();
}

#include "main.moc"