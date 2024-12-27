#ifndef EVENT_H
#define EVENT_H

#include <string>
#include"../graph/graph.hpp"
#include "../Entities/Army.h"
#include <vector>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <qpushbutton.h>
#include <QObject>
#include <QDialog>
#include <QPushButton>
#include <QString>
#include <sstream>
#include <cmath>
#include <iostream>
#include <QApplication>

enum class EventType{
    RANDOM,
    HISTORIC,
    MISSION
};

class Event {

public:
    Event();

    Event(unsigned int id, EventType type, const QString& title, const QString& imagePath,
          const QString& description, const QString& buttonText,
          const QVector<QString>& territoryTrigger, const QString& trigger,
          int triggerAmount, const QVector<QString>& territoryAffect,
          const QString& date);

    static EventType stringToEventType(const std::string &str);
    std::string eventTypeToString(EventType type);

    unsigned int id;
    QString getDate() const;
    EventType getType() const;
    void showEventWindow();
    bool canTrigger(int clientId, const QString &currentYear, const graph::Graph &clientGraph) const;

    /*
      "id" : 2 ,
      "type": "HISTORIC",
      "title": "A Sick Man of Europe",
      "image_path": ":/resources/a_sick_man_of_europe.png",
      "description": "The year is 1804. The Ottoman Empire, the guardian of centuries-old traditions and vast dominions, now faces challenges in maintaining order across its extensive lands. In the Balkans, where the Empire has brought stability and governance for generations, unrest stirs among certain factions. Fueled by misguided ambitions and foreign influences, rebellion threatens the harmony of the region. The Serbian people, once loyal subjects, now rise against the authority of the Sultan. It is your duty to restore order, protect the sovereignty of the Empire, and ensure the continued prosperity of the lands under Ottoman rule. Will you quell this insurrection and reassert the strength of the Crescent?",
      "button" : "Quell the Rebellion" ,
      "territory_trigger": [""],
      "trigger": "Intro",
      "trigger_amount": 0,
      "territory_affect": [""],
      "date": "1-1-1804"
     */
private:
    int clientId;
    EventType type;
    QString title;
    QString image_path;
    QString description;
    QString buttonText;
    QVector<QString> territoryTrigger;//ili vertexa videcu
    QString trigger;
    int triggerAmount;
    QVector<QString> territoryAffect;
    QString date;

    //widgets
    QWidget *infoWindow = nullptr;

};


#endif // EVENT_H
