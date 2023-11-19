#include <QApplication>

#include "tetris.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  Tetris tetris;
  tetris.show();
  return a.exec();
}
