#include "tetris.h"

#include "ui_tetris.h"

#include <QMessageBox>

Tetris::Tetris(QWidget *parent) : QMainWindow(parent), ui(new Ui::Tetris) {
    ui->setupUi(this);
    connect(
        ui->newGameButton, &QPushButton::clicked,
        ui->gameField, &GameField::StartNewGame
    );
    connect(
        ui->menuNewGame, &QAction::triggered,
        ui->gameField, &GameField::StartNewGame
    );
    connect(
        ui->gameField, &GameField::NextFigureUpdated,
        ui->nextFigureDisplay, &Table::DrawFigure
    );
    connect(
        ui->gameField, &GameField::ScoreUpdated,
        ui->scoreDisplay, static_cast<void (QLCDNumber::*)(int)>(&QLCDNumber::display)
    );
    connect(
        ui->gameField, &GameField::GameOver,
        ui->nextFigureDisplay, &Table::ResetCellsColor
    );
    connect(
        ui->gameField, &GameField::GameOver,
        ui->scoreDisplay, [this](){
            ui->scoreDisplay->display(0);
        }
    );
    connect(
        ui->exitButton, &QPushButton::clicked,
        this, &Tetris::close
    );
    connect(
        ui->menuExit, &QAction::triggered,
        this, &Tetris::close
    );
    connect(
        ui->menuAbout, &QAction::triggered,
        this, &Tetris::ShowHelp
    );
}

Tetris::~Tetris() { delete ui; }

void Tetris::ShowHelp() const {
    QMessageBox::about((QWidget*)this, "How to play",
        "Controls:\n"
        "\t-Move Left: Left Key;\n"
        "\t-Move Right: Right Key;\n"
        "\t-Rotate right: Up Key;\n"
        "\t-Soft drop: Down Key;\n"
        "\nGame rules:\n"
        "\t- filling a line causes it to be cleared"
        " resulting in score award in an amount"
        " that is a multiple of row length;\n"
        "\t- if you stack blocks too high the game will over;\n"
    );
}
