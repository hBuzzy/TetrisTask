#include "tetris.h"

tetris::tetris(QWidget *parent): QWidget(parent), grid(new GridDrawer()), figure_(new figure())
{
    int maximumHeightFigure = 4;
    gameOn = 1;
    grid->setColumns(7);
    grid->setRows(10);

    figure_->setData();
    figure_->setColor();
    int cond = figure_->condition();
    int x = (grid->columns() + 2 - figure_->data()[cond].size())/2;
    figure_->setX(x);
    int y = 2;
    if(figure_->data()[cond][0].size() == maximumHeightFigure){
        y = 0;
    }
    figure_->setY(y);
    qDebug() << x << " " << y;

    for(int i = 0; i < grid->columns()+2; i++){
        QVector<int> vectorInt;
        gridInt.push_back(vectorInt);
        for(int j = 0; j < grid->rows()+5; j++){
            if(i == 0 || i == grid->columns() + 1 || j == grid->rows() + 4){
                gridInt[i].push_back(1);
            }else{
                gridInt[i].push_back(0);
            }
        }
    }

    for(int i = 0; i < figure_->data()[cond].size(); i++){
        for(int j = 0; j < figure_->data()[cond][i].size(); j++){
            if(figure_->data()[cond][i][j]){
                gridInt[i + x][j + y] = 2;
            }
        }
    }

    for(int i = 0; i < grid->columns()+2; i++){
        qDebug() << gridInt[i];
    }

    QVector<QVector<QColor>> cellColors = grid->getCellColors();

    for(int i = 1; i < grid->columns()+1; i++){
        for(int j = maximumHeightFigure; j < grid->rows()+maximumHeightFigure; j++){
            if(gridInt[i][j] == 0){
                cellColors[i-1][j-maximumHeightFigure] = Qt::black;
            }
            if(gridInt[i][j] == 1){
                cellColors[i-1][j-maximumHeightFigure] = Qt::gray;
            }
            if(gridInt[i][j] == 2){
                cellColors[i-1][j-maximumHeightFigure] = figure_->color();
            }
        }
    }
    grid->setCellColors(cellColors);


    timer_ = new QTimer();
    connect(timer_, &QTimer::timeout, this, &tetris::TikTime);
    timer_->start(1000);
}

GridDrawer *tetris::getGridDrawer()
{
    return grid;
}

void tetris::checkRows()
{
    for(int j = grid->rows() + 3; j >= 0; j--){
        bool ind = true;
        for(int i = 0; i < grid->columns()+2; i++){
            if(gridInt[i][j] == 0){
                ind = false;
                break;
            }
        }
        if(ind){
            emit fullRow();
            for(int i = 0; i < grid->columns()+2; i++){
                gridInt[i].remove(j);
                if(i == 0 || i == grid->columns()+1){
                    gridInt[i].push_front(1);
                }else{
                    gridInt[i].push_front(0);
                }
            }
            j++;
        }
    }
}

int tetris::checkLastRow()
{
    int maximumHeightFigure = 4;
    for(int i = 1; i < grid->columns()+1; i++){
        if(gridInt[i][maximumHeightFigure] != 0){
            gameOn = 0;
            emit gameFinished();
            return 0;
        }
    }
    return 1;
}

void tetris::newGame()
{
    int maximumHeightFigure = 4;
    qDebug() << "new game";
    for(int i = 1; i < grid->columns()+1; i++){
        for(int j = 0; j < grid->rows()+maximumHeightFigure; j++){
            gridInt[i][j] = 0;
        }
    }

    emit nextStep();
    figure_->setData();
    figure_->setColor();
    figure_->setCondition(0);
    int cond = figure_->condition();
    int x = (grid->columns() + 2 - figure_->data()[cond].size())/2;
    figure_->setX(x);
    int y = 2;
    if(figure_->data()[cond][0].size() == maximumHeightFigure){
        y = 0;
    }
    figure_->setY(y);

    for(int i = 0; i < figure_->data()[cond].size(); i++){
        for(int j = 0; j < figure_->data()[cond][i].size(); j++){
            if(figure_->data()[cond][i][j]){
                gridInt[i + x][j + y] = 2;
            }
        }
    }

    updateCellColors();
    gameOn = 1;
}

void tetris::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    grid->draw(&painter);
    this->update();
}

void tetris::updateCellColors(){
    QVector<QVector<QColor>> cellColors = grid->getCellColors();
    int maximumHeightFigure = 4;
    for(int i = 1; i < grid->columns()+1; i++){
        for(int j = maximumHeightFigure; j < grid->rows()+maximumHeightFigure; j++){
            if(gridInt[i][j] == 0){
                cellColors[i-1][j-maximumHeightFigure] = Qt::black;
            }
            if(gridInt[i][j] == 1){
                cellColors[i-1][j-maximumHeightFigure] = Qt::gray;
            }
            if(gridInt[i][j] == 2){
                cellColors[i-1][j-maximumHeightFigure] = figure_->color();
            }
        }
    }
    grid->setCellColors(cellColors);
}

