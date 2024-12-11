#ifndef BATTLEREPLAYDIALOG_H
#define BATTLEREPLAYDIALOG_H

#include <QDialog>
#include "Results.h"
class BattleReplayDialog : public QDialog {
    Q_OBJECT

public:
    explicit BattleReplayDialog(QWidget *parent , int tableIndex, Results results);
    ~BattleReplayDialog();

    QPixmap getSoldierImage(ArmyType armyType);
private:
    int m_tableIndex; // Store the index to fetch corresponding data
    Results results;
};

#endif // BATTLEREPLAYDIALOG_H
