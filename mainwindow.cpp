#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadUI(){
    ui->play_button->setFont(QFont(default_font_family, font_size));
}

void MainWindow::on_play_button_clicked()
{
    this->close();
    Game *game = new Game();
    game->show();
}
