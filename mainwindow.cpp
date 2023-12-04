#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>
#include <QColor>
#include <QTimer>
#include <QKeyEvent>
#include "figures.h"
#include "logic.h"
#include <QMessageBox>

QTimer *timer;
Logic* boardlogic;

MainWindow::MainWindow( QWidget *parent) :
    QMainWindow(parent),
    ui( new Ui::MainWindow)
{
    ui->setupUi(this);
    this->grabKeyboard();
    // Connecting the timer
    speed = 450;
    boardlogic = new Logic(this);
    timer = new QTimer(this);






    connect(timer, SIGNAL(timeout()), this, SLOT(tmr_game()));
}
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    // Draw board
    for (int i = 0; i < 20; i++)
    for (int j = 0; j < 10; j++)
    {
        painter.setPen(QColor(0,0,0));
        switch(boardlogic->Board[i][j])
        {
            case 0:
                painter.setBrush(QColor(255,255,255));
                break;
            case 1:
                painter.setBrush(QColor(251,188,93));
                break;
            case 2:
                painter.setBrush(QColor(223,112,79));
                break;
            case 3:
                painter.setBrush(QColor(168,87,91));
                break;
            case 4:
                painter.setBrush(QColor(120,55,67));
                break;
            case 5:
                painter.setBrush(QColor(109,85,118));
                break;
            case 6:
                painter.setBrush(QColor(117, 145, 224));
                break;
            case 7:
                painter.setBrush(QColor(196,196,167));
                break;
            case 8:
                painter.setBrush(QColor(194, 165, 240));
                break;
            }
            painter.drawRect(j * 29 + 165, i * 29 + 10, 29, 29);
        }
    // Draw next piece box
    painter.setPen(QColor(0,0,0));
    painter.setBrush(QColor(255,255,255));
    painter.drawRect(520, 210, 161, 161);
    if (timer->isActive()) {
    // Draw current piece
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
            {
                painter.setPen(QColor(0,0,0));
                switch(boardlogic->CurrentPiece.PieceColor)
                {
                    case 0:
                        painter.setBrush(QColor(255,255,255));
                        break;
                    case 1:
                        painter.setBrush(QColor(251,188,93));
                        break;
                    case 2:
                        painter.setBrush(QColor(223,112,79));
                        break;
                    case 3:
                        painter.setBrush(QColor(168,87,91));
                        break;
                    case 4:
                        painter.setBrush(QColor(120,55,67));
                        break;
                    case 5:
                        painter.setBrush(QColor(109,85,118));
                        break;
                    case 6:
                        painter.setBrush(QColor(117, 145, 224));
                        break;
                    case 7:
                        painter.setBrush(QColor(196,196,167));
                        break;
                    case 8:
                        painter.setBrush(QColor(194, 165, 240));
                        break;
                }
                if (boardlogic->CurrentPiece.PieceArray[i][j])
                painter.drawRect(165 + (boardlogic->CurrentPiece.X + j) *
                29, 10 + (boardlogic->CurrentPiece.Y + i) * 29, 29, 29);
          }
        // Draw next piece
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
            {
                painter.setPen(QColor(0,0,0));
                switch(boardlogic->NextPiece.PieceColor)
                {
                    case 0:
                        painter.setBrush(QColor(255,255,255));
                        break;
                    case 1:
                        painter.setBrush(QColor(251,188,93));
                        break;
                    case 2:
                        painter.setBrush(QColor(223,112,79));
                        break;
                    case 3:
                        painter.setBrush(QColor(168,87,91));
                        break;
                    case 4:
                        painter.setBrush(QColor(120,55,67));
                        break;
                    case 5:
                        painter.setBrush(QColor(109,85,118));
                        break;
                    case 6:
                        painter.setBrush(QColor(117, 145, 224));
                        break;
                    case 7:
                        painter.setBrush(QColor(196,196,167));
                        break;
                    case 8:
                        painter.setBrush(QColor(194, 165, 240));
                        break;
                }
                if (boardlogic->NextPiece.PieceArray[i][j]) painter.drawRect(520 + (80 - boardlogic->GetPieceWidth(boardlogic->NextPiece) / 2) + j * 19, 210 + (80 - boardlogic->GetPieceHeight(boardlogic->NextPiece) / 2) + i * 19, 19, 19);
                }
        }
}

void MainWindow::on_startButton_clicked()
{
    if (!timer->isActive()) {
        if (boardlogic->dropping)
            timer->start(50);
        else
            timer->start(speed);
    }
}
void MainWindow::on_pauseButton_clicked()
{
    timer->stop();
}
void MainWindow::on_stopButton_clicked()
{
    timer->stop();
    boardlogic = new Logic(this);
    ui->lcdNumber->display(0);
    speed = 650;
    this->repaint();
}
void MainWindow::tmr_game()
{
    boardlogic->DropPiece();
    ui->lcdNumber->display(boardlogic->score);
    this->repaint();
}
void MainWindow::reset_speed()
{
    timer->setInterval(speed);
}
void MainWindow::increase_speed()
{
    speed *= 0.95;
    timer->setInterval(speed);
}
void MainWindow::game_over()
{
    timer->stop();
    QMessageBox * msg = new QMessageBox(this);
    msg->setText("Игра окончена!\nСчет: " + QString::number(ui->lcdNumber->value()));
    msg->show();
    on_stopButton_clicked();
}
void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (timer->isActive()) {
    switch(e->key())
    {
    case Qt::Key_Left:
        if (!boardlogic->dropping)
            if (boardlogic->PieceCanGoLeft()) boardlogic->CurrentPiece.X--;
        break;
    case Qt::Key_Right:
        if (!boardlogic->dropping)
            if (boardlogic->PieceCanGoRight()) boardlogic->CurrentPiece.X++;
         break;
    case Qt::Key_Up:
        if (!boardlogic->dropping)
            if (boardlogic->PieceCanRotate(boardlogic->rot < 4 ?boardlogic->rot + 1 : 1))
            {
            if (boardlogic->rot < 4)
                boardlogic->rot++;
                else boardlogic->rot = 1;
                boardlogic->ReloadPiece();
            }
         break;
    case Qt::Key_Down:
        if (!boardlogic->dropping)
        {
            boardlogic->dropping = true;
            timer->setInterval(20);
        }
        break;
    }
    this->repaint();
    }
}
void MainWindow::keyReleaseEvent(QKeyEvent *e) {
    if ( timer -> isActive() ) {
        switch( e -> key() ) {
            case Qt::Key_Down:
            if (boardlogic->dropping)
            {
                boardlogic->dropping = false;
                timer->setInterval(speed);
            }
            break;
        }
    }
}
void MainWindow::on_action_triggered()
{
}
void MainWindow::on_action_2_triggered()
{
    exit(0);
}
