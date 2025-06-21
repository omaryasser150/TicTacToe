#include "PlayerVsPlayerWindow.h"
#include "GameModeWindow.h"
#include <QDateTime>
#include <QInputDialog>
#include <QTimer>
#include <QStyle>

PlayerVsPlayerWindow::PlayerVsPlayerWindow(const QString& username, QWidget* parent)
    : QMainWindow(parent), user(username), history(username.toStdString()),
    player1Wins(0), player2Wins(0), draws(0) {

    // Get second player name
    bool ok;
    QString player2 = QInputDialog::getText(this, "Player vs Player",
                                            "Enter Player 2 name:", QLineEdit::Normal,
                                            "Player 2", &ok);
    if (!ok || player2.trimmed().isEmpty()) {
        player2 = "Player 2";
    }

    player1Name = username;  // Player 1 is always X
    player2Name = player2.trimmed();   // Player 2 is always O

    setWindowTitle(QString("Tic Tac Toe - %1 vs %2").arg(player1Name).arg(player2Name));
    setFixedSize(650, 700);  // Increased width for better layout
    setupUI();
    applyStyles();
    updateBoard();
    updatePlayerTurn();
}

void PlayerVsPlayerWindow::setupUI()
{
    QWidget* central = new QWidget;
    setCentralWidget(central);

    QVBoxLayout* mainLayout = new QVBoxLayout(central);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Control Frame
    controlFrame = new QFrame();
    controlFrame->setObjectName("controlFrame");
    controlFrame->setFixedHeight(140);

    QVBoxLayout* controlLayout = new QVBoxLayout(controlFrame);
    controlLayout->setSpacing(10);
    controlLayout->setContentsMargins(20, 15, 20, 15);

    // Player info section - FIXED
    QHBoxLayout* playerInfoLayout = new QHBoxLayout();
    playerInfoLayout->setSpacing(15);

    playerLabel = new QLabel(QString("%1 (X) vs %2 (O)").arg(player1Name).arg(player2Name));
    playerLabel->setObjectName("playerLabel");
    playerLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    currentPlayerLabel = new QLabel(QString("%1's Turn (X)").arg(player1Name));
    currentPlayerLabel->setObjectName("currentPlayerLabel");
    currentPlayerLabel->setAlignment(Qt::AlignCenter);
    currentPlayerLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    currentPlayerLabel->setMinimumWidth(180);
    currentPlayerLabel->setMaximumWidth(200);

    playerInfoLayout->addWidget(playerLabel, 1);
    playerInfoLayout->addWidget(currentPlayerLabel, 0);

    // Status label
    statusLabel = new QLabel("Game started! X goes first.");
    statusLabel->setObjectName("statusLabel");
    statusLabel->setAlignment(Qt::AlignCenter);

    // Score display
    scoreLabel = new QLabel(QString("%1: 0 | %2: 0 | Draws: 0").arg(player1Name).arg(player2Name));
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

    controlLayout->addLayout(playerInfoLayout);
    controlLayout->addWidget(statusLabel);
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
            connect(cells[r][c], &QPushButton::clicked, this, &PlayerVsPlayerWindow::handleCellClick);
        }
    }

    // Add to main layout
    mainLayout->addWidget(controlFrame);
    mainLayout->addWidget(gameFrame);

    // Connect signals
    connect(newGameBtn, &QPushButton::clicked, this, &PlayerVsPlayerWindow::newGame);
    connect(historyBtn, &QPushButton::clicked, this, &PlayerVsPlayerWindow::showHistory);
    connect(backBtn, &QPushButton::clicked, this, &PlayerVsPlayerWindow::goBack);
}

void PlayerVsPlayerWindow::applyStyles()
{
    setStyleSheet(R"(
        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #ff9a9e, stop:1 #fecfef);
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

        #currentPlayerLabel {
            font-size: 14px;
            font-weight: bold;
            color: #e74c3c;
            background: rgba(231, 76, 60, 0.1);
            padding: 6px 12px;
            border-radius: 12px;
            min-width: 160px;
            max-width: 180px;
            border: 1px solid rgba(231, 76, 60, 0.2);
        }

        #statusLabel {
            font-size: 14px;
            color: #7f8c8d;
            font-weight: 500;
        }

        #scoreLabel {
            font-size: 14px;
            color: #7f8c8d;
            font-weight: 500;
        }

        #controlButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #ff6b9d, stop:1 #c44569);
            color: white;
            border: none;
            border-radius: 17px;
            font-size: 12px;
            font-weight: bold;
            padding: 0 15px;
        }

        #controlButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #c44569, stop:1 #a71e4b);
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
                stop:0 #fff0f5, stop:1 #fce4ec);
            border: 3px solid #e91e63;
            transform: scale(1.02);
        }

        #gameCell:pressed {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #fce4ec, stop:1 #f8bbd9);
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

