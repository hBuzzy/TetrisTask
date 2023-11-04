#include <QApplication>

#include "src/tetris/tetris.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Tetris tetris;
    tetris.show();
    return QApplication::exec();
}
