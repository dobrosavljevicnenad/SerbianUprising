#include "BattleReplayDialog.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <cmath> // Za funkciju std::ceil

BattleReplayDialog::BattleReplayDialog(QWidget *parent, int tableIndex, Results results)
    : QDialog(parent), m_tableIndex(tableIndex), results(results) {
    setWindowTitle("Battle Replay Details");

    // Setup main layout
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Get number of soldiers for defender and attacker
    int defenderNumber = results.getDefenderNumber();
    int attackerNumber = results.getAttackerNumber();

    // Create a layout for soldiers (defender on the left, attacker on the right)
    QHBoxLayout *soldiersLayout = new QHBoxLayout();

    // Left side for defender soldiers
    QVBoxLayout *defenderLayout = new QVBoxLayout();
    int defenderSoldiers = std::ceil(defenderNumber / 10.0);  // Zaokruži naviše
    for (int i = 0; i < defenderSoldiers; ++i) {
        QLabel *soldierLabel = new QLabel(this);
        QPixmap defenderSoldierImage = getSoldierImage(results.getDefenderType());
        soldierLabel->setPixmap(defenderSoldierImage.scaled(50, 50, Qt::KeepAspectRatio));  // Prilagodi veličinu slike
        soldierLabel->setAlignment(Qt::AlignCenter);  // Centriraj slike
        defenderLayout->addWidget(soldierLabel);
    }

    // Right side for attacker soldiers
    QVBoxLayout *attackerLayout = new QVBoxLayout();
    int attackerSoldiers = std::ceil(attackerNumber / 10.0);  // Zaokruži naviše
    for (int i = 0; i < attackerSoldiers; ++i) {
        QLabel *soldierLabel = new QLabel(this);
        QPixmap attackerSoldierImage = getSoldierImage(results.getAttackerType());
        soldierLabel->setPixmap(attackerSoldierImage.scaled(50, 50, Qt::KeepAspectRatio));  // Prilagodi veličinu slike
        soldierLabel->setAlignment(Qt::AlignCenter);  // Centriraj slike
        attackerLayout->addWidget(soldierLabel);
    }

    // Add defender and attacker layouts to the soldiers layout
    soldiersLayout->addLayout(defenderLayout);
    soldiersLayout->addLayout(attackerLayout);

    // Add the soldiers layout to the main layout
    layout->addLayout(soldiersLayout);

    // Set the dialog layout
    setLayout(layout);

    // Set a fixed size for the dialog
    resize(800, 600);
    setObjectName("BattleReplayDialog");
    setStyleSheet("QDialog#BattleReplayDialog { background-color: rgb(34, 40, 49); }");
}

QPixmap BattleReplayDialog::getSoldierImage(ArmyType armyType) {
    // Vrati odgovarajuću sliku na osnovu tipa vojske
    switch(armyType) {
    case ArmyType::HAJDUK:
        return QPixmap(":/resources/Hajduk.png");
    case ArmyType::JANISSARY:
        return QPixmap(":/resources/Jannisary.png");
    default:
        // U slučaju neprepoznatog tipa vojske, vratiti podrazumevanu sliku
        return QPixmap(":/resources/OtherSoldier.png");
    }
}

BattleReplayDialog::~BattleReplayDialog() {
    // Cleanup if necessary
}
