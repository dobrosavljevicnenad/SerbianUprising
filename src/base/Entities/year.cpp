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

void Year::setCurrentDate(const QString &dateString) {
    currentDate = QDate::fromString(dateString, "dd MMMM yyyy");
    if (!currentDate.isValid()) {
        qWarning() << "Invalid date format for setCurrentDate:" << dateString;
    }
}

