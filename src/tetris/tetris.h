#ifndef TETRIS_H
#define TETRIS_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
    class Tetris;
}
QT_END_NAMESPACE

class Tetris final : public QMainWindow {
    Q_OBJECT
public:
    explicit Tetris(QWidget *parent = nullptr);
    ~Tetris() final;
private slots:
    void ShowHelp() const;
private:
    Ui::Tetris *ui;
};

#endif
