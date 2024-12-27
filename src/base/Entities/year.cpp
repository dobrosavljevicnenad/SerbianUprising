#include "year.h"

Year::Year(const QDate& startDate) : currentDate(startDate) {}

void Year::advanceThreeMonths() {
    currentDate = currentDate.addMonths(3);
}

QString Year::toJsonDateString() const {
    return currentDate.toString("dd-MM-yyyy");
}

QString Year::getCurrentDateString() const {
    return currentDate.toString("dd MMMM yyyy");
}
