#ifndef FIGURE_H
#define FIGURE_H

#include <QObject>
#include <QVector>

class Figure : public QObject
{
    Q_OBJECT
public:
    explicit Figure(QObject *parent = nullptr);
    Figure(const Figure& other);
    Figure& operator=(const Figure& other);

    int getHeight() const;
    int getWidth() const;
    int getShapeAt(int row, int col) const;

    void generate();
    void rotate();

private:
    void copyFrom(const Figure& other);

private:
    QVector<QVector<int>> shape_;
    static const QVector<QVector<int>> I_;
    static const QVector<QVector<int>> J_;
    static const QVector<QVector<int>> L_;
    static const QVector<QVector<int>> O_;
    static const QVector<QVector<int>> S_;
    static const QVector<QVector<int>> T_;
    static const QVector<QVector<int>> Z_;

signals:

};

#endif // FIGURE_H
