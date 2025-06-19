#ifndef PLAYERVSPLAYERWINDOW_H
#define PLAYERVSPLAYERWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QMessageBox>
#include "Game.h"
#include "History.h"

class PlayerVsPlayerWindow : public QMainWindow {
    Q_OBJECT

public:
    PlayerVsPlayerWindow(const QString& username, QWidget* parent = nullptr);

private slots:
    void handleCellClick();
    void updateBoard();
    void newGame();
    void showHistory();
    void goBack();

private:
    void setupUI();
    void applyStyles();
    void logResult(const QString& result);
    void showGameResult(const QString& result);
    void enableBoard(bool enabled);
    void handleGameEnd();
    void updatePlayerTurn();

    QPushButton* cells[3][3];
    QLabel* statusLabel;
    QLabel* scoreLabel;
    QLabel* playerLabel;
    QLabel* currentPlayerLabel;
    QPushButton* newGameBtn;
    QPushButton* historyBtn;
    QPushButton* backBtn;
    QFrame* gameFrame;
    QFrame* controlFrame;

    Game game;
    QString user;
    History history;

    int player1Wins;  // X wins
    int player2Wins;  // O wins
    int draws;

    QString player1Name;  // Always X
    QString player2Name;  // Always O
};

#endif // PLAYERVSPLAYERWINDOW_H
