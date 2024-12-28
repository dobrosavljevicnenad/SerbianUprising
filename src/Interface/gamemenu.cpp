#include "gamemenu.h"
#include "../lobbywindow.h"
#include <QGraphicsDropShadowEffect>
#include <QScreen>
#include <QResizeEvent>
#include <QFontDatabase>
#include <QSlider>
#include <QDialog>
#include <QScrollArea>
#include <QComboBox>

GameMenu::GameMenu(QWidget *parent) : QWidget(parent) {

    setupUI();
    resizeEvent(new QResizeEvent(this->size(), this->size()));

    musicPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    musicPlayer->setAudioOutput(audioOutput);
    musicPlayer->setSource(QUrl::fromLocalFile("../../resources/music/backgroundMusic.mp3"));
    audioOutput->setVolume(0.5);
    musicPlayer->play();

    connect(newGameButton, &QPushButton::clicked, this, &GameMenu::newGame);
    connect(settingsButton, &QPushButton::clicked, this, &GameMenu::openSettings);
    connect(exitButton, &QPushButton::clicked, this, &GameMenu::onExitButtonClicked);
    connect(fullScreenButton, &QPushButton::clicked, this, &GameMenu::fullScreenClicked);
    connect(muteButton, &QPushButton::clicked, this, [this]() {
        static bool isMuted = false;
        isMuted = !isMuted;
        audioOutput->setMuted(isMuted);
        muteButton->setText(isMuted ? "🔊" : "🔇");
    });
}

GameMenu::~GameMenu() {}

void GameMenu::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->setContentsMargins(0, 0, 0, 0);

    setBackgroundImage();

    logoLabel = new QLabel(this);
    QPixmap logoPixmap(":/resources/Images/logo.png");
    logoPixmap = logoPixmap.scaled(500, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    logoLabel->setPixmap(logoPixmap);
    logoLabel->setAlignment(Qt::AlignCenter);

    buttonFrame = new QFrame(this);
    buttonFrame->setMinimumWidth(400);
    buttonFrame->setMinimumHeight(300);
    buttonFrame->setStyleSheet(R"(
        background-color: transparent;
    )");

    newGameButton = new QPushButton("NEW GAME", this);
    settingsButton = new QPushButton("SETTINGS", this);
    exitButton = new QPushButton("EXIT", this);
    fullScreenButton = new QPushButton("⛶", this);
    muteButton = new QPushButton("🔇", this);

    QString buttonStyle = R"(
    QPushButton {
        border: none;
        background: transparent;
        background-image: url(:/resources/Images/button.png);
        background-position: center;
        background-repeat: no-repeat;
        color: white;
        font: bold 20px "Serif";
        text-align: center;
    }
    QPushButton:hover {
        background-image: url(:/resources/Images/button.png);
        color: #FFD700;
        font-size: 22px;
    }
    QPushButton:pressed {
        background-image: url(:/resources/Images/button.png);
        color: #FFA500;
    }
)";

    newGameButton->setStyleSheet(buttonStyle);
    settingsButton->setStyleSheet(buttonStyle);
    exitButton->setStyleSheet(buttonStyle);

    newGameButton->setFixedSize(400, 100);
    settingsButton->setFixedSize(400, 100);
    exitButton->setFixedSize(400, 100);
    fullScreenButton->setFixedSize(40, 40);
    fullScreenButton->setStyleSheet("font-size: 14px; background-color: #F44336; color: white; border-radius: 5px;}"
                                    "QPushButton::hover {font-size: 20px; padding: 7px;}");
    muteButton->setFixedSize(40, 40);
    muteButton->setStyleSheet("font-size: 14px; background-color: #FFB300; color: white; border-radius: 5px;}"
                              "QPushButton::hover {font-size: 20px; padding: 7px;}");

    QVBoxLayout *buttonLayout = new QVBoxLayout(buttonFrame);
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->setSpacing(5);
    buttonLayout->addStretch();
    buttonLayout->addWidget(newGameButton, 0, Qt::AlignCenter);
    buttonLayout->addWidget(settingsButton, 0, Qt::AlignCenter);
    buttonLayout->addWidget(exitButton, 0, Qt::AlignCenter);
    buttonLayout->addStretch();

    stackedWidget = new QStackedWidget(this);
    QWidget *mainMenu = new QWidget(this);
    QVBoxLayout *menuLayout = new QVBoxLayout(mainMenu);

    menuLayout->addWidget(logoLabel, 0, Qt::AlignCenter);
    menuLayout->addSpacing(0); // Reduce spacing here
    menuLayout->addWidget(buttonFrame, 0, Qt::AlignCenter);
    mainMenu->setLayout(menuLayout);
    stackedWidget->addWidget(mainMenu);

    QWidget *settingsMenu = createSettingsMenu();
    stackedWidget->addWidget(settingsMenu);

    layout->addWidget(stackedWidget);

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();
    bottomLayout->addWidget(muteButton);
    bottomLayout->addWidget(fullScreenButton);
    layout->addLayout(bottomLayout);

    setLayout(layout);
}



