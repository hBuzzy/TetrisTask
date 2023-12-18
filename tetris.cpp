#include "tetris.h"
#include "ui_tetris.h"
#include "gamefield.h"

#include <QMainWindow>
#include <QMessageBox>
#include <QLabel>
#include <QLCDNumber>
#include <QDebug>

Tetris::Tetris(QWidget *parent) : QMainWindow(parent), ui(new Ui::Tetris) {
  ui->setupUi(this);

  auto *gameField = new GameField(this);
  setCentralWidget(gameField);

  QMenu *gameMenu = new QMenu();
  gameMenu->setTitle("Menu");
  gameMenu->setVisible(true);

  QAction *startNewGameAction = new QAction("New Game");
  QAction *exitGameAction = new QAction("Exit");
  QAction *rulesAction = new QAction("Rules");

  connect(startNewGameAction, &QAction::triggered, gameField, &GameField::startNewGame);
  connect(exitGameAction, &QAction::triggered, this, &Tetris::close);
  connect(rulesAction, &QAction::triggered, this, &Tetris::showRules);

  gameMenu->addAction(startNewGameAction);
  gameMenu->addAction(exitGameAction);
  gameMenu->addAction(rulesAction);

  ui->menubar->addMenu(gameMenu);

  QLabel *label = new QLabel(this);
  label->setGeometry(350, 0, 100, 75);
  label->setText("Next Figure");
  label->show();

  QPushButton *newGameButton = new QPushButton("New Game", this);
  newGameButton->setGeometry(350, 200, 100, 25);
  QPushButton *exitButton = new QPushButton("Exit", this);
  exitButton->setGeometry(350, 250, 100, 25);

  connect(newGameButton, &QPushButton::clicked, gameField, &GameField::startNewGame);
  connect(exitButton, &QPushButton::clicked, this, &Tetris::close);
}
void Tetris::showRules() {
    QMessageBox::information(
                this,
                "Rules",
                "In Tetris, players complete lines by moving differently shaped pieces (tetrominoes), which descend onto the playing field.\n"
                "The completed lines disappear and grant the player points, and the player can proceed to fill the vacated spaces.\n"
                "The game ends when the uncleared lines reach the top of the playing field.\n"
                "\n"
                "Game Control\n"
                "  Up key- rotate figure\n"
                "  Left key - move  the figure Left\n"
                "  Right key - move  the figure Right\n"
                "  Down key - lower the figure down\n"
                );
  }

Tetris::~Tetris() { delete ui; }
