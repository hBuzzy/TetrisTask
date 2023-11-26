#include "tetriscontroller.h"

#include <QDebug>

#include "tetrismodel.h"
#include "tetrisview.h"

TetrisController::TetrisController(TetrisModel* model, TetrisView* view,
                                   QObject* parent)
    : QObject(parent), model_(model), view_(view) {
  connect(&timer_, SIGNAL(timeout()), SLOT(OnStep()));
}

void TetrisController::Start() {
  model_->ResetGame();
  Resume();
}

void TetrisController::OnStep() {
  model_->DoStep();
  view_->Refresh();
  if (model_->IsThisGameOver()) {
    qDebug() << model_->GetScore();
    model_->ResetGame();
  }
}

void TetrisController::Pause() { timer_.stop(); }

void TetrisController::Resume() { timer_.start(startSpeed); }

void TetrisController::MoveLeft() { InAction(&TetrisModel::MoveItemLeft); }

void TetrisController::MoveRight() { InAction(&TetrisModel::MoveItemRight); }

void TetrisController::Rotate() { InAction(&TetrisModel::RotateItem); }

void TetrisController::DropEnabled(bool enabled) {
  InAction(enabled ? &TetrisModel::StartDrop : &TetrisModel::StopDrop);
}

void TetrisController::TogglePause() { timer_.isActive() ? Pause() : Resume(); }

void TetrisController::InAction(void (TetrisModel::*action)()) {
  if (!timer_.isActive()) {
    return;
  }

  (model_->*action)();
  view_->Refresh();
}
