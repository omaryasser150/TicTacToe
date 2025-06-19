#include "GameModeWindow.h"
#include "MainWindow.h"
#include "PlayerVsPlayerWindow.h"
#include "StartupWindow.h"

GameModeWindow::GameModeWindow(const QString& username, QWidget *parent)
    : QWidget(parent), currentUsername(username)
{
    setWindowTitle("Tic Tac Toe - Choose Game Mode");
    setFixedSize(450, 550);
    setupUI();
    applyStyles();
}

void GameModeWindow::setupUI()
{
    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Main frame for styling
    mainFrame = new QFrame();
    mainFrame->setObjectName("mainFrame");

    QVBoxLayout *frameLayout = new QVBoxLayout(mainFrame);
    frameLayout->setSpacing(25);
    frameLayout->setContentsMargins(40, 50, 40, 50);

    // Title section
    titleLabel = new QLabel("CHOOSE GAME MODE");
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);

    welcomeLabel = new QLabel("Welcome, " + currentUsername + "!");
    welcomeLabel->setObjectName("welcomeLabel");
    welcomeLabel->setAlignment(Qt::AlignCenter);

    // Game mode buttons
    aiButton = new QPushButton("🤖 PLAY VS AI");
    aiButton->setObjectName("gameModeButton");
    aiButton->setFixedHeight(80);

    playerButton = new QPushButton("👥 PLAY VS PLAYER");
    playerButton->setObjectName("gameModeButton");
    playerButton->setFixedHeight(80);

    // Back button
    backButton = new QPushButton("← BACK");
    backButton->setObjectName("backButton");
    backButton->setFixedHeight(40);

    // Add to layout
    frameLayout->addStretch();
    frameLayout->addWidget(titleLabel);
    frameLayout->addWidget(welcomeLabel);
    frameLayout->addSpacing(30);
    frameLayout->addWidget(aiButton);
    frameLayout->addWidget(playerButton);
    frameLayout->addStretch();
    frameLayout->addWidget(backButton);

    mainLayout->addWidget(mainFrame);

    // Connect signals
    connect(aiButton, &QPushButton::clicked, this, &GameModeWindow::selectAIMode);
    connect(playerButton, &QPushButton::clicked, this, &GameModeWindow::selectPlayerMode);
    connect(backButton, &QPushButton::clicked, this, &GameModeWindow::goBack);
}

void GameModeWindow::applyStyles()
{
    setStyleSheet(R"(
        QWidget {
            font-family: 'Segoe UI', Arial, sans-serif;
        }

        #mainFrame {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #4facfe, stop:1 #00f2fe);
            border-radius: 15px;
        }

        #titleLabel {
            font-size: 24px;
            font-weight: bold;
            color: white;
            letter-spacing: 2px;
        }

        #welcomeLabel {
            font-size: 16px;
            color: rgba(255, 255, 255, 0.9);
            font-weight: 500;
        }

        #gameModeButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(255, 255, 255, 0.9), stop:1 rgba(255, 255, 255, 0.7));
            color: #2c3e50;
            border: none;
            border-radius: 40px;
            font-size: 18px;
            font-weight: bold;
            letter-spacing: 1px;
        }

        #gameModeButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(255, 255, 255, 1.0), stop:1 rgba(255, 255, 255, 0.9));
            transform: translateY(-3px);
        }

        #gameModeButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(255, 255, 255, 0.7), stop:1 rgba(255, 255, 255, 0.5));
        }

        #backButton {
            background: rgba(255, 255, 255, 0.2);
            color: white;
            border: 1px solid rgba(255, 255, 255, 0.3);
            border-radius: 20px;
            font-size: 14px;
            font-weight: bold;
        }

        #backButton:hover {
            background: rgba(255, 255, 255, 0.3);
        }

        #backButton:pressed {
            background: rgba(255, 255, 255, 0.1);
        }
    )");
}

void GameModeWindow::selectAIMode()
{
    emit gameModeSelected(currentUsername, GameMode::AI);

    MainWindow *mainWindow = new MainWindow(currentUsername);
    mainWindow->show();
    this->close();
}

void GameModeWindow::selectPlayerMode()
{
    emit gameModeSelected(currentUsername, GameMode::PLAYER);

    // Use the new PlayerVsPlayerWindow for player vs player games
    PlayerVsPlayerWindow *playerWindow = new PlayerVsPlayerWindow(currentUsername);
    playerWindow->show();
    this->close();
}

void GameModeWindow::goBack()
{
    StartupWindow *startup = new StartupWindow();
    startup->show();
    this->close();
}
