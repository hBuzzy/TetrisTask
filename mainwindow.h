#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include "tetrisgrid.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    TetrisGrid *tetrisGrid;
    QPushButton *startButton;
    QPushButton *exitButton;
    QPushButton *helpButton;
    QLabel *scoreLabel;
    void setupGame();
    int score_;
    bool gameOverHandled_ = false;

private slots:
    void startGame();
    void exitGame();
    void updateScore(int newScore);
    void restartGame();
    void showHelp();

public slots:
    void keyPressEvent(QKeyEvent *event);


};

#endif // MAINWINDOW_H
