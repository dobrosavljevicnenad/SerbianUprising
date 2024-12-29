#pragma once

#ifndef STRENGTH_H
#define STRENGTH_H

#include "../Entities/army.h"

enum class ArmyType;

class Strength
{
public:
    static Strength& instance();

    void setBoost(ArmyType type, float boost);
    float getBoost(ArmyType type) const;

    static int baseMorale(ArmyType type);
    static int baseStrength();

private:
    Strength() = default;

    float m_boostHajduk = 10;
    float m_boostJanissary = 10;
    static const int s_baseMoraleHajduk = 0;
    static const int s_baseMoraleJanissary = 0;

    static const int s_baseStrength = 10;
};

#endif // STRENGTH_H
