#ifndef CITY_H
#define CITY_H

class City
{
public:
    explicit City(bool isCity = false);
    //~City() = default;
    bool isCityLayer()const;
    int getBattleFactor()const;

private:
    bool isCity;
};

#endif // CITY_H
