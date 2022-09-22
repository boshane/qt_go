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
};

GameWindow::GameWindow()
{
    initWindow();
}

void GameWindow::initWindow()
{
    int boardSize = 10;
    gameData = new GameData(boardSize);
    renderBoard = new RenderBoard(*gameData, this);
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

    playerOptionLabel = new QLabel(tr("Select player"));

    auto *mainLayout = new QGridLayout;
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(1, 1);
    mainLayout->setColumnMinimumWidth(3, 150);
    mainLayout->setColumnMinimumWidth(4, 150);
    mainLayout->addWidget(renderBoard, 0, 0, 1, 3);
    mainLayout->addWidget(statusText, 0, 3, 1, 4);
    mainLayout->addWidget(playerOptionLabel, 2, 0, 1, 1);
    mainLayout->addWidget(radioBlack);
    mainLayout->addWidget(radioWhite);
    playerSelect->setLayout(mainLayout);
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
        out << pi->second << " at coordinates: " << field.x() << ", " << field.y() << '\n';
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