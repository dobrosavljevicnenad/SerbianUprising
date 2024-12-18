#pragma once

#ifndef CITY_H
#define CITY_H

class City
{
public:
    explicit City(int level = 0, int battleFactor = 0);
    ~City() = default;

    bool isCityLayer() const;
    int getBattleFactor() const;
    int getLevel() const;

private:
    int level;
    int battleFactor;
};

class CityFactory
{
public:
    static City* getCityByLevel(int level);
};

#endif // CITY_H
