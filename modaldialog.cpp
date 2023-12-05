#include "modaldialog.h"

ModalDialog::ModalDialog(QWidget *parent) : QDialog{parent} {
    QVBoxLayout *layout = new QVBoxLayout(this);

    label_ = new QLabel("ИГРА НА ПАУЗЕ", this);
    layout->addWidget(label_);

    okButton_ = new QPushButton("ПРОДОЛЖИТЬ", this);
    connect(okButton_, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(okButton_);

    setLayout(layout);
}
