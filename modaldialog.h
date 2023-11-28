#ifndef MODALDIALOG_H
#define MODALDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

class ModalDialog : public QDialog {
    Q_OBJECT
public:
    explicit ModalDialog(QWidget *parent = nullptr);

private:

    QPushButton *okButton;
    QLabel *label;

};

#endif // MODALDIALOG_H
