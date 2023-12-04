#include "tetrisform.h"
#include "ui_tetrisform.h"

tetrisForm::tetrisForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tetrisForm)
{
    ui->setupUi(this);
}

tetrisForm::~tetrisForm()
{
    delete ui;
}
