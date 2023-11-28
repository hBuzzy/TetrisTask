#include "tetris.h"

#include "ui_tetris.h"
#include <QHBoxLayout>
#include <QTimer>
#include <QGridLayout>
#include <QLCDNumber>
#include <QDebug>

Tetris::Tetris(QWidget *parent) : QMainWindow(parent), ui(new Ui::Tetris) {
    ui->setupUi(this);

    this->setMinimumSize(1200, 800);
    this->setMaximumSize(1200, 800);

    lcdNumber = new QLCDNumber;
    lcdNumber->setFixedSize(200,50);
    lcdNumber->setDigitCount(6);
    lcdNumber->display("000000");


    gamefield = new GameField(this);
    gamefield->setFixedSize(601,721);

    QLabel *labelPoints = new QLabel;

    labelPoints->setText("привет");

    QPushButton *newGameButton = new QPushButton("Новая игра");

    QGridLayout *gridLayout = new QGridLayout;

    gridLayout->addWidget(labelPoints,1, 0);
    gridLayout->addWidget(gamefield, 1, 1, Qt::AlignHCenter);
    gridLayout->addWidget(lcdNumber, 1, 2, Qt::AlignHCenter);
    gridLayout->addWidget(newGameButton, 0, 1, Qt::AlignHCenter);

    setCentralWidget(new QWidget(this));
    centralWidget()->setLayout(gridLayout);

    // Создаем таймер для движения вниз
    timer = new QTimer(this);
    connect(newGameButton, &QPushButton::clicked, this, &Tetris::startNewGame);
    connect(timer, &QTimer::timeout, gamefield, &GameField::moveFigure);
    connect(gamefield, &GameField::FigureSpawned, this, &Tetris::restoreBasedInterval);
    connect(gamefield, &GameField::CurrentFigureChanged, this, &Tetris::updateGameField);
    connect(gamefield, &GameField::ChangeWinPoints, this, &Tetris::updateWinPoints);
    connect(gamefield, &GameField::GameOver, this, &Tetris::openEndGameDialog);
}

Tetris::~Tetris() { delete ui; }


void Tetris::updateGameField() {
    gamefield->update();
}

void Tetris::updateWinPoints() {
    int currentCountPoints = gamefield->GetWinPoints();

    QString newDisplayValue = QString::number(currentCountPoints).rightJustified(lcdNumber->digitCount(), '0');
    lcdNumber->display(newDisplayValue);
}

void Tetris::restoreBasedInterval() {
    if (timer->interval() != 400) {
        timer->setInterval(400);
        timer->start();
    }
}

void Tetris::gamePause() {
    timer->stop();
    openModalDialog();
}

void Tetris::startNewGame() {
    timer->stop();

    gamefield->SetWinPoints(0);
    updateWinPoints();

    gamefield->clearGameGrid();
    gamefield->spawnNextFigure();
    gamefield->SetFigurePosition(0, gamefield->GetColumnsNumber() / 2);

    gamefield->setFocus();

    timer->start(400);
}

void Tetris::endGame() {
    timer->stop();
    gamefield->clearGameGrid();
}

void Tetris::openModalDialog() {
    ModalDialog dialog(this);
    dialog.exec();
    timer->start(400);
}



void Tetris::openEndGameDialog() {
    GameOverDialog dialog(this);
    dialog.exec();
    endGame();
}

void Tetris::keyPressEvent(QKeyEvent *event) {
    int currRow = gamefield->GetCurrentFigureRow();
    int currColumn = gamefield->GetCurrentFigureColumn();

    if (event->key() == Qt::Key_Space) {
        gamePause();
    }

    if (event->key() == Qt::Key_Right) {
        if(!gamefield->CheckCollisionMoveRight()) {
            gamefield->SetFigurePosition(currRow, currColumn + 1);
        }
    }

    if (event->key() == Qt::Key_Left) {
        if(!gamefield->CheckCollisionMoveLeft()) {
            gamefield->SetFigurePosition(currRow, currColumn - 1);
        }
    }

    if (event->key() == Qt::Key_Down) {
        timer->start(10);
    }

    if (event->key() == Qt::Key_Up) {
        gamefield->GetRotateCurrentFigure();

        QVector<QVector<int>> CurrentFigure = gamefield->GetCurrentFigure();

        if(!gamefield->CheckCollisionRotate()) {
            gamefield->GetCurrentFigure();
            gamefield->update();
            //qDebug() << "Пересечений нет";
        } else {
            gamefield->SetFigurePosition(currRow, currColumn - CurrentFigure[0].size() + 1);
            gamefield->update();
            //qDebug() << "Пересечения есть";
            //qDebug() << CurrentFigure[0].size();
        }
    }
}


