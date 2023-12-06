#include "tetris.h"

#include "ui_tetris.h"
#include <QHBoxLayout>
#include <QTimer>
#include <QGridLayout>
#include <QLCDNumber>
#include <QDebug>

Tetris::Tetris(QWidget *parent) : QMainWindow(parent), ui(new Ui::Tetris) {
    ui->setupUi(this);

    baseSpeed_ = 400;
    boostSpeed_ = 10;

    this->setMinimumSize(1200, 800);
    this->setMaximumSize(1200, 800);

    scoreCounter_ = new QLCDNumber;
    scoreCounter_->setFixedSize(200, 50);
    scoreCounter_->setDigitCount(6);
    scoreCounter_->display("000000");

    nextFigureGrid_ = new NextFigureGameGrid(this);
    gamefield_ = new GameField(this);

    gamefield_->setFixedSize(601, 721);

    QPushButton *newGameButton = new QPushButton("Новая игра");

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(nextFigureGrid_, 1, 0, Qt::AlignHCenter);
    gridLayout->addWidget(gamefield_, 1, 1, Qt::AlignHCenter);
    gridLayout->addWidget(scoreCounter_, 1, 2, Qt::AlignHCenter);
    gridLayout->addWidget(newGameButton, 0, 1, Qt::AlignHCenter);

    setCentralWidget(new QWidget(this));
    centralWidget()->setLayout(gridLayout);

    timer_ = new QTimer(this);

    connect(newGameButton, &QPushButton::clicked, nextFigureGrid_, &NextFigureGameGrid::SetNextFigure);
    connect(newGameButton, &QPushButton::clicked, this, &Tetris::UpdateGameFieldFigure);
    connect(newGameButton, &QPushButton::clicked, this, &Tetris::StartNewGame);
    connect(gamefield_, &GameField::FigureSpawned, nextFigureGrid_, &NextFigureGameGrid::SetNextFigure);
    connect(nextFigureGrid_, &NextFigureGameGrid::NextFigureChanged, this, &Tetris::UpdateGameFieldFigure);
    connect(timer_, &QTimer::timeout, gamefield_, &GameField::MoveFigure);
    connect(gamefield_, &GameField::FigureSpawned, this, &Tetris::RestoreBasedInterval);
    connect(gamefield_, &GameField::CurrentFigureChanged, this, &Tetris::UpdateGameField);
    connect(gamefield_, &GameField::ChangeWinPoints, this, &Tetris::UpdateWinPoints);
    connect(gamefield_, &GameField::GameOver, this, &Tetris::OpenEndGameDialog);
}

Tetris::~Tetris() { delete ui; }

void Tetris::UpdateGameField() {
    gamefield_->update();
}

void Tetris::UpdateGameFieldFigure() {
    gamefield_->SetNextFigureGrid(nextFigureGrid_->GetNextFigure());
}

void Tetris::UpdateWinPoints() {
    int currentCountPoints = gamefield_->GetWinPoints();
    QString newDisplayValue = QString::number(currentCountPoints).rightJustified(scoreCounter_->digitCount(), '0');
    scoreCounter_->display(newDisplayValue);
}

void Tetris::RestoreBasedInterval() {
    if (timer_->interval() != baseSpeed_) {
        timer_->setInterval(baseSpeed_);
        timer_->start();
    }
}

void Tetris::GamePause() {
    timer_->stop();
    OpenModalDialog();
}

void Tetris::StartNewGame() {
    int CenterPointX = 2;
    timer_->stop();
    gamefield_->SetWinPoints(0);
    UpdateWinPoints();

    gamefield_->ClearGameGrid();
    gamefield_->SpawnNextFigure();
    gamefield_->SetFigurePosition(0, gamefield_->GetColumnsNumber() / CenterPointX);

    gamefield_->setFocus();
    timer_->start(baseSpeed_);
}

void Tetris::EndGame() {
    timer_->stop();
    gamefield_->ClearGameGrid();
    gamefield_->SetWinPoints(0);
}

void Tetris::OpenModalDialog() {
    ModalDialog dialog(this);
    dialog.exec();
    timer_->start(400);
}

void Tetris::OpenEndGameDialog() {
    GameOverDialog dialog(this);
    dialog.exec();
    EndGame();
}

void Tetris::keyPressEvent(QKeyEvent *event) {
    int currRow = gamefield_->GetCurrentFigureRow();
    int currColumn = gamefield_->GetCurrentFigureColumn();

    if (event->key() == Qt::Key_Space) {
        GamePause();
    }

    if (event->key() == Qt::Key_Right) {
        if(!gamefield_->HasCollisionMove(0, 1)) {
            gamefield_->SetFigurePosition(currRow, currColumn + 1);
        }
    }

    if (event->key() == Qt::Key_Left) {
        if(!gamefield_->HasCollisionMove(0, -1)) {
            gamefield_->SetFigurePosition(currRow, currColumn - 1);
        }
    }

    if (event->key() == Qt::Key_Down) {
        timer_->start(boostSpeed_);
    }

    if (event->key() == Qt::Key_Up) {
        gamefield_->GetRotateCurrentFigure();

        QVector<QVector<int>> CurrentFigure = gamefield_->GetCurrentFigure();

        if(!gamefield_->HasCollisionRotation()) {
            gamefield_->GetCurrentFigure();
            gamefield_->update();
        } else {
            gamefield_->SetFigurePosition(currRow, currColumn - CurrentFigure[0].size() + 1);
            gamefield_->update();
        }
    }
}
