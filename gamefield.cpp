#include "gamefield.h"

GameField::GameField(QWidget *parent): QWidget(parent) {
    gameState_ = new GameState(this);

    connect(gameState_, &GameState::figureMoved, this, &GameField::updateUI);
    connect(gameState_, &GameState::lineCleared, this, &GameField::updateUI);

    connect(this, &GameField::leftKeyPressed, gameState_, &GameState::moveLeft);
    connect(this, &GameField::rightKeyPressed, gameState_, &GameState::moveRight);
    connect(this, &GameField::downKeyPressed, gameState_, &GameState::moveDown);
    connect(this, &GameField::rotateKeyPressed, gameState_, &GameState::rotate);

    QPushButton *newGameBtn = new QPushButton("Новая игра", this);
    QPushButton *exitBtn = new QPushButton("Выйти", this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addStretch();
    layout->addWidget(newGameBtn, 0, Qt::AlignRight | Qt::AlignBottom);
    layout->addWidget(exitBtn, 0, Qt::AlignRight | Qt::AlignBottom);

    QObject::connect(newGameBtn, &QPushButton::clicked, this, &GameField::startNewGame);
    QObject::connect(exitBtn, &QPushButton::clicked, parent, &QMainWindow::close);
}

int GameField::getRowsNumber() const {
    return rowsNumber_;
}

void GameField::setRowsNumber(int rowsNumber) {
    if (rowsNumber_ == rowsNumber) return;

    rowsNumber_ = rowsNumber;
    emit rowsNumberChanged();
}

int GameField::getColumnsNumber() const {
    return columnsNumber_;
}

void GameField::setColumnNumber(int columnsCount) {
    if (columnsNumber_ == columnsCount) return;

    columnsNumber_ = columnsCount;
    emit columnsNumberChanged();
}

void GameField::startNewGame() {
    gameState_->startNewGame();
}

void GameField::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);

    QColor dark(QColor(100, 100, 100));
    QColor green(QColor(100, 210, 100));

    painter.fillRect(0, 0, 400, 800, Qt::white);
    painter.fillRect(400, 0, 400, 800, dark);

    QVector<QVector<int>> gameBoard = gameState_ -> getGameBoard();
    int cellSize = 40;

    for (int i = 0; i < gameBoard.size(); ++i) {
        for (int j = 0; j < gameBoard[i].size(); ++j) {
            if (gameBoard[i][j] == 1) {
                painter.fillRect(j * cellSize, i * cellSize, cellSize, cellSize, green);
            } else {
                painter.drawRect(j * cellSize, i * cellSize, cellSize, cellSize);
            }
        }
    }

    QFont font("Arial", 12);
    painter.setFont(font);
    painter.setPen(Qt::white);
    painter.drawText(xPramText, 100, QString("Очки: %1").arg(gameState_ -> getScore()));

    painter.setPen(Qt::white);
    painter.drawText(xPramText, 200, QString("Следующая:"));

    Figure currentFigure_ = gameState_->getNextFigure();
    for (int i = 0; i < currentFigure_.getHeight(); ++i){
        for (int j = 0; j < currentFigure_.getWidth(); ++j){
            if (currentFigure_.getShapeAt(j, i) == 1){
                 painter.fillRect(yParamDraw + j * cellSize, yParamDraw + i * cellSize, cellSize, cellSize, Qt::white);
            }
        }
    }
}

void GameField::updateUI() {
    repaint();
}
