#include "tetrixboard.h"
#include "tetrixwindow.h"

#include <QCoreApplication>
#include <QGridLayout>
#include <QLabel>
#include <QLCDNumber>
#include <QPushButton>


TetrixWindow::TetrixWindow(QWidget *parent)
    : QWidget(parent), board(new TetrixBoard)
{

    nextPieceLabel = new QLabel;
    nextPieceLabel->setFrameStyle(QFrame::Box | QFrame::Raised);
    nextPieceLabel->setAlignment(Qt::AlignCenter);
    board->setNextPieceLabel(nextPieceLabel);

    scoreLcd = new QLCDNumber(5);
    scoreLcd->setSegmentStyle(QLCDNumber::Filled);

    linesLcd = new QLCDNumber(5);
    linesLcd->setSegmentStyle(QLCDNumber::Filled);

    startButton = new QPushButton(tr("&Новая игра"));
    startButton->setFocusPolicy(Qt::NoFocus);
    quitButton = new QPushButton(tr("&Выход"));
    quitButton->setFocusPolicy(Qt::NoFocus);

    connect(startButton, &QPushButton::clicked, board, &TetrixBoard::start);
    connect(quitButton , &QPushButton::clicked, qApp, &QCoreApplication::quit);
    connect(board, &TetrixBoard::scoreChanged,
            scoreLcd, QOverload<int>::of(&QLCDNumber::display));
    connect(board, &TetrixBoard::linesRemovedChanged,
            linesLcd, QOverload<int>::of(&QLCDNumber::display));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(createLabel(tr("Следующая фигура")), 0, 0);
    layout->addWidget(nextPieceLabel, 1, 0);
    layout->addWidget(startButton, 4, 0);
    layout->addWidget(board, 0, 1, 6, 1);
    layout->addWidget(createLabel(tr("Счёт")), 0, 2);
    layout->addWidget(scoreLcd, 1, 2);
    layout->addWidget(createLabel(tr("Линий собрано")), 2, 2);
    layout->addWidget(linesLcd, 3, 2);
    layout->addWidget(quitButton, 4, 2);
    setLayout(layout);

    setWindowTitle(tr("Тетрис"));
    resize(550, 370);
}

QLabel *TetrixWindow::createLabel(const QString &text)
{
    QLabel *label = new QLabel(text);
    label->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    return label;
}


