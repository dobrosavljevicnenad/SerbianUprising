#ifndef BATTLEREPLAYDIALOG_H
#define BATTLEREPLAYDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QPainter>
#include <QScrollArea>
#include <cmath>
#include <qmediaplayer.h>
#include <qpushbutton.h>
#include <QCoreApplication>
#include "Results.h"
#include <qtablewidget.h>
#include <qevent.h>
#include <qheaderview.h>
#include <QTimer>
#include <QAudioOutput>

class BattleReplayDialog : public QDialog {
    Q_OBJECT

public:
    explicit BattleReplayDialog(QWidget *parent , int tableIndex, Results results);
    ~BattleReplayDialog();

    QPixmap getSoldierImage(ArmyType armyType);
    QString getTerrainImage(TerrainType terrainType);
    QPixmap getSoldierFallenImage(ArmyType armyType);
    QPixmap getSoldierStandingImage(ArmyType armyType);
    QPixmap getSoldierFiringImage(ArmyType armyType);
public slots:
    void onReplayClicked();
private:
    QHBoxLayout *topLayout;
    QVBoxLayout *centerTopLayout;
    int m_tableIndex; // Store the index to fetch corresponding data
    Results results;
    std::vector<QLabel*> defenderImages;
    std::vector<QLabel*> attackerImages;
    QPushButton* replayButton;
    QPushButton *closeButton;
    QVBoxLayout* defenderResultLayout;
    QVBoxLayout* attackerResultLayout;
    QString getButtonStyle();
    QLabel *remainingAttackersLabel;
    QLabel *remainingDefendersLabel;
    void keyPressEvent(QKeyEvent *event);
    void playBattleMusic();
    QMediaPlayer m_mediaPlayer;                                   // Media player for playing sounds
    QAudioOutput m_audioOutput;
};

#endif // BATTLEREPLAYDIALOG_H
