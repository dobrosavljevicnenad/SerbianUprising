#include "event.h"
#include <qscrollarea.h>


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
    clientId=clientId;
    switch (type) {
    case EventType::RANDOM:
        return true;

    case EventType::MISSION:
        for (const QString& label : territoryTrigger) {
            auto vertex = clientGraph.get_vertex_by_label(label);
            if (vertex->army.armyType() != ArmyType::HAJDUK) {
                return false;
            }
        }
        return true;

    case EventType::HISTORIC:
        return date == currentYear;

    default:
        return false;
    }
}

void Event::showEventWindow() {
    infoWindow = new QWidget;
    infoWindow->setWindowFlags(Qt::FramelessWindowHint);
    infoWindow->setMinimumSize(900, 900);

    if (clientId == 1) {
        infoWindow->setStyleSheet(
            "background-color: rgba(74, 47, 47,190); "
            "border-radius: 10px; "
            "border-width: 10px;"
            "border-image: url(:/resources/border1.png) 60 stretch;"
            );
    } else {
        infoWindow->setStyleSheet(
            "background-color: rgba(3, 66, 5,190); "
            "border-radius: 10px; "
            "border-width: 10px;"
            "border-image: url(:/resources/border1.png) 60 stretch;"
            );
    }

    QVBoxLayout* mainLayout = new QVBoxLayout(infoWindow);

    QLabel* titleLabel = new QLabel(title, infoWindow);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFixedSize(820, 100);
    QFont titleFont = titleLabel->font();
    titleFont.setBold(true);
    titleFont.setPointSize(30);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #FFD700; background-color: transparent; border-image: none;");
    mainLayout->addWidget(titleLabel, 0, Qt::AlignCenter);

    QLabel* imageLabel = new QLabel(infoWindow);
    imageLabel->setFixedSize(820, 450);
    imageLabel->setStyleSheet("margin: 10px;border-image:none;");
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setScaledContents(true);

    QPixmap labelImage(image_path);
    labelImage = labelImage.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    imageLabel->setPixmap(labelImage);
    mainLayout->addWidget(imageLabel, 0, Qt::AlignCenter);

    QHBoxLayout* descriptionOutcomeLayout = new QHBoxLayout;

    QLabel* descriptionLabel = new QLabel(description, infoWindow);
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    descriptionLabel->setStyleSheet(
        "font-size: 12pt; "
        "color: #D4AF37; "
        "padding: 10px; "
        "background-color: rgba(0, 0, 0, 128); "
        "border-radius: 5px;"
        "border: 1px solid #A0522D;"
        "border-image:none;"
        );
    descriptionLabel->setFixedSize(656, 200);
    descriptionOutcomeLayout->addWidget(descriptionLabel, 4);

    QLabel* outcomeLabel = new QLabel("Outcome", infoWindow);
    outcomeLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    outcomeLabel->setStyleSheet(
        "font-size: 12pt; "
        "color: #FFFFFF; "
        "padding: 10px; "
        "background-color: rgba(0, 0, 0, 128); "
        "border-radius: 5px; "
        "border: 1px solid #A0522D;"
        "border-image:none;"

        );
    outcomeLabel->setFixedSize(164, 200);
    descriptionOutcomeLayout->addWidget(outcomeLabel, 1);

    mainLayout->addLayout(descriptionOutcomeLayout);

    QPushButton* actionButton = new QPushButton(buttonText, infoWindow);
    actionButton->setFixedSize(820, 50);
    actionButton->setStyleSheet(
        "QPushButton { "
        "   background-color: #8B0000; "
        "   border-radius: 10px; "
        "   color: white;   "
        "   border: 2px solid #5A0000; "
        "   font-size: 12pt; "
        "   padding: 5px;   "
        "   border-image: none; "
        "} "
        "QPushButton:hover { "
        "   background-color: #B22222; "
        "} "
        "QPushButton:pressed { "
        "   background-color: #5A0000; "
        "} "
        );
    QObject::connect(actionButton, &QPushButton::clicked, infoWindow, &QWidget::close);

    mainLayout->addWidget(actionButton, 0, Qt::AlignCenter);

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
