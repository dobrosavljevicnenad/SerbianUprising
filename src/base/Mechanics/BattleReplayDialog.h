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
#include <qpushbutton.h>
#include <QCoreApplication>
#include "Results.h"

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
    int m_tableIndex; // Store the index to fetch corresponding data
    Results results;
    std::vector<QLabel*> defenderImages;
    std::vector<QLabel*> attackerImages;
    QPushButton* replayButton;
    QVBoxLayout* defenderResultLayout;
    QVBoxLayout* attackerResultLayout;
};

#endif // BATTLEREPLAYDIALOG_H
