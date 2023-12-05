#include "gameoverdialog.h"

GameOverDialog::GameOverDialog(QWidget *parent) : QDialog{parent} {
    QVBoxLayout *layout = new QVBoxLayout(this);

    label_ = new QLabel("Вы проиграли!", this);
    layout->addWidget(label_);

    newTryButton_ = new QPushButton("Попробуйте еще раз!", this);
    connect(newTryButton_, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(newTryButton_);

    setLayout(layout);
}
