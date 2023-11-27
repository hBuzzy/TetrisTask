#ifndef PREVIEWWINDOW_H
#define PREVIEWWINDOW_H

#include <QWidget>
#include "figure.h"

class PreviewWindow : public QWidget {
  Q_OBJECT

  Q_PROPERTY(uint rowsNumber READ GetRowsNumber WRITE SetRowsNumber NOTIFY
                   RowsNumberChanged FINAL)
  Q_PROPERTY(uint columnsNumber READ GetColumnsNumber WRITE SetColumnNumber
                   NOTIFY ColumnsNumberChanged FINAL)

 public:
 explicit PreviewWindow(QWidget *parent = nullptr);

 signals:
  void RowsNumberChanged();
  void ColumnsNumberChanged();
  void InitialisationStarted();

 public slots:
  void ResetCellsColor();
  void StartNewGame(Figure nextFigure);

 private slots:
  void SetCells();

 protected:
  void paintEvent(QPaintEvent *event);
  QSize GetSize() const;
  void GamePlay();
  static constexpr uint kCellSize = 30;

 public:
  void DrawCells(QPainter *painter);
  uint GetRowsNumber() const;
  void SetRowsNumber(uint newRowsNumber);

  uint GetColumnsNumber() const;
  void SetColumnNumber(uint newColumnsCount);

 private:
  uint rowsNumber_ = 0;
  uint columnsNumber_ = 0;
  QVector<QVector<QColor>> cellsColors_;
  uint rowsCount_ = 0;
  uint columnsCount_ = 0;
  const QColor kCellDefaultColor = QColor(150, 150, 150);
  Figure currFigure_;
  Figure nextFigure_;
};

#endif