void GameMenu::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    setBackgroundImage();

    const int baseWidth = 1024;
    const int baseHeight = 768;

    int screenWidth = event->size().width();
    int screenHeight = event->size().height();

    double scaleX = static_cast<double>(screenWidth) / baseWidth;
    double scaleY = static_cast<double>(screenHeight) / baseHeight;
    double scale = qMin(scaleX, scaleY);

    // Resize buttons
    int buttonWidth = static_cast<int>(400 * scale);
    int buttonHeight = static_cast<int>(100 * scale);

    QSize buttonSize(buttonWidth, buttonHeight);
    newGameButton->setMinimumSize(buttonSize);
    newGameButton->setMaximumSize(buttonSize);
    settingsButton->setMinimumSize(buttonSize);
    settingsButton->setMaximumSize(buttonSize);
    exitButton->setMinimumSize(buttonSize);
    exitButton->setMaximumSize(buttonSize);

    int fontSize = qMin(static_cast<int>(20 * scale), 30);


    QString buttonStyle;
    if (isFullScreenMode) {
        buttonStyle = QString(R"(
            QPushButton {
                border: none;
                background: transparent;
                background-image: url(:/resources/Images/bigButton.png);
                background-position: center;
                background-repeat: no-repeat;
                color: white;
                font: bold %1px "Serif";
                text-align: center;
            }
            QPushButton:hover {
                background-image: url(:/resources/Images/bigButton.png);
                color: #FFD700;
                font-size: %2px;
            }
            QPushButton:pressed {
                background-image: url(:/resources/Images/bigButton.png);
                color: #FFA500;
            }
        )").arg(fontSize).arg(fontSize + 2);
    } else {
        buttonStyle = QString(R"(
            QPushButton {
                border: none;
                background: transparent;
                background-image: url(:/resources/Images/button.png);
                background-position: center;
                background-repeat: no-repeat;
                color: white;
                font: bold %1px "Serif";
                text-align: center;
            }
            QPushButton:hover {
                background-image: url(:/resources/Images/button.png);
                color: #FFD700;
                font-size: %2px;
            }
            QPushButton:pressed {
                background-image: url(:/resources/Images/button.png);
                color: #FFA500;
            }
        )").arg(fontSize).arg(fontSize + 2);
    }
    newGameButton->setStyleSheet(buttonStyle);
    settingsButton->setStyleSheet(buttonStyle);
    exitButton->setStyleSheet(buttonStyle);

    int buttonFrameWidth = static_cast<int>(450 * scale);
    int buttonFrameHeight = static_cast<int>(350 * scale);
    if (buttonFrame) {
        buttonFrame->setMinimumSize(buttonFrameWidth, buttonFrameHeight);
        buttonFrame->setMaximumSize(buttonFrameWidth, buttonFrameHeight);
    }

    stackedWidget->layout()->setContentsMargins(
        static_cast<int>(10 * scale),
        static_cast<int>(50 * scale),
        static_cast<int>(10 * scale),
        static_cast<int>(10 * scale)
        );

    if (logoLabel) {
        int logoWidth = static_cast<int>(500 * scale);
        int logoHeight = static_cast<int>(400 * scale);
        QPixmap logoPixmap(":/resources/Images/logo.png");
        logoPixmap = logoPixmap.scaled(logoWidth, logoHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        logoLabel->setPixmap(logoPixmap);
        logoLabel->setFixedSize(logoWidth, logoHeight);
    }
}


void GameMenu::setBackgroundImage() {
    QPixmap backgroundPixmap(":/resources/Images/pocetna.png");
    backgroundPixmap = backgroundPixmap.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(backgroundPixmap));
    this->setPalette(palette);
    this->setAutoFillBackground(true);
}

void GameMenu::fullScreenClicked() {
    if (window()->windowState() & Qt::WindowMaximized) {
        window()->showNormal();
        isFullScreenMode = false;
    } else {
        window()->showMaximized();
        isFullScreenMode = true;
    }
}



