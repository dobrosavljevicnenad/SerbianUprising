#ifndef GAMEMENU_H
#define GAMEMENU_H

#include <QWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class GameMenu : public QWidget
{
    Q_OBJECT

public:
    explicit GameMenu(QWidget *parent = nullptr);
    ~GameMenu();

signals:
    void startGame();
    void exitGame();
    void fullScreenClicked();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void openSettings();

private:
    QLabel *shadowLabel;
    QLabel *titleLabel;
    QPushButton *newGameButton;
    QPushButton *settingsButton;
    QPushButton *exitButton;
    QPushButton *fullScreenButton;
    QStackedWidget *stackedWidget;

    void setupUI();
    void setBackgroundImage();

    QWidget *createSettingsMenu();
};

#endif // GAMEMENU_H
