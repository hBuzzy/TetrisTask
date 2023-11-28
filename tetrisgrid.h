#ifndef TETRISGRID_H
#define TETRISGRID_H

#include <QWidget>
#include <QVector>
#include <QTimer>

class TetrisGrid : public QWidget {
    Q_OBJECT
    Q_PROPERTY(int rows READ rows WRITE setRows NOTIFY rowsChanged)
    Q_PROPERTY(int cols READ cols WRITE setCols NOTIFY colsChanged)
public:
    TetrisGrid(QWidget *parent = nullptr);
    int rows() const;
    int cols() const;
    bool isValidMove(const QVector<QVector<int>> &shape, int rows, int cols);
    void lockShape();
    void handleKeyPressEvent(QKeyEvent *event);
    void setupGame();
    QColor generateRandomColor();

public slots:
    void setRows(int rows);
    void setCols(int cols);
    void startGame();
    void dropShape();
    void updateScore(int newScore);
    void moveShapeLeft();
    void moveShapeRight();
    void rotateShape();
    void speedFigure();

signals:
    void rowsChanged(int rows);
    void colsChanged(int cols);
    void scoreChanged(int newScore);
    void gameOver();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    int m_rows_;
    int m_cols_;
    QVector<QVector<int>> m_grid_;
    QTimer *timer_;
    QVector<QVector<int>> currentShape_;
    QVector<QVector<int>> nextShape_;
    QVector<QVector<int>> m_gridNew_;
    int shapeRow_;
    int shapeCol_;
    int score_;
    int speed_ = 500;
    void drawGrid(QPainter &painter, int cellSize);
    void drawShape(QPainter &painter, int cellSize);
    void generateNewShape();
    void checkLines();
    void addCurrentShapeToGridNew();
    bool isCollision();
    bool locking_;
    bool shapeLocked_;
};

#endif // TETRISGRID_H
