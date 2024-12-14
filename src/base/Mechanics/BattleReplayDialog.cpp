#include "BattleReplayDialog.h"


BattleReplayDialog::BattleReplayDialog(QWidget *parent, int tableIndex, Results results)
    : QDialog(parent), m_tableIndex(tableIndex), results(results) {
    setWindowTitle("Battle Replay Details");
    QVBoxLayout *layout = new QVBoxLayout(this);
    replayButton = new QPushButton("Replay", this);
    connect(replayButton, &QPushButton::clicked, this, &BattleReplayDialog::onReplayClicked);

    replayButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #81D4FA;"
        "    color: black;"
        "    border: 2px solid #81D4FA;"
        "    border-radius: 10px;"
        "    padding: 10px 20px;"
        "    font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #4FC3F7;"
        "    border-color: #4FC3F7;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #29B6F6;"
        "    border-color: #29B6F6;"
        "}"
        );

    QHBoxLayout *topLayout = new QHBoxLayout();
    QVBoxLayout *centerTopLayout = new QVBoxLayout();
    centerTopLayout->addWidget(replayButton);
    topLayout->addLayout(centerTopLayout);
    topLayout->setAlignment(Qt::AlignTop);
    centerTopLayout->setAlignment(Qt::AlignCenter);
    QHBoxLayout *resultsHboxLayout = new QHBoxLayout();
    centerTopLayout->addLayout(resultsHboxLayout);
    defenderResultLayout = new QVBoxLayout();
    attackerResultLayout = new QVBoxLayout();
    defenderResultLayout->setAlignment(Qt::AlignCenter);
    attackerResultLayout->setAlignment(Qt::AlignCenter);

    resultsHboxLayout->addLayout(defenderResultLayout);
    resultsHboxLayout->addLayout(attackerResultLayout);
    layout->addLayout(topLayout);
    QHBoxLayout *soldiersLayout = new QHBoxLayout();
    QVBoxLayout *defenderLayout = new QVBoxLayout();
    QHBoxLayout *dcolumnLayout = new QHBoxLayout();
    defenderLayout->addLayout(dcolumnLayout);
    int defenderNumber = results.getDefenderNumber();
    int attackerNumber = results.getAttackerNumber();

    int defenderSoldiers = std::ceil(static_cast<double>(defenderNumber) / 10.0);
    int currentDefenderSoldier  = 0;
    while (defenderNumber != 0 && currentDefenderSoldier < defenderSoldiers) {
        QVBoxLayout *defenderArmyLayout = new QVBoxLayout();
        for (int i = 0; i < 8 && currentDefenderSoldier < defenderSoldiers ; ++i) {
            QLabel *soldierLabel = new QLabel(this);
            QPixmap defenderSoldierImage = getSoldierImage(results.getDefenderType());
            defenderImages.push_back(soldierLabel);
            soldierLabel->setPixmap(defenderSoldierImage.scaled(70, 70, Qt::KeepAspectRatio));
            soldierLabel->setAlignment(Qt::AlignCenter);
            defenderArmyLayout->addWidget(soldierLabel);
            currentDefenderSoldier++;
        }
        dcolumnLayout->addLayout(defenderArmyLayout);
    }

    QVBoxLayout *attackerLayout = new QVBoxLayout();
    QHBoxLayout *acolumnLayout = new QHBoxLayout();
    attackerLayout->addLayout(acolumnLayout);

    int attackerSoldiers = std::ceil(static_cast<double>(attackerNumber) / 10.0);
    int currentAttackerSoldier  = 0;
    while (attackerNumber != 0 && currentAttackerSoldier < attackerSoldiers) {
        QVBoxLayout *attackerArmyLayout = new QVBoxLayout();
        for (int i = 0; i < 8 && currentAttackerSoldier < attackerSoldiers; ++i) {
            QLabel *soldierLabel = new QLabel(this);

            QPixmap attackerSoldierImage = getSoldierImage(results.getAttackerType());

            attackerImages.push_back(soldierLabel);
            QTransform transform;
            transform.scale(-1, 1);
            QPixmap flippedAttackerImage = attackerSoldierImage.transformed(transform);

            soldierLabel->setPixmap(flippedAttackerImage.scaled(70, 70, Qt::KeepAspectRatio));
            soldierLabel->setAlignment(Qt::AlignCenter);
            attackerArmyLayout->addWidget(soldierLabel);
            currentAttackerSoldier++;
        }
        acolumnLayout->addLayout(attackerArmyLayout);
    }

    soldiersLayout->addLayout(defenderLayout);
    soldiersLayout->addLayout(attackerLayout);

    QScrollArea *scrollArea = new QScrollArea(this);
    QWidget *scrollWidget = new QWidget(scrollArea);
    scrollWidget->setLayout(soldiersLayout);
    scrollArea->setStyleSheet("QScrollArea { background: transparent; border: none; }");
    scrollWidget->setStyleSheet("background: transparent;");
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMaximumHeight(620);

    layout->addWidget(scrollArea);

    layout->setAlignment(Qt::AlignBottom);
    setLayout(layout);

    resize(800, 800);
    setObjectName("BattleReplayDialog");
    QString styleSheet = QString("QDialog#BattleReplayDialog { "
                                 "border-image: url(%1);}").arg(getTerrainImage(results.getTerrain()));
    setStyleSheet(styleSheet);
}

