#include "StartupWindow.h"
#include "LoginWindow.h"
#include "RegisterWindow.h"
#include <QApplication>

StartupWindow::StartupWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Tic Tac Toe - Welcome");
    setFixedSize(400, 500);
    setupUI();
    applyStyles();
}

void StartupWindow::setupUI()
{
    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Main frame for styling
    mainFrame = new QFrame();
    mainFrame->setObjectName("mainFrame");

    QVBoxLayout *frameLayout = new QVBoxLayout(mainFrame);
    frameLayout->setSpacing(30);
    frameLayout->setContentsMargins(40, 60, 40, 60);

    // Title section
    titleLabel = new QLabel("TIC TAC TOE");
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);

    subtitleLabel = new QLabel("Advanced Game Experience");
    subtitleLabel->setObjectName("subtitleLabel");
    subtitleLabel->setAlignment(Qt::AlignCenter);

    // Buttons
    loginButton = new QPushButton("LOGIN");
    loginButton->setObjectName("primaryButton");
    loginButton->setFixedHeight(50);

    registerButton = new QPushButton("REGISTER");
    registerButton->setObjectName("secondaryButton");
    registerButton->setFixedHeight(50);

    // Add to layout
    frameLayout->addStretch();
    frameLayout->addWidget(titleLabel);
    frameLayout->addWidget(subtitleLabel);
    frameLayout->addSpacing(40);
    frameLayout->addWidget(loginButton);
    frameLayout->addWidget(registerButton);
    frameLayout->addStretch();

    mainLayout->addWidget(mainFrame);

    // Connect signals
    connect(loginButton, &QPushButton::clicked, this, &StartupWindow::openLogin);
    connect(registerButton, &QPushButton::clicked, this, &StartupWindow::openRegister);
}

void StartupWindow::applyStyles()
{
    setStyleSheet(R"(
        QWidget {
            font-family: 'Segoe UI', Arial, sans-serif;
        }

        #mainFrame {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #667eea, stop:1 #764ba2);
            border-radius: 15px;
        }

        #titleLabel {
            font-size: 32px;
            font-weight: bold;
            color: white;
            letter-spacing: 3px;
        }

        #subtitleLabel {
            font-size: 14px;
            color: rgba(255, 255, 255, 0.8);
            font-weight: 300;
        }

        #primaryButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #ff6b6b, stop:1 #ee5a24);
            color: white;
            border: none;
            border-radius: 25px;
            font-size: 16px;
            font-weight: bold;
            letter-spacing: 1px;
        }

        #primaryButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #ff5252, stop:1 #d63031);
            transform: translateY(-2px);
        }

        #primaryButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #c44569, stop:1 #b71540);
        }

        #secondaryButton {
            background: rgba(255, 255, 255, 0.2);
            color: white;
            border: 2px solid rgba(255, 255, 255, 0.3);
            border-radius: 25px;
            font-size: 16px;
            font-weight: bold;
            letter-spacing: 1px;
        }

        #secondaryButton:hover {
            background: rgba(255, 255, 255, 0.3);
            border: 2px solid rgba(255, 255, 255, 0.5);
        }

        #secondaryButton:pressed {
            background: rgba(255, 255, 255, 0.1);
        }
    )");
}

void StartupWindow::openLogin()
{
    // Create login window
    LoginWindow *login = new LoginWindow();
    login->show();

    // Hide this window instead of deleting it
    this->hide();
}

void StartupWindow::openRegister()
{
    // Create register window
    RegisterWindow *reg = new RegisterWindow();
    reg->show();

    // Hide this window instead of deleting it
    this->hide();
}
