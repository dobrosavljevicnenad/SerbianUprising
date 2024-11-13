#include "city.h"

City::City(bool isCity): isCity(isCity) {}

bool City::isCityLayer() const  {
    return isCity;
}

int City::getBattleFactor() const {
    return isCity ? 5 : 0;
}