QWidget *GameMenu::createSettingsMenu() {
    QWidget *settingsMenu = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(settingsMenu);

    QLabel *audioLabel = new QLabel("Audio", settingsMenu);
    QSlider *volume = new QSlider(Qt::Horizontal, settingsMenu);
    volume->setRange(0, 100);
    volume->setValue(50);
    connect(volume, &QSlider::valueChanged, this, [this](int value) {
        audioOutput->setVolume(value / 100.0);
    });

    QLabel *screenLabel = new QLabel("Display mode", settingsMenu);
    QComboBox *display = new QComboBox(settingsMenu);
    display->addItem("qHD");
    display->addItem("HD+");
    display->addItem("Full screen");
    display->setStyleSheet(R"(
        QComboBox {
            background-color: rgba(139, 69, 19, 180);
            border: 1px solid rgba(255, 215, 0, 150);
            border-radius: 10px;
            padding: 5px;
            color: white;
        }
        QComboBox QAbstractItemView {
            border: 1px solid rgba(139, 69, 19, 180);
            border-radius: 5px;
            background-color: rgba(255, 215, 0, 150);
        }
    )");

    display->setCurrentIndex(1);

    connect(display, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
        QWidget *mainWindow = this->window();
        switch (index) {
        case 0:
            mainWindow->resize(960, 540);
            break;
        case 1:
            mainWindow->resize(1600, 900);
            break;
        case 2:
            mainWindow->showFullScreen();
            break;
        default:
            break;
        }
    });


    QPushButton *back = new QPushButton("Back", settingsMenu);
    back->setFixedSize(400, 100);
    back->setStyleSheet(R"(
        QPushButton {
            border: none;
            background: transparent;
            background-image: url(:/resources/Images/button.png);
            background-position: center;
            background-repeat: no-repeat;
            color: white;
            font: bold 20px "Serif";
            text-align: center;
        }
        QPushButton:hover {
            background-image: url(:/resources/Images/button.png);
            color: #FFD700;
            font-size: 22px;
        }
        QPushButton:pressed {
            background-image: url(:/resources/Images/button.png);
            color: #FFA500;
        }
    )");

    connect(back, &QPushButton::clicked, this, [=]() {
        stackedWidget->setCurrentIndex(0);
    });

    QWidget *frame = new QWidget(settingsMenu);
    frame->setStyleSheet(R"(
        background-color: qlineargradient(
            spread:pad, x1:0, y1:0, x2:1, y2:1,
            stop:0 rgba(255, 228, 196, 200),
            stop:1 rgba(139, 69, 19, 220)
        );        border: 2px solid rgba(255, 215, 0, 150);
        border-radius: 15px;
        padding: 20px;
    )");

    QVBoxLayout *frameLayout = new QVBoxLayout(frame);
    frameLayout->addWidget(audioLabel);
    frameLayout->addWidget(volume);
    frameLayout->addSpacing(10);
    frameLayout->addWidget(screenLabel);
    frameLayout->addWidget(display);
    frameLayout->addSpacing(10);
    frameLayout->addSpacing(20);
    frameLayout->addWidget(back, 0, Qt::AlignCenter);

    QScrollArea *scrollArea = new QScrollArea(settingsMenu);
    scrollArea->setWidget(frame);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet(R"(
        QScrollArea {
            border: none;
            border-radius: 15px;
            background-color: rgba(255, 255, 255, 80%);
        }
        QScrollBar:vertical {
            width: 12px;
            background: rgba(255, 215, 0, 150);
            border-radius: 6px;
        }
        QScrollBar::handle:vertical {
            background: rgba(255, 215, 0, 200);
            border-radius: 6px;
        }
        QScrollBar::handle:vertical:hover {
            background: rgba(255, 215, 0, 255);
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            background: none;
        }
        QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {
            background: none;
        }
    )");

    layout->addWidget(scrollArea);
    layout->addSpacing(10);

    settingsMenu->setMinimumWidth(400);
    settingsMenu->setFixedHeight(400);

    return settingsMenu;
}

void GameMenu::openSettings() {
    stackedWidget->setCurrentIndex(1);
}

void GameMenu::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        QWidget *parentWindow = parentWidget();
        if (parentWindow && parentWindow->isFullScreen()) {
            parentWindow->showNormal();
        } else if (this->isFullScreen()) {
            this->showNormal();
        }
    }
    QWidget::keyPressEvent(event);
}

void GameMenu::onExitButtonClicked() {
    emit exitGame();
}
