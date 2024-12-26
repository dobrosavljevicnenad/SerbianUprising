#include "event.h"


Event::Event(unsigned int id, EventType type, const QString& title, const QString& imagePath,
             const QString& description, const QString& buttonText,
             const QVector<QString>& territoryTrigger, const QString& trigger,
             int triggerAmount, const QVector<QString>& territoryAffect, const QString& date)
    :id(id), type(type),
    title(title),
    image_path(imagePath),
    description(description),
    buttonText(buttonText),
    territoryTrigger(territoryTrigger),
    trigger(trigger),
    triggerAmount(triggerAmount),
    territoryAffect(territoryAffect),
    date(date) {}

EventType Event::getType() const{return type;}

QString Event::getDate() const {return date;}



bool Event::canTrigger(int clientId, const QString& currentYear, const graph::Graph& clientGraph) const {
    switch (type) {
    case EventType::RANDOM:
        return true;

    case EventType::MISSION:
        for (const QString& label : territoryTrigger) {
            auto vertex = clientGraph.get_vertex_by_label(label);
            unsigned m_id;
            if(vertex->army.armyType() == ArmyType::HAJDUK) m_id = 1;
            else if (vertex->army.armyType() == ArmyType::JANISSARY) m_id = 2;
            if (vertex && m_id == clientId) {
                return true;
            }
        }
        return false;

    case EventType::HISTORIC:
        return date == currentYear;

    default:
        return false;
    }
}

void Event::showEventWindow() {
    infoWindow = new QWidget;
    infoWindow->setWindowTitle("Event Details");
    infoWindow->setMinimumSize(600,400);
    infoWindow->setStyleSheet(
        "background-color: #f0e6d2;"
        "border: 4px solid #4e3629;"
        "border-radius: 15px;"
        );

    QVBoxLayout * layout = new QVBoxLayout(infoWindow);

    QString eventType;
    switch(type){
        case EventType::RANDOM:
            eventType = "Random Event";
            break;
        case EventType::HISTORIC:
            eventType = "Historic Event";
            break;
        case EventType:: MISSION:
            eventType = "Mission Event";
            break;
    }

    typeLabel = new QLabel("<h1 style='color: #d7a13d; font-family: \"SimSun\", serif; font-size: 26px;'>"
    "Karadjordje zauzima Cetinje</h1>", infoWindow);
    typeLabel->setAlignment(Qt::AlignCenter);
    typeLabel->setStyleSheet("border: none");
    layout->addWidget(typeLabel);

    QLabel *imageLabel = new QLabel(infoWindow);
    QPixmap label_image(image_path);
    label_image = label_image.scaled(infoWindow->width(),100,Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    imageLabel->setPixmap(label_image);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setStyleSheet("margin-bottom: 15px");
    layout->addWidget(imageLabel);

    outcomeLabel = new QLabel(QString("<p style='color: #6b3f3d; font-size: 16px;'>%1</p>")
                                  .arg(QString::fromStdString("cao")), infoWindow);

    layout->addWidget(outcomeLabel);

    QPushButton *closeButton = new QPushButton("X",infoWindow);
    layout->addWidget(closeButton);
    closeButton->setFixedSize(70,50);
    closeButton->setStyleSheet(        "background-color: #ff6347;"
                               "border: 1px solid #8b4513;"
                               "border-radius: 5px;"
                               "text-align: center;"
                               "font-size: 14px; padding: 8px;");


    gainLabel = new QLabel(QString("<p style='color: #006400;'>Outcome Gain: %1</p>").arg(23), infoWindow);
    gainLabel->setStyleSheet("boder : none;");
    layout->addWidget(gainLabel);

    territoryCountLabel = new QLabel(QString("<p style='color: #4b0082;'>Territories Affected: %1</p>").arg(territoryAffect.size()), infoWindow);
    layout->addWidget(territoryCountLabel);

    QObject::connect(closeButton, &QPushButton::clicked,infoWindow,&QWidget::close);

    infoWindow->show();
}

EventType Event::stringToEventType(const std::string& str) {
    if (str == "RANDOM") {
        return EventType::RANDOM;
    } else if (str == "HISTORIC") {
        return EventType::HISTORIC;
    } else if (str == "MISSION") {
        return EventType::MISSION;
    } else {
        throw std::invalid_argument("Unknown EventType string: " + str);
    }
}

std::string Event::eventTypeToString(EventType type) {
    switch (type) {
    case EventType::RANDOM:
        return "RANDOM";
    case EventType::HISTORIC:
        return "HISTORIC";
    case EventType::MISSION:
        return "MISSION";
    default:
        throw std::invalid_argument("Unknown EventType enum value.");
    }
}
