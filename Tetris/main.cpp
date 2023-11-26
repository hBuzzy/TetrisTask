#include <QApplication>
#include <QIcon>

#include "tetrisview.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  TetrisView w;
  w.setWindowTitle("Предсессионный тетрис Barbie Edition");
  QIcon ico(":/image/tetrislogo.ico");
  w.setWindowIcon(ico);
  w.show();
  return a.exec();
}
