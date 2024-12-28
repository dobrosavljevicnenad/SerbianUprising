#pragma once

#include "../lobbywindow.h"


#include <QWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>

class GameMenu : public QWidget
{
    Q_OBJECT

public:
    explicit GameMenu(QWidget *parent = nullptr);
    ~GameMenu();
    QMediaPlayer *musicPlayer;
    QAudioOutput *audioOutput;

signals:
    void newGame();
    void exitGame();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void openSettings();
    void onExitButtonClicked();
    void fullScreenClicked();

private:
    QLabel *shadowLabel;
    QLabel *titleLabel;
    QPushButton *newGameButton;
    QPushButton *settingsButton;
    QPushButton *exitButton;
    QPushButton *fullScreenButton;
    QLabel *logoLabel;
    bool isFullScreenMode = false;
    QPushButton *muteButton;
    QStackedWidget *stackedWidget;
    QFrame *buttonFrame;

    LobbyWindow *lobbyWindow;

    QLabel *backgroundLabel; // Za dinamičko menjanje pozadine
    QVBoxLayout *stackedWidgetLayout;

    void setupUI();
    void setBackgroundImage();

    QWidget *createSettingsMenu();
    QWidget *lobbyMenu();
    QWidget *createLobby();
};
