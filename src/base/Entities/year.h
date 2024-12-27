#ifndef YEAR_H
#define YEAR_H

#include <QString>
#include <QDate>

class Year {
public:
    explicit Year(const QDate& startDate = QDate(1804, 1, 1));
    void advanceThreeMonths();
    QString getCurrentDateString() const;
    QString toJsonDateString() const;

private:
    QDate currentDate;
};

#endif // YEAR_H
