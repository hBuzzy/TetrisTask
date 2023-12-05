#include "mainwindow.h"
#include <QVBoxLayout>
#include <QApplication>
#include <QStyleFactory>
#include <QMessageBox>
#include <QtWidgets/QGraphicsDropShadowEffect>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), score_(0) {
    isGameOverHandled_ = false;
    tetrisGrid_ = new TetrisGrid(this);
    startButton_ =new QPushButton("Начать новую игру", this);
    exitButton_ = new QPushButton("Выход", this);
    scoreLabel_ = new QLabel("Счетчик очков: 0", this);
    helpButton_ = new QPushButton("Справка", this);

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
    startButton_->setStyleSheet(buttonStyle);
    startButton_->setGraphicsEffect(shadowEffect);
    helpButton_->setStyleSheet(buttonStyle);
    helpButton_->setGraphicsEffect(shadowEffect);
    exitButton_->setStyleSheet(buttonStyle);
    exitButton_->setGraphicsEffect(shadowEffect);

    connect(helpButton_, &QPushButton::clicked, this, &MainWindow::showHelp);
    connect(startButton_, &QPushButton::clicked, this, &MainWindow::restartGame);
    connect(exitButton_, &QPushButton::clicked, this, &MainWindow::exitGame);
    connect(tetrisGrid_, SIGNAL(scoreChanged(int)), this, SLOT(updateScore(int)));
    connect(tetrisGrid_, &TetrisGrid::gameOver, [=]() {
        if (!isGameOverHandled_) {
            isGameOverHandled_ = true;
            exitGame();
        }
    });

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(tetrisGrid_, Qt::AlignHCenter);
    layout->addWidget(scoreLabel_);
    layout->addWidget(startButton_);
    layout->addWidget(helpButton_);
    layout->addWidget(exitButton_);

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
    tetrisGrid_->startGame();
    setupGame();
}

void MainWindow::startGame() {
    setFocus();
    score_ = 0;
    updateScore(score_);
}

void MainWindow::exitGame() {
    isGameOverHandled_ = true;
    QMessageBox messageBox;
    messageBox.setWindowTitle("Игра окончена");
    messageBox.setText("Игра завершена. Набрано очков: " + QString::number(score_));
    messageBox.setStandardButtons(QMessageBox::Ok);
    messageBox.exec();
    close();
}

void MainWindow::updateScore(int newScore) {
    score_ = newScore;
    scoreLabel_->setText("Счетчик очков: " + QString::number(score_));
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
     tetrisGrid_->handleKeyPressEvent(event);
}

void MainWindow::setupGame() {}
