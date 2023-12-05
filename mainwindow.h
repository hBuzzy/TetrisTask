#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include "tetrisgrid.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupGame();
    int score_;
    bool isGameOverHandled_ = false;
    TetrisGrid *tetrisGrid_;
    QPushButton *startButton_;
    QPushButton *exitButton_;
    QPushButton *helpButton_;
    QLabel *scoreLabel_;

private slots:
    void startGame();
    void exitGame();
    void updateScore(int newScore);
    void restartGame();
    void showHelp();

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H