void PlayerVsPlayerWindow::handleCellClick() {
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

    // Make move
    if (!game.makeMove(r, c)) return;

    updateBoard();

    // Check for game end
    if (game.getWinner() != Player::NONE || game.isDraw()) {
        handleGameEnd();
    } else {
        updatePlayerTurn();
    }
}

void PlayerVsPlayerWindow::handleGameEnd() {
    enableBoard(false);

    QString result;
    QString winnerName;

    if (game.getWinner() == Player::X) {
        result = QString("%1 wins! 🎉").arg(player1Name);
        winnerName = player1Name;
        player1Wins++;
        statusLabel->setText(QString("%1 won the game!").arg(player1Name));
        currentPlayerLabel->setText("🎉 " + player1Name + " Wins!");
        currentPlayerLabel->setStyleSheet("color: #00b894; background: rgba(0, 184, 148, 0.1);");
    } else if (game.getWinner() == Player::O) {
        result = QString("%1 wins! 🎉").arg(player2Name);
        winnerName = player2Name;
        player2Wins++;
        statusLabel->setText(QString("%1 won the game!").arg(player2Name));
        currentPlayerLabel->setText("🎉 " + player2Name + " Wins!");
        currentPlayerLabel->setStyleSheet("color: #00b894; background: rgba(0, 184, 148, 0.1);");
    } else {
        result = "It's a draw! 🤝";
        draws++;
        statusLabel->setText("Game ended in a draw!");
        currentPlayerLabel->setText("🤝 Draw!");
        currentPlayerLabel->setStyleSheet("color: #f39c12; background: rgba(243, 156, 18, 0.1);");
    }

    // Update score display
    scoreLabel->setText(QString("%1: %2 | %3: %4 | Draws: %5")
                            .arg(player1Name).arg(player1Wins)
                            .arg(player2Name).arg(player2Wins)
                            .arg(draws));

    logResult(result);

    // Show result after delay
    QTimer::singleShot(1500, [this, result]() {
        showGameResult(result);
    });
}

void PlayerVsPlayerWindow::updatePlayerTurn() {
    Player currentPlayer = game.getCurrentPlayer();

    if (currentPlayer == Player::X) {
        currentPlayerLabel->setText(QString("%1's Turn (X)").arg(player1Name));
        currentPlayerLabel->setStyleSheet("color: #e74c3c; background: rgba(231, 76, 60, 0.1); border: 1px solid rgba(231, 76, 60, 0.2); padding: 6px 12px; border-radius: 12px; min-width: 160px; max-width: 180px;");
        statusLabel->setText(QString("%1, make your move!").arg(player1Name));
    } else {
        currentPlayerLabel->setText(QString("%1's Turn (O)").arg(player2Name));
        currentPlayerLabel->setStyleSheet("color: #3498db; background: rgba(52, 152, 219, 0.1); border: 1px solid rgba(52, 152, 219, 0.2); padding: 6px 12px; border-radius: 12px; min-width: 160px; max-width: 180px;");
        statusLabel->setText(QString("%1, make your move!").arg(player2Name));
    }
}

void PlayerVsPlayerWindow::updateBoard() {
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

void PlayerVsPlayerWindow::enableBoard(bool enabled) {
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            if (cells[r][c]->text().isEmpty()) {
                cells[r][c]->setEnabled(enabled);
            }
        }
    }
}

void PlayerVsPlayerWindow::newGame() {
    game.reset();
    updateBoard();
    enableBoard(true);
    updatePlayerTurn();

    // Reset cell styling
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            cells[r][c]->setProperty("text", "");
            cells[r][c]->style()->unpolish(cells[r][c]);
            cells[r][c]->style()->polish(cells[r][c]);
        }
    }

    statusLabel->setText("New game started! X goes first.");
}

void PlayerVsPlayerWindow::showHistory() {
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

void PlayerVsPlayerWindow::goBack() {
    GameModeWindow *gameModeWindow = new GameModeWindow(user);
    gameModeWindow->show();
    this->close();
}

void PlayerVsPlayerWindow::logResult(const QString& result) {
    GameResult res;
    res.date = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toStdString();
    res.result = result.toStdString();
    history.saveResult(res);
}

void PlayerVsPlayerWindow::showGameResult(const QString& result) {
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
            background: #e91e63;
            color: white;
            border: none;
            border-radius: 5px;
            padding: 8px 20px;
            font-weight: bold;
        }
        QMessageBox QPushButton:hover {
            background: #c2185b;
        }
    )");

    int ret = msgBox.exec();
    if (ret == QMessageBox::Yes) {
        newGame();
    }
}