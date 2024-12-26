#pragma once

#ifndef STRENGTH_H
#define STRENGTH_H

#include "../Entities/army.h"

enum class ArmyType;

class Strength
{
public:
    static Strength& instance();

    void setBoost(ArmyType type, int boost);
    int getBoost(ArmyType type) const;

    static int baseMorale();
    static int baseStrength();

private:
    Strength() = default;

    int m_boostHajduk = 10;
    int m_boostJanissary = 0;
    static const int s_baseMorale = 0;   // Base morale, consistent across all instances
    static const int s_baseStrength = 10; // Base strength, consistent across all instances
};

#endif // STRENGTH_H
