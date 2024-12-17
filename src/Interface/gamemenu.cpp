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

    mediaPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    mediaPlayer->setAudioOutput(audioOutput);
    mediaPlayer->setSource(QUrl::fromLocalFile("../../resources/music/backgroundMusic.mp3"));
    audioOutput->setVolume(0.5);
    mediaPlayer->play();

    connect(newGameButton, &QPushButton::clicked, this, [this]() {
        stackedWidget->setCurrentIndex(2);
    });
    connect(settingsButton, &QPushButton::clicked, this, &GameMenu::openSettings);
    connect(exitButton, &QPushButton::clicked, this, &GameMenu::exitGame);
    connect(fullScreenButton, &QPushButton::clicked, this, &GameMenu::fullScreenClicked);

}

GameMenu::~GameMenu() {}

void GameMenu::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->setContentsMargins(10, 70, 10, 10);

    setBackgroundImage();

    buttonFrame = new QFrame(this);
    buttonFrame->setMinimumWidth(450);
    buttonFrame->setMinimumHeight(350);
    //buttonFrame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    buttonFrame->setStyleSheet(R"(
        background-color: qlineargradient(
            spread:pad, x1:0, y1:0, x2:1, y2:1,
            stop:0 rgba(255, 228, 196, 200),
            stop:1 rgba(139, 69, 19, 220)
        );
        border-radius: 10px;
        border: 2px solid rgba(255, 215, 0, 150);
    )");

    newGameButton = new QPushButton("NEW GAME", this);
    settingsButton = new QPushButton("SETTINGS", this);
    exitButton = new QPushButton("EXIT", this);
    fullScreenButton = new QPushButton("⤄", this);

    QString buttonStyle = R"(
    QPushButton {
        border: none;
        background: transparent;
        background-image: url(:/resources/button.png);
        background-position: center;
        background-repeat: no-repeat;
        color: white;
        font: bold 20px "Serif";
        text-align: center;
    }
    QPushButton:hover {
        background-image: url(:/resources/button.png);
        color: #FFD700;
        font-size: 22px;
    }
    QPushButton:pressed {
        background-image: url(:/resources/button.png);
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


    QVBoxLayout *buttonLayout = new QVBoxLayout(buttonFrame);
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->setSpacing(10);
    buttonLayout->addStretch();
    buttonLayout->addWidget(newGameButton, 0, Qt::AlignCenter);
    //buttonLayout->addSpacing(10);
    buttonLayout->addWidget(settingsButton, 0, Qt::AlignCenter);
    //buttonLayout->addSpacing(10);
    buttonLayout->addWidget(exitButton, 0, Qt::AlignCenter);
    buttonLayout->addStretch();
    //buttonLayout->setAlignment(Qt::AlignCenter);

    stackedWidget = new QStackedWidget(this);
    QWidget *mainMenu = new QWidget(this);
    QVBoxLayout *menuLayout = new QVBoxLayout(mainMenu);
    menuLayout->addWidget(buttonFrame, 0, Qt::AlignCenter);
    mainMenu->setLayout(menuLayout);
    stackedWidget->addWidget(mainMenu);
    stackedWidget->addWidget(createSettingsMenu());
    stackedWidget->addWidget(lobbyMenu());

    layout->addWidget(stackedWidget);

    // layout->addStretch();
    // layout->addLayout(buttonLayout);
    // layout->addStretch();

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();
    bottomLayout->addWidget(fullScreenButton);
    layout->addLayout(bottomLayout);

    setLayout(layout);

}

void GameMenu::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    setBackgroundImage();

    const int baseWidth = 800;
    const int baseHeight = 600;

    int screenWidth = event->size().width();
    int screenHeight = event->size().height();

    double scaleX = static_cast<double>(screenWidth) / baseWidth;
    double scaleY = static_cast<double>(screenHeight) / baseHeight;
    double scale = qMin(scaleX, scaleY);

    int buttonFrameWidth = 450*scale;
    int buttonFrameHeight = 350*scale;

    if (buttonFrame) {
        buttonFrame->setFixedSize(buttonFrameWidth, buttonFrameHeight);
    }

    int buttonWidth = 400*scale;
    int buttonHeight = 100*scale;

    QSize buttonSize(buttonWidth, buttonHeight);
    newGameButton->setFixedSize(buttonSize);
    settingsButton->setFixedSize(buttonSize);
    exitButton->setFixedSize(buttonSize);

    int fontSize = qMin(static_cast<int>(20*scale), 30);
    QString buttonStyle = QString(R"(
        QPushButton {
            border: none;
            background: transparent;
            background-image: url(:/resources/button.png);
            background-position: center;
            background-repeat: no-repeat;
            color: white;
            font: bold %1px "Serif";
            text-align: center;
        }
        QPushButton:hover {
            background-image: url(:/resources/button.png);
            color: #FFD700;
            font-size: %2px;
        }
        QPushButton:pressed {
            background-image: url(:/resources/button.png);
            color: #FFA500;
        }
    )").arg(fontSize).arg(fontSize + 2);

    newGameButton->setStyleSheet(buttonStyle);
    settingsButton->setStyleSheet(buttonStyle);
    exitButton->setStyleSheet(buttonStyle);

    // if(newGameButton->size() != QSize(400, 100)){
    //     QSize buttonSize(screenWidth / 2, 50);
    //     newGameButton->setFixedSize(buttonSize);
    //     settingsButton->setFixedSize(buttonSize);
    //     exitButton->setFixedSize(buttonSize);
    // }

    stackedWidget->layout()->setContentsMargins(10*scale, 70*scale, 10*scale, 10*scale);
}

void GameMenu::setBackgroundImage() {
    QPixmap backgroundPixmap(":/resources/pozadina.png");
    backgroundPixmap = backgroundPixmap.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(backgroundPixmap));
    this->setPalette(palette);
    this->setAutoFillBackground(true);
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

    connect(display, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
        switch (index) {
        case 0:
            this->parentWidget()->resize(960, 540);
            break;
        case 1:
            this->parentWidget()->resize(1600, 900);
            break;
        case 2:
            if (QWidget *parentWindow = this->parentWidget()) {
                parentWindow->showFullScreen();
            }
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
            background-image: url(:/resources/button.png);
            background-position: center;
            background-repeat: no-repeat;
            color: white;
            font: bold 20px "Serif";
            text-align: center;
        }
        QPushButton:hover {
            background-image: url(:/resources/button.png);
            color: #FFD700;
            font-size: 22px;
        }
        QPushButton:pressed {
            background-image: url(:/resources/button.png);
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
    //layout->addWidget(back, 0, Qt::AlignCenter);

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

QWidget *GameMenu::lobbyMenu() {
    LobbyWindow *lobby = new LobbyWindow(this);

    connect(lobby, &LobbyWindow::backToMenu, this, [this]() {
        stackedWidget->setCurrentIndex(0); // Vrati na glavni meni
    });

    return lobby;
}
