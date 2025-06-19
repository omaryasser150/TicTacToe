#include "MainWindow.h"
#include "GameModeWindow.h"
#include <QGridLayout>
#include <QMessageBox>
#include <QDateTime>
#include <QTimer>
#include <QApplication>
#include <QStyle>

MainWindow::MainWindow(const QString& username, QWidget* parent)
    : QMainWindow(parent), ai(Player::O), user(username), history(username.toStdString()),
    playerWins(0), aiWins(0), draws(0) {

    setWindowTitle("Tic Tac Toe - Playing as " + username);
    setFixedSize(600, 700);
    setupUI();
    applyStyles();
    updateBoard();
}

void MainWindow::setupUI()
{
    QWidget* central = new QWidget;
    setCentralWidget(central);

    QVBoxLayout* mainLayout = new QVBoxLayout(central);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Control Frame
    controlFrame = new QFrame();
    controlFrame->setObjectName("controlFrame");
    controlFrame->setFixedHeight(120);

    QVBoxLayout* controlLayout = new QVBoxLayout(controlFrame);
    controlLayout->setSpacing(10);
    controlLayout->setContentsMargins(20, 15, 20, 15);

    // Player info and status
    QHBoxLayout* topControlLayout = new QHBoxLayout();

    playerLabel = new QLabel("Player: " + user + " (X)");
    playerLabel->setObjectName("playerLabel");

    statusLabel = new QLabel("Your turn!");
    statusLabel->setObjectName("statusLabel");
    statusLabel->setAlignment(Qt::AlignCenter);

    topControlLayout->addWidget(playerLabel);
    topControlLayout->addStretch();
    topControlLayout->addWidget(statusLabel);

    // Score display
    scoreLabel = new QLabel("Wins: 0 | AI: 0 | Draws: 0");
    scoreLabel->setObjectName("scoreLabel");
    scoreLabel->setAlignment(Qt::AlignCenter);

    // Control buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    newGameBtn = new QPushButton("🔄 NEW GAME");
    newGameBtn->setObjectName("controlButton");
    newGameBtn->setFixedHeight(35);

    historyBtn = new QPushButton("📊 HISTORY");
    historyBtn->setObjectName("controlButton");
    historyBtn->setFixedHeight(35);

    backBtn = new QPushButton("← BACK");
    backBtn->setObjectName("backButton");
    backBtn->setFixedHeight(35);

    buttonLayout->addWidget(newGameBtn);
    buttonLayout->addWidget(historyBtn);
    buttonLayout->addStretch();
    buttonLayout->addWidget(backBtn);

    controlLayout->addLayout(topControlLayout);
    controlLayout->addWidget(scoreLabel);
    controlLayout->addLayout(buttonLayout);

    // Game Frame
    gameFrame = new QFrame();
    gameFrame->setObjectName("gameFrame");

    QGridLayout* gameLayout = new QGridLayout(gameFrame);
    gameLayout->setSpacing(8);
    gameLayout->setContentsMargins(20, 20, 20, 20);

    // Create game cells
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            cells[r][c] = new QPushButton("");
            cells[r][c]->setObjectName("gameCell");
            cells[r][c]->setFixedSize(120, 120);
            gameLayout->addWidget(cells[r][c], r, c);
            connect(cells[r][c], &QPushButton::clicked, this, &MainWindow::handleCellClick);
        }
    }

    // Add to main layout
    mainLayout->addWidget(controlFrame);
    mainLayout->addWidget(gameFrame);

    // Connect signals
    connect(newGameBtn, &QPushButton::clicked, this, &MainWindow::newGame);
    connect(historyBtn, &QPushButton::clicked, this, &MainWindow::showHistory);
    connect(backBtn, &QPushButton::clicked, this, &MainWindow::goBack);
}