bool tetris::moveFigureDown(){
    int maximumHeightFigure = 4;
    bool ind = true;
    QVector<QVector<int>> tmpGridInt = gridInt;
    for(int j = grid->rows()+maximumHeightFigure; j > 0; j--){
        if(ind){
            for(int i = 0; i < grid->columns()+2; i++){
                if(gridInt[i][j-1] != 1 && gridInt[i][j-1] != 0 && gridInt[i][j] == 1){
                    ind = false;
                    break;
                }else{
                    if(gridInt[i][j] != 1 && gridInt[i][j-1] != 1){
                        tmpGridInt[i][j] = gridInt[i][j-1];
                        tmpGridInt[i][j-1] = 0;
                    }
                }
            }
        }else{
            break;
        }
    }
    if(ind){
        gridInt = tmpGridInt;
        figure_->setY(figure_->y() + 1);
    }else{
        checkRows();
        qDebug() << checkLastRow();
//      перекраска в серый цвет
        for(int i = 0; i < grid->columns()+2; i++){
                for(int j = 0; j < grid->rows()+5; j++){
                    if(gridInt[i][j] > 1){
                        gridInt[i][j] = 1;
                    }
                }
            }
        if(checkLastRow()){
            emit nextStep();
            figure_->setCondition(0);
            int cond = figure_->condition();
            int x = (grid->columns() + 2 - figure_->data()[cond].size())/2;
            figure_->setX(x);
            int y = 2;
            if(figure_->data()[cond][0].size() == maximumHeightFigure){
                y = 0;
            }
            figure_->setY(y);

            for(int i = 0; i < figure_->data()[cond].size(); i++){
                for(int j = 0; j < figure_->data()[0][i].size(); j++){
                    if(figure_->data()[cond][i][j]){
                        gridInt[i + x][j + y] = 2;
                    }
                }
            }

            for(int i = 0; i < grid->columns()+2; i++){
                qDebug() << gridInt[i];
            }
        }
    }
    updateCellColors();
    return ind;
}

GridDrawer *tetris::getGrid()
{
    return grid;
}

void tetris::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "Key event";

    if (event->key() == Qt::Key_Down){
        qDebug() << "Key Down Pressed";
        moveFigureDown();
    }

    if(event->key() == Qt::Key_Space){
        qDebug() << "Key Space Pressed";
        bool ind = true;
        while(ind && gameOn){
            ind = moveFigureDown();
            qDebug() << ind;
        }
    }

    if (event->key() == Qt::Key_Left){
        qDebug() << "Key Right Pressed";
        bool ind = true;
        QVector<QVector<int>> tmpGridInt = gridInt;
        for(int i = 1; i < grid->columns()+2; i++){
            if(ind){
                for(int j = 0; j < grid->rows() + 5; j++){
                    if(gridInt[i][j] != 1 && gridInt[i][j] != 0 && gridInt[i-1][j] == 1){
                        ind = false;
                        break;
                    }else{
                        if(gridInt[i][j] != 1 && gridInt[i-1][j] != 1){
                            tmpGridInt[i-1][j] = gridInt[i][j];
                            tmpGridInt[i][j] = 0;
                        }
                    }
                }
            }else{
                break;
            }
        }
        if(ind){
            gridInt = tmpGridInt;
            figure_->setX(figure_->x() - 1);
        }
        updateCellColors();
    }

    if (event->key() == Qt::Key_Right){
        qDebug() << "Key Right Pressed";
        bool ind = true;
        QVector<QVector<int>> tmpGridInt = gridInt;
        for(int i = grid->columns()+1; i >0; i--){
            if(ind){
                for(int j = 0; j < grid->rows() + 5; j++){
                    if(gridInt[i-1][j] != 1 && gridInt[i-1][j] != 0 && gridInt[i][j] == 1){
                        ind = false;
                        break;
                    }else{
                        if(gridInt[i][j] != 1 && gridInt[i-1][j] != 1){
                            tmpGridInt[i][j] = gridInt[i-1][j];
                            tmpGridInt[i-1][j] = 0;
                        }
                    }
                }
            }else{
                break;
            }
        }
        if(ind){
            gridInt = tmpGridInt;
            figure_->setX(figure_->x() + 1);
        }
        updateCellColors();
    }
    for(int i = 0; i < grid->columns()+2; i++){
        qDebug() << gridInt[i];
    }

    if (event->key() == Qt::Key_Up){
        QVector<QVector<int>> tmpGridInt = gridInt;
        for(int i = grid->columns()+1; i >0; i--){
            for(int j = 0; j < grid->rows() + 5; j++){
                if(tmpGridInt[i][j] > 1){
                    tmpGridInt[i][j] = 0;
                }
            }
        }
        figure_->nextCondition();
        int cond = figure_->condition();
        bool ind = 1;
        for(int i = 0; i < figure_->data()[cond].size(); i++){
            if(ind){
                for(int j = 0; j < figure_->data()[cond][i].size(); j++){
                    if(figure_->data()[cond][i][j] && tmpGridInt[i+figure_->x()][j+figure_->y()] == 0){
                        tmpGridInt[i+figure_->x()][j+figure_->y()] = 2;
                    }else{
                        if(figure_->data()[cond][i][j] && tmpGridInt[i+figure_->x()][j+figure_->y()]){
                            ind = false;
                            break;
                        }
                    }
                }
            }
        }
        if(ind){
            gridInt = tmpGridInt;
            updateCellColors();
        }else{
            figure_->prevCondition();
        }
    }
    this->update();
}

void tetris::TikTime()
{
    moveFigureDown();
}
