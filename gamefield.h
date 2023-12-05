#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include "tetramino.h"
#include "nextfiguregamegrid.h"

#include <QWidget>

class Tetris;

class GameField : public QWidget {
  Q_OBJECT
 public:
  explicit GameField(QWidget *parent = nullptr);

    Q_PROPERTY(uint rowsNumber READ GetRowsNumber WRITE SetRowsNumber NOTIFY
                 RowsNumberChanged FINAL)
    Q_PROPERTY(uint columnsNumber READ GetColumnsNumber WRITE SetColumnNumber
                 NOTIFY ColumnsNumberChanged FINAL)

 signals:
    void RowsNumberChanged();
    void ColumnsNumberChanged();
    void FigureSpawned();
    void FigureFalling();
    void CurrentFigureChanged();
    void ChangeWinPoints();
    void GameOver();

 public:
    void SetNextFigureGrid(QVector<QVector<int>> fig);

    uint GetRowsNumber() const;
    void SetRowsNumber(uint newRowsNumber);

    uint GetColumnsNumber() const;
    void SetColumnNumber(uint newColumnsCount);

    void SetFigurePosition(int row, int column);

    void SpawnNextFigure();
    void UpdateGameGrid();

    bool HasCollisionMove(int xO, int yO);

    bool HasCollisionRotation();

    QVector<QVector<int>> GetCurrentFigure();
    QVector<QVector<int>> GetRotateCurrentFigure();

    bool HasFullLine(int row);
    void CheckingLine();

    void RemoveFullLine(int row);

    int GetWinPoints();

    void MoveFigure();

    void GetGameOver();

    uint GetCurrentFigureRow();
    uint GetCurrentFigureColumn();

    void ClearGameGrid();
    void SetWinPoints(int points);

protected:
    void paintEvent(QPaintEvent *event);

 private:
    uint rowsNumber_ = 0;
    uint columnsNumber_ = 0;
    int currentRow_ = 0;

    int blockSize_ = 30;
    int winPoints_ = 0;

    bool isLineFull_ = true;
    bool isGameOver_ = false;

    QVector<QVector<int>> currentFigure_;
    QVector<QVector<int>> nextFigure_;

    QVector<QVector<int>> gameGrid_;

    uint currentFigureRow_;
    uint currentFigureColumn_;

    QColor figureColor_;

    Tetramino *tetraminoGeneratorFirst_;
    NextFigureGameGrid *nextGameGrid_;
};

#endif  // GAMEFIELD_H
