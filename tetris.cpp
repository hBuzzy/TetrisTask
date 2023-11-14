#include "tetris.h"

#include "ui_tetris.h"

Tetris::Tetris(QWidget *parent) : QMainWindow(parent), ui(new Ui::Tetris) {
  ui->setupUi(this);

  ui->centralwidget->setStyleSheet("background-color: black; color: white;");
  ui->exitButton->setStyleSheet("background-color: black;");
  ui->newGameButton->setStyleSheet("background-color: black;");
  QMenu *gameMenu = menuBar()->addMenu("&Игра");
  QAction *startNewGame = new QAction("&Нова игра", this);

  gameMenu->addAction(startNewGame);
  connect(startNewGame, &QAction::triggered, ui->gameField,
          &GameField::StartNewGame);

  connect(
    ui->gameField, &GameField::nextFigureChanged,
    [this](){ ui->prevWindow->StartNewGame(ui->gameField->GetNextFigure()); }
  );

  connect(
    ui->gameField, &GameField::clearPrevWindow, ui->prevWindow, &PreviewWindow::ResetCellsColor
  );

  connect(ui->newGameButton, &QPushButton::clicked, ui->gameField,
          &GameField::StartNewGame);

  connect(
      ui->gameField, &GameField::lsdNumberChanged,
      ui->labelScore, [this](){ ui->labelScore->setText(ui->gameField->GetScore()); }
  );

  connect(
      ui->exitButton, &QPushButton::clicked,
      this, &Tetris::close
  );
}

Tetris::~Tetris() { delete ui; }
