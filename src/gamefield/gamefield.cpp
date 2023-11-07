#include "gamefield.h"

#include <QKeyEvent>
#include <QPainter>
#include <QMessageBox>

GameField::GameField(QWidget *parent): Table(parent), timer_(this) {
    connect(&timer_, &QTimer::timeout, this, &GameField::FigureDrop);
}

void GameField::StartNewGame() {
    setFocus();
    ResetCellsColor();
    nextFigure_ = figureGenerator_();
    NextFigure();
    timer_.start(500);
}

void GameField::StopGame() {
    ResetCellsColor();
    timer_.stop();
    score_ = 0;
    emit GameOver();
}

void GameField::FigureDrop() {
    DrawFigure(currentFigure_);
    repaint();
    if (IsOutOfBounds(currentFigure_, 0, 1)) {
        if (currentFigure_.GetTop() + dy_)
            NextFigure();
        else {
            QMessageBox::critical(
                (QWidget*)this->parent(), "Game over",
                "Your block stack has grown too high."
            );
            StopGame();
        }
        return;
    }
    EraseFigure(currentFigure_);
    ++dy_;
}

void GameField::NextFigure() {
    dy_ = 0, dx_ = 0;
    currentFigure_ = std::exchange(nextFigure_, figureGenerator_());
    emit NextFigureUpdated(nextFigure_);
    uint count = std::ranges::count_if(cellsColors_, [](auto&& line){
        return std::ranges::all_of(
            std::forward<decltype(line)>(line),
            [](const QColor& color){
                return color != kCellDefaultColor;
            }
        );
    });
    if (count) {
        QVector<QVector<QColor>> copy {rowsCount_};
        for (auto& cell : copy) {
            cell.resize(columnsCount_);
            cell.fill(kCellDefaultColor);
        }
        std::ranges::copy_if(
            std::ranges::views::reverse(cellsColors_), copy.rbegin(), [](auto&& line){
                return std::ranges::any_of(
                    std::forward<decltype(line)>(line),
                    [](const QColor& color){
                        return color == kCellDefaultColor;
                    }
                );
            }
        );
        cellsColors_ = copy;
        repaint();
        score_ += static_cast<int>(count * columnsCount_);
        emit ScoreUpdated(score_);
    }
}

void GameField::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
        case Qt::Key_Up:
            RedrawFigure(
                !IsOutOfBounds(currentFigure_.Rotated()),
                [this](){ currentFigure_.Rotate(); }
            );
            break;
        case Qt::Key_Left:
            RedrawFigure(
                !IsOutOfBounds(currentFigure_, -1),
                [this](){ --dx_; }
            );
            break;
        case Qt::Key_Down:
            RedrawFigure(
                !IsOutOfBounds(currentFigure_, 0, 1),
                [this](){ ++dy_; }
            );
            break;
        case Qt::Key_Right:
            RedrawFigure(
                !IsOutOfBounds(currentFigure_, 1),
                [this](){ ++dx_; }
            );
            break;
    }
}

void GameField::RedrawFigure(bool condition, auto&& function) {
    if (condition) {
        EraseFigure(currentFigure_);
        std::forward<decltype(function)>(function)();
        DrawFigure(currentFigure_);
        repaint();
    }
}

void GameField::DrawFigure(const Figure& figure) {
    QColor color = figure.GetColor();
    for (auto&& [y, x] : figure.GetGeometry())
        cellsColors_[y + dy_][x + dx_] = color;
}

void GameField::EraseFigure(const Figure& figure) {
    for (auto&& [y, x] : figure.GetGeometry())
        cellsColors_[y + dy_][x + dx_] = kCellDefaultColor;
}

bool GameField::IsOutOfBounds(const Figure& figure, int dx, int dy) {
    bool result = static_cast<int>(figure.GetLeft()) + dx_ + dx < 0
        || static_cast<int>(figure.GetRight()) + dx_ + dx >= columnsCount_
        || static_cast<int>(figure.GetBottom()) + dy_ + dy >= rowsCount_
        || std::ranges::any_of(
            std::ranges::views::filter(figure.GetGeometry(), [this, dx, dy](auto&& cell){
                return std::ranges::all_of(
                    currentFigure_.GetGeometry(),
                    [this, cell = std::forward<decltype(cell)>(cell), dx, dy](auto&& current){
                        return std::forward<decltype(current)>(current).first + dy_ != cell.first + dy_ + dy
                        || std::forward<decltype(current)>(current).second + dx_ != cell.second + dx_ + dx;
                    }
                );
            }), [this, dx, dy](auto&& cell) {
                return cellsColors_[std::forward<decltype(cell)>(cell).first + dy_ + dy]
                [std::forward<decltype(cell)>(cell).second + dx_ + dx] != kCellDefaultColor;
            }
    );
    return result;
}
