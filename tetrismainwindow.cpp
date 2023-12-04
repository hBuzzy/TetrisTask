#include "tetrismainwindow.h"
#include "ui_tetrismainwindow.h"

TetrisMainWindow::TetrisMainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TetrisMainWindow)
{
    ui->setupUi(this);
}

TetrisMainWindow::~TetrisMainWindow()
{
    delete ui;
}

