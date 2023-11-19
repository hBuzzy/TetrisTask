#include "tetris.h"

#include <QMessageBox>

#include "gamefield.h"

Tetris::Tetris(QWidget *parent) : QMainWindow(parent), ui(new Ui::Tetris) {
  ui->setupUi(this);

  menuBar()->setStyleSheet(QString("font-size: %1px").arg(14));

  QMenu *gameMenu = menuBar()->addMenu("&Игра");

  QAction *startNewGame = new QAction("&Новая игра", this);
  QAction *exitGame = new QAction("&Выйти из игры", this);
  QAction *help = new QAction("&Справка", this);

  gameMenu->addAction(startNewGame);
  gameMenu->addAction(exitGame);
  gameMenu->addAction(help);

  connect(startNewGame, &QAction::triggered, ui->gameField,
          &GameField::StartNewGame);
  connect(exitGame, &QAction::triggered, qApp, &QApplication::quit);
  connect(help, &QAction::triggered, this, &Tetris::ShowHelp);

  connect(ui->newGameButton, &QPushButton::clicked, ui->gameField,
          &GameField::StartNewGame);
  connect(ui->exitGameButton, &QPushButton::clicked, qApp, &QApplication::quit);

  connect(ui->gameField, &GameField::ScoreChanged, ui->counter,
          QOverload<int>::of(&QLCDNumber::display));

  connect(ui->gameField, &GameField::NextFigureGenerated, [=]() {
    ui->gameField->ShowNextFigure(ui->nextFigurePicture_, ui->nextFigureLabel_);
  });
}

void Tetris::ShowHelp() {
  QMessageBox::about(
      this, tr("Управление"),
      tr("<br><b>Клавиша →</b>  Двигаться вправо \n</br>"
         "<br><b>Клавиша ←</b>  Двигаться влево \n</br>"
         "<br><b>Клавиша ↑</b>  Повернуть по часовой стрелке \n</br>"
         "<br><b>Клавиша ↓</b>  Повернуть против часовой стрелки \n</br>"
         "<br><b>Клавиша V</b>  Досрочно спустить фигуру на одну линию \n</br>"
         "<br><b>Пробел </b>  Спустить фигуру в самый низ \n</br>"));
}

Tetris::~Tetris() { delete ui; }