void MainWindow::applyStyles()
{
    setStyleSheet(R"(
        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #667eea, stop:1 #764ba2);
        }

        #controlFrame {
            background: rgba(255, 255, 255, 0.95);
            border-radius: 15px;
            border: 2px solid rgba(255, 255, 255, 0.3);
        }

        #gameFrame {
            background: rgba(255, 255, 255, 0.95);
            border-radius: 15px;
            border: 2px solid rgba(255, 255, 255, 0.3);
        }

        #playerLabel {
            font-size: 16px;
            font-weight: bold;
            color: #2c3e50;
        }

        #statusLabel {
            font-size: 18px;
            font-weight: bold;
            color: #e74c3c;
            background: rgba(231, 76, 60, 0.1);
            padding: 8px 15px;
            border-radius: 15px;
        }

        #scoreLabel {
            font-size: 14px;
            color: #7f8c8d;
            font-weight: 500;
        }

        #controlButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #74b9ff, stop:1 #0984e3);
            color: white;
            border: none;
            border-radius: 17px;
            font-size: 12px;
            font-weight: bold;
            padding: 0 15px;
        }

        #controlButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #0984e3, stop:1 #0770d1);
        }

        #backButton {
            background: rgba(149, 165, 166, 0.2);
            color: #2c3e50;
            border: 1px solid rgba(149, 165, 166, 0.4);
            border-radius: 17px;
            font-size: 12px;
            font-weight: bold;
            padding: 0 15px;
        }

        #backButton:hover {
            background: rgba(149, 165, 166, 0.3);
        }

        #gameCell {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #f8f9fa, stop:1 #e9ecef);
            border: 3px solid #dee2e6;
            border-radius: 15px;
            font-size: 48px;
            font-weight: bold;
            color: #495057;
        }

        #gameCell:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #e3f2fd, stop:1 #bbdefb);
            border: 3px solid #2196f3;
            transform: scale(1.02);
        }

        #gameCell:pressed {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #bbdefb, stop:1 #90caf9);
        }

        #gameCell[text="X"] {
            color: #e74c3c;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #ffebee, stop:1 #ffcdd2);
        }

        #gameCell[text="O"] {
            color: #3498db;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #e3f2fd, stop:1 #bbdefb);
        }
    )");
}

void MainWindow::handleCellClick() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    int r = -1, c = -1;

    // Find clicked cell position
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (cells[i][j] == btn) {
                r = i; c = j;
                break;
            }
        }
    }

    // Make player move
    if (!game.makeMove(r, c)) return;

    updateBoard();
    statusLabel->setText("AI thinking...");
    statusLabel->setStyleSheet("color: #f39c12; background: rgba(243, 156, 18, 0.1);");

    QApplication::processEvents(); // Update UI immediately

    // Check for game end after player move
    if (game.getWinner() != Player::NONE || game.isDraw()) {
        QTimer::singleShot(500, [this]() {
            handleGameEnd();
        });
        return;
    }

    // Disable board during AI turn
    enableBoard(false);

    // AI makes move after short delay
    QTimer::singleShot(800, [this]() {
        auto move = ai.findBestMove(game);
        game.makeMove(move.first, move.second);
        updateBoard();

        statusLabel->setText("Your turn!");
        statusLabel->setStyleSheet("color: #e74c3c; background: rgba(231, 76, 60, 0.1);");

        enableBoard(true);

        if (game.getWinner() != Player::NONE || game.isDraw()) {
            handleGameEnd();
        }
    });
}

