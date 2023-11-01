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
  void InitialisationStarted();

 private slots:
  void SetCells();

 public:
  uint GetRowsNumber() const;
  void SetRowsNumber(uint newRowsNumber);

  uint GetColumnsNumber() const;
  void SetColumnNumber(uint newColumnsCount);

 private:
  uint rowsNumber_ = 0;
  uint columnsNumber_ = 0;
};

#endif  // GAMEFIELD_H
