#include "gameoverdialog.h"

GameOverDialog::GameOverDialog(QWidget *parent) : QDialog{parent} {
    QVBoxLayout *layout = new QVBoxLayout(this);

    label = new QLabel("Вы проиграли!", this);
    layout->addWidget(label);

    newTryButton = new QPushButton("Попробуйте еще раз!", this);
    connect(newTryButton, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(newTryButton);

    setLayout(layout);
}
