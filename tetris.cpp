#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    auto *gameField = new GameField(this);
    setCentralWidget(gameField);


    connect(this, &MainWindow::leftKeyPressed, gameField, &GameField::leftKeyPressed);
    connect(this, &MainWindow::rightKeyPressed, gameField, &GameField::rightKeyPressed);
    connect(this, &MainWindow::downKeyPressed, gameField, &GameField::downKeyPressed);
    connect(this, &MainWindow::rotateKeyPressed, gameField, &GameField::rotateKeyPressed);

    setFixedSize(800, 840);

    QMenuBar* menuBar = this->menuBar();

    QMenu* gameMenu = menuBar->addMenu("Игра");

    QAction* newGameAction = new QAction("Начать новую игру", this);
    connect(newGameAction, &QAction::triggered, gameField, &GameField::startNewGame);
    gameMenu->addAction(newGameAction);

    QAction* exitAction = new QAction("Выйти", this);
    connect(exitAction, &QAction::triggered, this, &MainWindow::close);
    gameMenu->addAction(exitAction);

    QMenu* helpMenu = menuBar->addMenu("Справка");

    QAction* rulesAction = new QAction("Правила и управление", this);
    connect(rulesAction, &QAction::triggered, this, &MainWindow::showRules);
    helpMenu->addAction(rulesAction);
}

void MainWindow::showRules() {
    QMessageBox::information(
                this,
                "Правила и управление",
                "Управление производится стрелочками:\n"
                "  Вверх - поворот фигуры\n"
                "  Влево - движение влево\n"
                "  Вправо - движение вправо\n"
                "  Вниз - ускоренное движение вниз\n"
                );
  }

void MainWindow::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Left:
            emit leftKeyPressed();
            break;
        case Qt::Key_Right:
            emit rightKeyPressed();
            break;
        case Qt::Key_Down:
            emit downKeyPressed();
            break;
        case Qt::Key_Up:
            emit rotateKeyPressed();
            break;
        default:
            QWidget::keyPressEvent(event);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

