#include "modaldialog.h"

ModalDialog::ModalDialog(QWidget *parent) : QDialog{parent} {
    QVBoxLayout *layout = new QVBoxLayout(this);

    label = new QLabel("ИГРА НА ПАУЗЕ", this);
    layout->addWidget(label);

    okButton = new QPushButton("ПРОДОЛЖИТЬ", this);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(okButton);

    setLayout(layout);
}
