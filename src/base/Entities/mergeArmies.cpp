#include "mergeArmies.h"

void MergeArmies::setTerrainAdvantage(int advantage1, int advantage2) {
    m_defenderAdvantage = advantage1;
    m_attackerAdvantage = advantage2;
}
MergeArmies::MergeArmies(std::vector<Army>&armies) {
    this->armies = armies;
}
Army MergeArmies::battleArmies(Player& player1,Player& player2, std::vector<Army>& armies) {
    int disorganised = -10;
    int totalStrengthfirstPlayer = 0;
    int remainingStrength = 0;

    Player firstPlayer;
    Player secondPlayer;
    if(player1.getPlayerId() == 1){
        firstPlayer = player1;
        secondPlayer = player2;
    }
    else{
        firstPlayer = player2;
        secondPlayer = player1;
    }
    std::cout << "Starting battle between "
              << (firstPlayer.getArmyType() == ArmyType::HAJDUK ? "Hajduk" : "Janissary")
              << " and "
              << (secondPlayer.getArmyType() == ArmyType::HAJDUK ? "Hajduk" : "Janissary") << "!" << std::endl;


    for (auto& army : armies) {
        if(army.armyType() == firstPlayer.getArmyType()){
            totalStrengthfirstPlayer+=army.getSoldiers();
        }
    }
    if(armies[0].armyType() ==  firstPlayer.getArmyType()){
        armies[0].setSoldiers(totalStrengthfirstPlayer);
    }
    else{
        Army attackArmy(totalStrengthfirstPlayer, firstPlayer.getArmyType());
        Battle battle(armies[0],attackArmy);
        battle.setTerrainAdvantage(m_defenderAdvantage,m_attackerAdvantage);
        armies[0] = *battle.start().getWinner();
    }
    for(auto& army : armies){
        if(army.armyType() == secondPlayer.getArmyType()){
            remainingStrength+=army.getSoldiers();
        }
    }

    if(armies[0].armyType() == secondPlayer.getArmyType()){
        armies[0].setSoldiers(remainingStrength);
    }else{
        std::cout << "Counter attack of "
                  << (secondPlayer.getArmyType() == ArmyType::HAJDUK ? "Hajduk" : "Janissary") << "!" << std::endl;
        Army counterAttackArmy (remainingStrength, secondPlayer.getArmyType());
        Battle battle2(armies[0], counterAttackArmy);
        battle2.setTerrainAdvantage(m_defenderAdvantage+ disorganised,m_attackerAdvantage);
        armies[0] = *battle2.start().getWinner();
    }
    while(armies.size()!=1)
        armies.pop_back();
    std::cout << (armies[0].armyType() == ArmyType::HAJDUK ? "Hajduk" : "Janissary")
              << " wins!" << std::endl;
    return armies[0];
}
