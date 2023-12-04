#ifndef TETRISMAINWINDOW_H
#define TETRISMAINWINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class TetrisMainWindow; }
QT_END_NAMESPACE

class TetrisMainWindow : public QWidget
{
    Q_OBJECT

public:
    TetrisMainWindow(QWidget *parent = nullptr);
    ~TetrisMainWindow();

private:
    Ui::TetrisMainWindow *ui;
    QLabel *createLabel(const QString &text);

    TetrisBoard *board;
    QLabel *nextPieceLabel;
    QLCDNumber *scoreLcd;
    QLCDNumber *levelLcd;
    QLCDNumber *linesLcd;
    QPushButton *startButton;
    QPushButton *quitButton;
    QPushButton *pauseButton;
};
#endif // TETRISMAINWINDOW_H
