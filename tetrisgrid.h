#ifndef TETRISGRID_H
#define TETRISGRID_H

#include <QWidget>
#include <QVector>
#include <QTimer>

class TetrisGrid : public QWidget {
    Q_OBJECT
    Q_PROPERTY(int rows READ rows WRITE setRows NOTIFY rowsChanged)
    Q_PROPERTY(int columns READ columns WRITE setColumns NOTIFY columnsChanged)
public:
    int rows() const;
    int columns() const;
    bool isValidMove(const QVector<QVector<int>> &shape, int rows, int columns);
    void lockShape();
    void handleKeyPressEvent(QKeyEvent *event);
    void setupGame();
    TetrisGrid(QWidget *parent = nullptr);
    QColor generateRandomColor();

public slots:
    void setRows(int rows);
    void setColumns(int columns);
    void startGame();
    void dropShape();
    void updateScore(int newScore);
    void moveShapeLeft();
    void moveShapeRight();
    void rotateShape();
    void speedFigure();

signals:
    void rowsChanged(int rows);
    void columnsChanged(int columns);
    void scoreChanged(int newScore);
    void gameOver();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void drawGrid(QPainter &painter, int cellSize);
    void drawShape(QPainter &painter, int cellSize);
    void drawNextShape(QPainter &painter, int cellSize);
    void generateNewShape();
    void checkLines();
    void addCurrentShapeToGridNew();
    bool isCollision();
    QVector<QVector<int>> mGrid_;
    QVector<QVector<int>> currentShape_;
    QVector<QVector<int>> nextShape_;
    QTimer *timer_;
    int mRows_;
    int mColumns_;
    int shapeRow_;
    int shapeColumn_;
    int score_;
    int speed_ = 500;
};

#endif // TETRISGRID_H
