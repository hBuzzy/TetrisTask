#include "mainwindow.h"
#include <QVBoxLayout>
#include <QApplication>
#include <QStyleFactory>
#include <QMessageBox>
#include <QtWidgets/QGraphicsDropShadowEffect>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), score_(0) {
    gameOverHandled_ = false;
    tetrisGrid = new TetrisGrid(this);
    startButton =new QPushButton("Начать новую игру", this);
    exitButton = new QPushButton("Выход", this);
    scoreLabel = new QLabel("Счетчик очков: 0", this);
    helpButton = new QPushButton("Справка", this);

    qApp->setStyle(QStyleFactory::create("Fusion"));
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(15);
    shadowEffect->setColor(QColor(0, 0, 0, 150));
    shadowEffect->setOffset(0, 5);

    QString buttonStyle = "QPushButton {"
                                        "   background-color: #3498db;"
                                        "   border-style: outset;"
                                        "   border-width: 2px;"
                                        "   border-radius: 10px;"
                                        "   border-color: #2980b9;"
                                        "   color: #ecf0f1;"
                                        "   padding: 5px;"
                                        "}"
                                        "QPushButton:hover {"
                                        "   background-color: #2980b9;"
                                        "   color: #bdc3c7;"
                                        "}";
    startButton->setStyleSheet(buttonStyle);
    startButton->setGraphicsEffect(shadowEffect);
    helpButton->setStyleSheet(buttonStyle);
    helpButton->setGraphicsEffect(shadowEffect);
    exitButton->setStyleSheet(buttonStyle);
    exitButton->setGraphicsEffect(shadowEffect);

    connect(helpButton, &QPushButton::clicked, this, &MainWindow::showHelp);
    connect(startButton, &QPushButton::clicked, this, &MainWindow::restartGame);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::exitGame);
    connect(tetrisGrid, SIGNAL(scoreChanged(int)), this, SLOT(updateScore(int)));
    connect(tetrisGrid, SIGNAL(gameOver()), this, SLOT(exitGame()));
    connect(tetrisGrid, &TetrisGrid::gameOver, [=]() {
        if (!gameOverHandled_) {
            gameOverHandled_ = true;
            exitGame();
        }
    });

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(tetrisGrid, Qt::AlignHCenter);
    layout->addWidget(scoreLabel);
    layout->addWidget(startButton);
    layout->addWidget(helpButton);
    layout->addWidget(exitButton);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow() {}

void MainWindow::showHelp() {
    QString helpText = "Правила игры:\n\n"
                       "1. Сдвигайте фигуры влево/вправо, используя стрелки клавиатуры.\n"
                       "2. Поворачивайте фигуры при помощи клавиши 'Вверх'.\n"
                       "3. Ускоряйте падение фигур с помощью кнопки 'Вниз'.\n"
                       "4. Заполняйте горизонтальные линии, чтобы они исчезали и приносили очки.\n"
                       "\nУправление:\n"
                       "- Сдвиг влево: Стрелка влево\n"
                       "- Сдвиг вправо: Стрелка вправо\n"
                       "- Поворот: Стрелка вверх\n"
                       "- Ускорение падения: Стрелка вниз'\n";
    QMessageBox::information(this, "Справка", helpText);
}

void MainWindow::restartGame() {
    tetrisGrid->startGame();
    setupGame();
}

void MainWindow::startGame() {
    setFocus();
    score_ = 0;
    updateScore(score_);
}

void MainWindow::exitGame() {
    gameOverHandled_ = true;
    QMessageBox messageBox;
    messageBox.setWindowTitle("Игра окончена");
    messageBox.setText("Игра завершена. Набрано очков: " + QString::number(score_));
    messageBox.setStandardButtons(QMessageBox::Ok);
    messageBox.exec();
    close();
}

void MainWindow::updateScore(int newScore) {
    score_ = newScore;
    scoreLabel->setText("Счетчик очков: " + QString::number(score_));
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
     tetrisGrid->handleKeyPressEvent(event);
}

void MainWindow::setupGame() {}
