#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clientwindow.h"
#include <QPixmap>
#include <QPalette>

// void setWidgetBackground(QWidget *widget, const QString &imagePath) {
//     QPixmap backgroundPixmap(":/resources/pozadina.png");
//     backgroundPixmap = backgroundPixmap.scaled(widget->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//     QPalette palette;
//     palette.setBrush(QPalette::Window, QBrush(backgroundPixmap));
//     widget->setPalette(palette);
//     widget->setAutoFillBackground(true);
// }

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), gameMenu(new GameMenu(this)),
    lobbyWindow(new LobbyWindow(this)),
    createLobbyWindow(new CreateLobbyWindow(this)),
    stackedWidget(new QStackedWidget(this))  {

    ui->setupUi(this);

    stackedWidget->addWidget(gameMenu);
    stackedWidget->addWidget(lobbyWindow);
    stackedWidget->addWidget(createLobbyWindow);

    setCentralWidget(stackedWidget);
    stackedWidget->setCurrentWidget(gameMenu);
    stackedWidget->resize(gameMenu->size());

    // stackedWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // stackedWidget->resize(gameMenu->size());

    // lobbyWindow->resize(gameMenu->size());
    // createLobbyWindow->resize(gameMenu->size());

    // setWidgetBackground(gameMenu, ":/resources/pozadina.png");
    // setWidgetBackground(lobbyWindow, ":/resources/pozadina.png");
    // setWidgetBackground(createLobbyWindow, ":/resources/pozadina.png");


    connect(gameMenu, &GameMenu::newGame, this, &MainWindow::showLobbyWindow);
    connect(gameMenu, &GameMenu::exitGame, this, &MainWindow::close);
    connect(lobbyWindow, &LobbyWindow::createLobby, this, &MainWindow::showCreateLobbyWindow);
    connect(lobbyWindow, &LobbyWindow::backToMenu, this, &MainWindow::showGameMenu);
    connect(createLobbyWindow, &CreateLobbyWindow::backToLobby, this, &MainWindow::showLobbyWindow);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::showGameMenu() {
    stackedWidget->setCurrentWidget(gameMenu);
}

void MainWindow::showLobbyWindow() {
    stackedWidget->setCurrentWidget(lobbyWindow);
}

void MainWindow::showCreateLobbyWindow() {
    stackedWidget->setCurrentWidget(createLobbyWindow);
}

