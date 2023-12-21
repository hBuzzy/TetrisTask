#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    int score = 0;
    tetris *tetris_ = new tetris();
    nextfigure *nextfigure_ = new nextfigure();

    QPushButton *buttonNewGame = new QPushButton("Новая игра");
    QPushButton *buttonExit = new QPushButton("Выход");
    QLabel *labelScore = new QLabel("Cчетчик очков: " + QString::number(score));
    QLabel *labelGameOver = new QLabel();

    connect(buttonNewGame, &QPushButton::clicked, tetris_, &tetris::newGame);
    connect(buttonNewGame, &QPushButton::clicked, [=]() mutable{
        score = 0;
        labelScore->setText("Cчетчик очков: " + QString::number(score));
        labelGameOver->setText("");
    });
    connect(buttonExit, &QPushButton::clicked, this, &QMainWindow::close);
    connect(tetris_, &tetris::fullRow, [=]() mutable{
        score += 5;
        labelScore->setText("Cчетчик очков: " + QString::number(score));
    });
    connect(tetris_, &tetris::gameFinished, [=]() mutable{
        labelGameOver->setText("ИГРА ОКОНЧЕНА");
    });
    connect(tetris_, &tetris::nextStep, nextfigure_, &nextfigure::nextStep);
    connect(nextfigure_, &nextfigure::figureChanged, tetris_, &tetris::onFigureChanged);


    QAction *newGameAction = new QAction("Начать новую игру", this);
    connect(newGameAction, &QAction::triggered, tetris_, &tetris::newGame);

    QAction *exitAction = new QAction("Выйти", this);
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);

    QAction *helpAction = new QAction("Справка", this);
    connect(helpAction, &QAction::triggered, this, [=](){
        QMessageBox::information(this, "Справка", QString(u8"\u2193") + " - спускает фигуру на одну ячейку вниз\n" + QString(u8"\u2190") + " - сдвигает фигуру на одну ячейку влево\n" + QString(u8"\u2192") + " - сдвигает фигуру на одну ячейку вправо\n" + QString(u8"\u2191") + " - поворачивает фигуру\n Пробел - сбрасывает фигуру вниз");
    });

    QMenu *gameMenu = menuBar()->addMenu("Игра");
    gameMenu->addAction(newGameAction);
    gameMenu->addAction(exitAction);

    QMenu *helpMenu = menuBar()->addMenu("Справка");
    helpMenu->addAction(helpAction);

    QHBoxLayout *hLayout = new QHBoxLayout;
    QVBoxLayout *vLayout = new QVBoxLayout;
    hLayout->addWidget(tetris_);
    vLayout->addWidget(nextfigure_);
    vLayout->addWidget(buttonNewGame);
    vLayout->addWidget(labelScore);
    vLayout->addWidget(labelGameOver);
    vLayout->addWidget(buttonExit);
    hLayout->addLayout(vLayout);


    setCentralWidget(new QWidget);
    centralWidget()->setLayout(hLayout);

    GridDrawer *tetrisGrid = tetris_->getGrid();
    GridDrawer *nextFigureGrid = nextfigure_->getGridDrawer();
    int width_ = tetrisGrid->columns() * tetrisGrid->sizeCell() + nextFigureGrid->columns()*nextFigureGrid->sizeCell() + 250;
    int hieght_ = tetrisGrid->rows() * tetrisGrid->sizeCell() + 300;
    resize(width_, hieght_);

    tetris_->setFocus();
}
MainWindow::~MainWindow() {}