QPixmap BattleReplayDialog::getSoldierImage(ArmyType armyType) {
    switch(armyType) {
    case ArmyType::HAJDUK:
        return QPixmap(":/resources/Images/SoldierHajdukStanding.png");
    case ArmyType::JANISSARY:
        return QPixmap(":/resources/Images/SoldierJannisaryStanding.png");
    default:
        return QPixmap(":/resources/OtherSoldier.png");
    }
}
QPixmap BattleReplayDialog::getSoldierFallenImage(ArmyType armyType) {
    switch(armyType) {
    case ArmyType::HAJDUK:
        return QPixmap(":/resources/Images/SoldierHajdukFallen.png");
    case ArmyType::JANISSARY:
        return QPixmap(":/resources/Images/SoldierJannisaryFallen.png");
    default:
        return QPixmap(":/resources/OtherSoldier.png");
    }
}
QPixmap BattleReplayDialog::getSoldierFiringImage(ArmyType armyType) {
    switch(armyType) {
    case ArmyType::HAJDUK:
        return QPixmap(":/resources/Images/SoldierHajdukFiring.png");
    case ArmyType::JANISSARY:
        return QPixmap(":/resources/Images/SoldierJannisaryFiring.png");
    default:
        return QPixmap(":/resources/OtherSoldier.png");
    }
}
QPixmap BattleReplayDialog::getSoldierStandingImage(ArmyType armyType) {
    switch(armyType) {
    case ArmyType::HAJDUK:
        return QPixmap(":/resources/Images/SoldierHajdukStanding.png");
    case ArmyType::JANISSARY:
        return QPixmap(":/resources/Images/SoldierJannisaryStanding.png");
    default:
        return QPixmap(":/resources/OtherSoldier.png");
    }
}
QString BattleReplayDialog::getTerrainImage(TerrainType terrainType) {
    switch(terrainType) {
    case TerrainType::FIELD:
        return QString(":/resources/Images/MapFields.png");
    case TerrainType::HILL:
        return QString(":/resources/Images/MapHills.png");
    case TerrainType::MOUNTAIN:
        return QString(":/resources/Images/MapMountains.png");
    case TerrainType::FOREST:
        return QString(":/resources/Images/MapForest.png");
    default:
        return QString(":/resources/OtherMap.png");
    }
}
void BattleReplayDialog::onReplayClicked() {
    std::vector<int> fallenDefenders;
    std::vector<int> fallenAttackers;
    std::vector<int> firingAttackers;
    int defenderNumber = results.getDefenderNumber();
    int attackerNumber = results.getAttackerNumber();
    int defenderSoldiers = std::ceil(static_cast<double>(defenderNumber) / 10.0);
    int attackerSoldiers = std::ceil(static_cast<double>(attackerNumber) / 10.0);
    int remainingAtackers = attackerNumber;
    int remainingDefenders = defenderNumber;
    std::time_t start = std::time(0);
    while(std::difftime(std::time(0),start)<1) {
        QCoreApplication::processEvents();
    }
        if(defenderNumber != 0 && attackerNumber != 0){
        for(auto round : results.getRounds()){
            std::srand(static_cast<unsigned>(std::time(nullptr)));
            int firingDef = (std::ceil(static_cast<double>(round.defenderHits)/10));
            if (remainingDefenders <= round.defenderHits)
                for (int var = 0; var < defenderImages.size(); var++) {
                    QPixmap defenderSoldierFiringImage = getSoldierFiringImage(results.getDefenderType());
                    defenderImages[var]->setPixmap(defenderSoldierFiringImage.scaled(70, 70, Qt::KeepAspectRatio));
                }
            else{
                for(int i = 0; i < firingDef; i++){
                    int x = 0;
                    do{
                        x = std::rand() % defenderSoldiers;
                    } while(std::find(fallenDefenders.begin(), fallenDefenders.end(), x) != fallenDefenders.end());
                    QPixmap defenderSoldierFiringImage = getSoldierFiringImage(results.getDefenderType());
                    defenderImages[x]->setPixmap(defenderSoldierFiringImage.scaled(70, 70, Qt::KeepAspectRatio));
                }
            }
            int firingAck = std::ceil(static_cast<double>(round.attackerHits)/10);
            if(remainingAtackers <= round.attackerHits){
                for (int var = 0; var < attackerImages.size(); var++) {
                    QPixmap attackerSoldierFiringImage = getSoldierFiringImage(results.getAttackerType());
                    QTransform transform;
                    transform.scale(-1, 1);
                    QPixmap flippedAttackerImage = attackerSoldierFiringImage.transformed(transform);
                    attackerImages[var]->setPixmap(flippedAttackerImage.scaled(70, 70, Qt::KeepAspectRatio));            }
            }
            else{
                for(int i = 0; i < firingAck; i++){
                    int x = 0;
                    do{
                        x = std::rand() % attackerSoldiers;
                    } while(std::find(fallenAttackers.begin(), fallenAttackers.end(), x) != fallenAttackers.end());
                    QPixmap attackerSoldierFiringImage = getSoldierFiringImage(results.getAttackerType());
                    QTransform transform;
                    transform.scale(-1, 1);
                    QPixmap flippedAttackerImage = attackerSoldierFiringImage.transformed(transform);
                    attackerImages[x]->setPixmap(flippedAttackerImage.scaled(70, 70, Qt::KeepAspectRatio));              }
            }

            std::time_t startReplay = std::time(0);
            int fallenDef =(defenderSoldiers - fallenDefenders.size()) - std::ceil(static_cast<double>(round.defenderRemaining)/10);
            if (round.defenderRemaining <= 0)
                for (int var = 0; var < defenderImages.size(); var++) {
                    QPixmap defenderSoldierFallenImage = getSoldierFallenImage(results.getDefenderType());
                    defenderImages[var]->setPixmap(defenderSoldierFallenImage.scaled(70, 70, Qt::KeepAspectRatio));
                    if((std::find(fallenDefenders.begin(), fallenDefenders.end(), var) == fallenDefenders.end())){
                        fallenDefenders.push_back(var);
                    }
                }
            else{
                for(int i = 0; i < fallenDef; i++){
                    int x = 0;
                    do{
                        x = std::rand() % defenderSoldiers;
                    } while(std::find(fallenDefenders.begin(), fallenDefenders.end(), x) != fallenDefenders.end());
                    fallenDefenders.push_back(x);
                    QPixmap defenderSoldierFallenImage = getSoldierFallenImage(results.getDefenderType());
                    defenderImages[x]->setPixmap(defenderSoldierFallenImage.scaled(70, 70, Qt::KeepAspectRatio));
                }
            }
            int fallenAtk = (attackerSoldiers - fallenAttackers.size()) - std::ceil(static_cast<double>(round.attackerRemaining)/10);
            if(round.attackerRemaining <= 0){
                for (int var = 0; var < attackerImages.size(); var++) {
                    QPixmap attackerSoldiersFallenImage = getSoldierFallenImage(results.getAttackerType());
                    QTransform transform;
                    transform.scale(-1, 1);
                    QPixmap flippedAttackerImage = attackerSoldiersFallenImage.transformed(transform);
                    attackerImages[var]->setPixmap(flippedAttackerImage.scaled(70, 70, Qt::KeepAspectRatio));
                    if((std::find(fallenAttackers.begin(), fallenAttackers.end(), var) == fallenAttackers.end())){
                        fallenAttackers.push_back(var);
                    }
                }
            }
            else{
                for(int i = 0; i < fallenAtk; i++){
                    int x = 0;
                    do{
                        x = std::rand() % attackerSoldiers;
                    } while(std::find(fallenAttackers.begin(), fallenAttackers.end(), x) != fallenAttackers.end());
                    fallenAttackers.push_back(x);
                    QPixmap attackerSoldiersFallenImage = getSoldierFallenImage(results.getAttackerType());
                    QTransform transform;
                    transform.scale(-1, 1);
                    QPixmap flippedAttackerImage = attackerSoldiersFallenImage.transformed(transform);
                    attackerImages[x]->setPixmap(flippedAttackerImage.scaled(70, 70, Qt::KeepAspectRatio));            }
            }
            while (std::difftime(std::time(0), startReplay) < 1) {
                QCoreApplication::processEvents();
            }
            for (int var = 0; var < attackerImages.size(); var++) {
                if((std::find(fallenAttackers.begin(), fallenAttackers.end(), var) == fallenAttackers.end())){
                    QPixmap standingSoldierImage = getSoldierStandingImage(results.getAttackerType());
                    QTransform transform;
                    transform.scale(-1, 1);
                    QPixmap flippedAttackerImage = standingSoldierImage.transformed(transform);
                    attackerImages[var]->setPixmap(flippedAttackerImage.scaled(70, 70, Qt::KeepAspectRatio));
                }
            }
            for (int var = 0; var < defenderImages.size(); var++) {
                if((std::find(fallenDefenders.begin(), fallenDefenders.end(), var) == fallenDefenders.end())){
                    QPixmap standingSoldierImage =getSoldierStandingImage(results.getDefenderType());
                    defenderImages[var]->setPixmap(standingSoldierImage.scaled(70, 70, Qt::KeepAspectRatio));
                }
            }
        }
        if(results.getWinner()->armyType() == results.getDefenderType() && results.getRounds()[results.getRounds().size()-1].attackerRemaining> 0){
            for (int var = 0; var < attackerImages.size(); var++) {
                if((std::find(fallenAttackers.begin(), fallenAttackers.end(), var) == fallenAttackers.end())){
                    QPixmap retreatImage = QPixmap(QString(":/resources/Images/RetreatFlag.png"));
                    attackerImages[var]->setPixmap(retreatImage.scaled(70, 70, Qt::KeepAspectRatio));
                }
            }
        }
        if(results.getWinner()->armyType() == results.getAttackerType() &&  results.getRounds()[results.getRounds().size()-1].defenderRemaining> 0){
            for (int var = 0; var < defenderImages.size(); var++) {
                if((std::find(fallenDefenders.begin(), fallenDefenders.end(), var) == fallenDefenders.end())){
                    QPixmap retreatImage = QPixmap(QString(":/resources/Images/RetreatFlag.png"));
                    defenderImages[var]->setPixmap(retreatImage.scaled(70, 70, Qt::KeepAspectRatio));
                }
            }
        }
    }
    QLabel *victoryLabel = new QLabel(this);
    QPixmap victoryImage = QPixmap(QString(":/resources/Images/Victory.png"));
    victoryLabel->setPixmap(victoryImage.scaled(300, 100, Qt::IgnoreAspectRatio));
    victoryLabel->setAlignment(Qt::AlignCenter);
    QLabel *loseLabel = new QLabel(this);
    QPixmap loseImage = QPixmap(QString(":/resources/Images/Lose.png"));
    loseLabel->setPixmap(loseImage.scaled(300, 80, Qt::IgnoreAspectRatio));
    loseLabel->setAlignment(Qt::AlignCenter);

    if(results.getWinner()->armyType() == results.getDefenderType()){

        defenderResultLayout->addWidget(victoryLabel);
        if(attackerNumber != 0) attackerResultLayout->addWidget(loseLabel);
    }
    else{
        attackerResultLayout->addWidget(victoryLabel);
        if(defenderNumber != 0)defenderResultLayout->addWidget(loseLabel);
    }
    replayButton->setEnabled(false);
    replayButton->deleteLater();

}
BattleReplayDialog::~BattleReplayDialog() {
}
