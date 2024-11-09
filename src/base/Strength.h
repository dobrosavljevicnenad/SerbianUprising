#ifndef STRENGTH_H
#define STRENGTH_H

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

    int m_boostHajduk;
    int m_boostJanissary;
    static const int s_baseMorale = 0;   // Base morale, consistent across all instances
    static const int s_baseStrength = 30; // Base strength, consistent across all instances
};

#endif // STRENGTH_H
