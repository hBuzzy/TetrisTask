#ifndef GAMEOVERDIALOG_H
#define GAMEOVERDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

class GameOverDialog : public QDialog {
    Q_OBJECT
public:
    explicit GameOverDialog(QWidget *parent = nullptr);

private:

    QPushButton *newTryButton;
    QLabel *label;

};

#endif // GAMEOVERDIALOG_H