void MainWindow::handleGameEnd() {
    enableBoard(false);

    QString result;
    if (game.getWinner() == Player::X) {
        result = "You win! 🎉";
        playerWins++;
        statusLabel->setText("You won!");
        statusLabel->setStyleSheet("color: #00b894; background: rgba(0, 184, 148, 0.1);");
    } else if (game.getWinner() == Player::O) {
        result = "AI wins! 🤖";
        aiWins++;
        statusLabel->setText("AI won!");
        statusLabel->setStyleSheet("color: #e74c3c; background: rgba(231, 76, 60, 0.1);");
    } else {
        result = "It's a draw! 🤝";
        draws++;
        statusLabel->setText("Draw!");
        statusLabel->setStyleSheet("color: #f39c12; background: rgba(243, 156, 18, 0.1);");
    }

    // Update score display
    scoreLabel->setText(QString("Wins: %1 | AI: %2 | Draws: %3")
                            .arg(playerWins).arg(aiWins).arg(draws));

    logResult(result);

    // Show result after delay
    QTimer::singleShot(1500, [this, result]() {
        showGameResult(result);
    });
}

void MainWindow::updateBoard() {
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            Player p = game.at(r, c);
            QString text = (p == Player::X) ? "X" : (p == Player::O) ? "O" : "";
            cells[r][c]->setText(text);

            // Update button properties for styling
            if (!text.isEmpty()) {
                cells[r][c]->setProperty("text", text);
                cells[r][c]->style()->unpolish(cells[r][c]);
                cells[r][c]->style()->polish(cells[r][c]);
            }
        }
    }
}

void MainWindow::enableBoard(bool enabled) {
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            if (cells[r][c]->text().isEmpty()) {
                cells[r][c]->setEnabled(enabled);
            }
        }
    }
}

void MainWindow::newGame() {
    game.reset();
    updateBoard();
    enableBoard(true);
    statusLabel->setText("Your turn!");
    statusLabel->setStyleSheet("color: #e74c3c; background: rgba(231, 76, 60, 0.1);");

    // Reset cell styling
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            cells[r][c]->setProperty("text", "");
            cells[r][c]->style()->unpolish(cells[r][c]);
            cells[r][c]->style()->polish(cells[r][c]);
        }
    }
}

void MainWindow::showHistory() {
    auto gameHistory = history.loadHistory();
    QString historyText = "Game History:\n\n";

    if (gameHistory.empty()) {
        historyText += "No games played yet!";
    } else {
        for (const auto& result : gameHistory) {
            historyText += QString("%1: %2\n").arg(QString::fromStdString(result.date))
            .arg(QString::fromStdString(result.result));
        }
    }

    QMessageBox msgBox;
    msgBox.setWindowTitle("Game History");
    msgBox.setText(historyText);
    msgBox.setStyleSheet(R"(
        QMessageBox {
            background: white;
        }
        QMessageBox QLabel {
            color: #2c3e50;
            font-size: 12px;
        }
    )");
    msgBox.exec();
}

void MainWindow::showAbout() {
    QMessageBox::about(this, "About",
                       "Advanced Tic Tac Toe\n\n"
                       "Player: " + user + "\n"
                                    "Playing against AI\n\n"
                                    "Enjoy the game!");
}

void MainWindow::goBack() {
    GameModeWindow *gameModeWindow = new GameModeWindow(user);
    gameModeWindow->show();
    this->close();
}

void MainWindow::logResult(const QString& result) {
    GameResult res;
    res.date = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toStdString();
    res.result = result.toStdString();
    history.saveResult(res);
}

void MainWindow::showGameResult(const QString& result) {
    QMessageBox msgBox;
    msgBox.setWindowTitle("Game Over");
    msgBox.setText(result);
    msgBox.setInformativeText("Would you like to play again?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);

    msgBox.setStyleSheet(R"(
        QMessageBox {
            background: white;
            border-radius: 10px;
        }
        QMessageBox QLabel {
            color: #2c3e50;
            font-size: 14px;
            font-weight: bold;
        }
        QMessageBox QPushButton {
            background: #3498db;
            color: white;
            border: none;
            border-radius: 5px;
            padding: 8px 20px;
            font-weight: bold;
        }
        QMessageBox QPushButton:hover {
            background: #2980b9;
        }
    )");

    int ret = msgBox.exec();
    if (ret == QMessageBox::Yes) {
        newGame();
    }
}
