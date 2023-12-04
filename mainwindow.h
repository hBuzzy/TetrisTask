#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void reset_speed();
    void increase_speed();
    void game_over();

    private slots:
    void on_startButton_clicked();
    void on_pauseButton_clicked();
    void on_stopButton_clicked();
    void on_action_triggered();
    void on_action_2_triggered();

private:
    Ui::MainWindow *ui;
    void paintEvent(QPaintEvent*);
    int speed;
    public slots:
    void tmr_game();
    protected:
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);


};
#endif // MAINWINDOW_H
