#include <QApplication>
#include <QStyleFactory>
#include <QDir>
#include <QStandardPaths>
#include <QMessageBox>
#include <QIcon>
#include <QFont>
#include <QFontDatabase>
#include "StartupWindow.h"
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Set application properties
    app.setApplicationName("Advanced Tic Tac Toe");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("TicTacToe Games");
    app.setOrganizationDomain("tictactoe.local");

    // Set application icon (if you have one)
    // app.setWindowIcon(QIcon(":/icons/app_icon.png"));

    // Ensure the application uses a consistent style
    app.setStyle(QStyleFactory::create("Fusion"));

    // Set up application-wide font
    QFont appFont("Segoe UI", 10);
    if (appFont.exactMatch()) {
        app.setFont(appFont);
    } else {
        // Fallback fonts
        QStringList fallbackFonts = {"Arial", "Helvetica", "sans-serif"};
        for (const QString& fontName : fallbackFonts) {
            QFont fallbackFont(fontName, 10);
            if (fallbackFont.exactMatch()) {
                app.setFont(fallbackFont);
                break;
            }
        }
    }

    // Create application data directory if it doesn't exist
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir;
    if (!dir.exists(dataDir)) {
        if (!dir.mkpath(dataDir)) {
            QMessageBox::critical(nullptr, "Error",
                                  "Could not create application data directory.\n"
                                  "The application may not function properly.");
        }
    }

    // Set working directory to application data directory
    QDir::setCurrent(dataDir);

    // Enable high DPI scaling for better display on high-resolution screens
    app.setAttribute(Qt::AA_EnableHighDpiScaling, true);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);

    // Create and show the startup window
    StartupWindow startup;
    startup.show();

    // Center the window on screen
    startup.move(
        (app.primaryScreen()->geometry().width() - startup.width()) / 2,
        (app.primaryScreen()->geometry().height() - startup.height()) / 2
        );

    // Run the application
    return app.exec();
}
