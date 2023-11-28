#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <QWidget>

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
    uint GetRowsNumber() const;
    void SetRowsNumber(uint newRowsNumber);

    uint GetColumnsNumber() const;
    void SetColumnNumber(uint newColumnsCount);

    void SetFigurePosition(int row, int column);

    void spawnNextFigure();
    void updateGameGrid();

    bool CheckCollision();

    bool CheckCollisionMoveLeft();
    bool CheckCollisionMoveRight();
    bool CheckCollisionRotate();

    QVector<QVector<int>> GetCurrentFigure();
    QVector<QVector<int>> GetRotateCurrentFigure();

    bool CheckFullLine(int row);
    bool CheckFullColumn(int column);


    void CheckLine();
    void CheckColumn();

    void RemoveFullLine(int row);

    int GetWinPoints();

    void moveFigure();

    uint GetCurrentFigureRow();
    uint GetCurrentFigureColumn();

    void clearGameGrid();
    void SetWinPoints(int points);


protected:
    void paintEvent(QPaintEvent *event);

 private:
    uint rowsNumber_ = 0;
    uint columnsNumber_ = 0;
    int currentRow_ = 0;
    QVector<QVector<int>> gridGame;

    int blockSize = 30;

    int WinPoints = 0;

    bool isLineFull = true;
    bool isGameOver = false;

    QVector<QVector<int>> currentFigure_;

    QVector<QVector<int>> GameGrid_; // Поле с информацией о заполнении поля фигурами

    uint currentFigureRow_;
    uint currentFigureColumn_;

    QColor colorFigure;

};

#endif  // GAMEFIELD_H
