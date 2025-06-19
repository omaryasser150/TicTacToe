#include "LoginWindow.h"
#include "GameModeWindow.h"
#include "RegisterWindow.h"
#include "StartupWindow.h"
#include <QTimer>

LoginWindow::LoginWindow(QWidget* parent) : QWidget(parent), auth("users.db") {
    setWindowTitle("Tic Tac Toe - Login");
    setFixedSize(400, 550);
    setupUI();
    applyStyles();
}

void LoginWindow::setupUI()
{
    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Main frame for styling
    mainFrame = new QFrame();
    mainFrame->setObjectName("mainFrame");

    QVBoxLayout *frameLayout = new QVBoxLayout(mainFrame);
    frameLayout->setSpacing(20);
    frameLayout->setContentsMargins(40, 50, 40, 50);

    // Title
    titleLabel = new QLabel("LOGIN");
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);

    // Input fields
    QLabel* userLabel = new QLabel("Username:");
    userLabel->setObjectName("inputLabel");

    userEdit = new QLineEdit;
    userEdit->setObjectName("inputField");
    userEdit->setPlaceholderText("Enter your username");
    userEdit->setFixedHeight(45);

    QLabel* passLabel = new QLabel("Password:");
    passLabel->setObjectName("inputLabel");

    passEdit = new QLineEdit;
    passEdit->setObjectName("inputField");
    passEdit->setPlaceholderText("Enter your password");
    passEdit->setEchoMode(QLineEdit::Password);
    passEdit->setFixedHeight(45);

    // Buttons
    loginBtn = new QPushButton("LOGIN");
    loginBtn->setObjectName("primaryButton");
    loginBtn->setFixedHeight(50);

    registerBtn = new QPushButton("Don't have an account? Register");
    registerBtn->setObjectName("linkButton");
    registerBtn->setFixedHeight(35);

    backBtn = new QPushButton("← BACK");
    backBtn->setObjectName("backButton");
    backBtn->setFixedHeight(40);

    // Status label
    statusLabel = new QLabel();
    statusLabel->setObjectName("statusLabel");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setWordWrap(true);

    // Add to layout
    frameLayout->addStretch();
    frameLayout->addWidget(titleLabel);
    frameLayout->addSpacing(30);
    frameLayout->addWidget(userLabel);
    frameLayout->addWidget(userEdit);
    frameLayout->addWidget(passLabel);
    frameLayout->addWidget(passEdit);
    frameLayout->addSpacing(20);
    frameLayout->addWidget(loginBtn);
    frameLayout->addWidget(registerBtn);
    frameLayout->addWidget(statusLabel);
    frameLayout->addStretch();
    frameLayout->addWidget(backBtn);

    mainLayout->addWidget(mainFrame);

    // Connect signals
    connect(loginBtn, &QPushButton::clicked, this, &LoginWindow::handleLogin);
    connect(registerBtn, &QPushButton::clicked, this, &LoginWindow::handleRegister);
    connect(backBtn, &QPushButton::clicked, this, &LoginWindow::goBack);

    // Enter key support
    connect(userEdit, &QLineEdit::returnPressed, this, &LoginWindow::handleLogin);
    connect(passEdit, &QLineEdit::returnPressed, this, &LoginWindow::handleLogin);
}

void LoginWindow::applyStyles()
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
            font-size: 28px;
            font-weight: bold;
            color: white;
            letter-spacing: 2px;
        }

        #inputLabel {
            color: white;
            font-size: 14px;
            font-weight: 500;
            margin-bottom: 5px;
        }

        #inputField {
            background: rgba(255, 255, 255, 0.9);
            border: 2px solid rgba(255, 255, 255, 0.3);
            border-radius: 22px;
            padding: 0 15px;
            font-size: 14px;
            color: #2c3e50;
        }

        #inputField:focus {
            border: 2px solid rgba(255, 255, 255, 0.8);
            background: white;
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
        }

        #primaryButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #c44569, stop:1 #b71540);
        }

        #linkButton {
            background: none;
            color: rgba(255, 255, 255, 0.8);
            border: none;
            font-size: 12px;
            text-decoration: underline;
        }

        #linkButton:hover {
            color: white;
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

        #statusLabel {
            color: white;
            font-size: 12px;
            min-height: 20px;
        }
    )");
}

void LoginWindow::handleLogin() {
    QString u = userEdit->text().trimmed();
    QString p = passEdit->text();

    if (u.isEmpty() || p.isEmpty()) {
        showError("Please fill in all fields!");
        return;
    }

    if (auth.loginUser(u.toStdString(), p.toStdString())) {
        showSuccess("Login successful!");

        // Delay to show success message, then switch windows
        QTimer::singleShot(1000, [this, u]() {
            // Create the new window first
            GameModeWindow *gameModeWindow = new GameModeWindow(u);
            gameModeWindow->show();

            // Emit signal before closing
            emit loginSuccessful(u);

            // Close this window
            this->close();
        });
    } else {
        showError("Invalid username or password!");
    }
}

void LoginWindow::handleRegister() {
    // Create register window first
    RegisterWindow *registerWindow = new RegisterWindow();
    registerWindow->show();

    // Close this window
    this->close();
}

void LoginWindow::goBack() {
    // Create startup window first
    StartupWindow *startup = new StartupWindow();
    startup->show();

    // Close this window
    this->close();
}

void LoginWindow::showError(const QString& message) {
    statusLabel->setText("❌ " + message);
    statusLabel->setStyleSheet("color: #ff6b6b; font-weight: bold;");

    // Clear after 3 seconds
    QTimer::singleShot(3000, [this]() {
        statusLabel->clear();
    });
}

void LoginWindow::showSuccess(const QString& message) {
    statusLabel->setText("✅ " + message);
    statusLabel->setStyleSheet("color: #00b894; font-weight: bold;");
}

QString LoginWindow::getUsername() const {
    return userEdit->text();
}
