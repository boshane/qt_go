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

signals:
    void appendStatus();

private slots:
    void radioSetWhite(bool checked);
    void radioSetBlack(bool checked);
    void appendStatusText(Field);
    void newGame();

private:
    void initWindow();
    RenderBoard *renderBoard;
    GameData *gameData;
    QLabel *playerOptionLabel;
    QGroupBox *playerSelect;
    QRadioButton *radioBlack;
    QRadioButton *radioWhite;
    QButtonGroup *playerButtons;
    QTextEdit *statusText;
    QTextEdit *dataText;
    QPushButton *resetGame;
    QVBoxLayout *mainLayout;
};

GameWindow::GameWindow()
{
    initWindow();
}

void GameWindow::newGame()
{
    gameData->populate_fields();
    update();
}

void GameWindow::initWindow()
{
    int boardSize = 10;

    QHBoxLayout* displayLayout = new QHBoxLayout;
    QHBoxLayout* controlLayout = new QHBoxLayout;
    QVBoxLayout* displaySubLayout = new QVBoxLayout;

    gameData = new GameData(boardSize);
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

    connect(radioBlack, SIGNAL(clicked(bool)), SLOT(radioSetBlack(bool)));
    connect(radioWhite, SIGNAL(clicked(bool)), SLOT(radioSetWhite(bool)));
    connect(renderBoard, SIGNAL(appendStatus(Field)), SLOT(appendStatusText(Field)));
    connect(resetGame, SIGNAL(clicked(bool)), SLOT(newGame()));

    playerOptionLabel = new QLabel(tr("Select player"));

    mainLayout = new QVBoxLayout;
    displayLayout->addWidget(renderBoard);
    displaySubLayout->addWidget(statusText);
    displaySubLayout->addWidget(dataText);
    displayLayout->addLayout(displaySubLayout);

    controlLayout->addWidget(playerOptionLabel);
    controlLayout->addWidget(radioBlack);
    controlLayout->addWidget(radioWhite);
    controlLayout->addWidget(resetGame);
    playerSelect->setLayout(mainLayout);

    mainLayout->addLayout(displayLayout);
    mainLayout->addLayout(controlLayout);

    setLayout(mainLayout);

    setWindowTitle(tr("GO game"));
}

void GameWindow::radioSetBlack(bool checked)
{
    if (checked)
    {
        gameData->_currentPlayer = Player::Black;
    }
}

void GameWindow::radioSetWhite(bool checked)
{
    if (checked)
    {
        gameData->_currentPlayer = Player::White;
    }
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

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    GameWindow gameWindow;
    gameWindow.show();
    return app.exec();
}

#include "main.moc"