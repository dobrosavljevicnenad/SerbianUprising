#include "infowidget.h"

NodeInfoWidget::NodeInfoWidget(QMap<MapLayer*, graph::Vertex*> layerToVertex, QWidget* parent)
    : QWidget(parent), layerToVertex(layerToVertex) {
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_StyledBackground, true);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(32, 0, 32, 100);
    mainLayout->setSpacing(2);

    QHBoxLayout* titleLayout = new QHBoxLayout();
    titleLayout->setAlignment(Qt::AlignTop);
    titleLayout->setContentsMargins(0, 32, 0, 0);

    titleLabel = new QLabel("Node Information", this);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 16px; color: white;");
    titleLabel->setAlignment(Qt::AlignLeft);

    closeButton = new QPushButton("X", this);
    closeButton->setFixedSize(20, 20);
    closeButton->setStyleSheet(
        "QPushButton { background-color: red; border-radius: 10px; color: white; } "
        "QPushButton:hover { background-color: darkRed; }");
    connect(closeButton, &QPushButton::clicked, this, &NodeInfoWidget::hide);

    titleLayout->addWidget(titleLabel, 1);           // Left aligned title
    titleLayout->addWidget(closeButton, 0, Qt::AlignTop | Qt::AlignRight); // Right aligned close button

    mainLayout->addLayout(titleLayout);

    QHBoxLayout* picturesLayout = new QHBoxLayout();
    picturesLayout->setAlignment(Qt::AlignTop);
    picturesLayout->setSpacing(4);
    picturesLayout->setContentsMargins(0, 0, 0, 0);

    imageLabel = new QLabel(this);
    imageLabel->setFixedSize(80, 80);
    imageLabel->setStyleSheet("background: transparent;");
    picturesLayout->addWidget(imageLabel, 0, Qt::AlignCenter);

    backgroundImageLabel = new QLabel(this);
    backgroundImageLabel->setFixedSize(210, 80);
    backgroundImageLabel->setStyleSheet("background: transparent;");
    picturesLayout->addWidget(backgroundImageLabel, 0, Qt::AlignCenter);

    mainLayout->addLayout(picturesLayout);

    troopCountLabel = new QLabel("Troop Count: 10", this);
    troopCountLabel->setAlignment(Qt::AlignLeft);
    troopCountLabel->setStyleSheet("color: white;");
    mainLayout->addWidget(troopCountLabel);

    cityLabel = new QLabel(this);
    cityLabel->setAlignment(Qt::AlignLeft);
    cityLabel->setStyleSheet("color: white;");
    mainLayout->addWidget(cityLabel);

    regionLabel = new QLabel(this);
    regionLabel->setAlignment(Qt::AlignLeft);
    regionLabel->setStyleSheet("color: white;");
    mainLayout->addWidget(regionLabel);

    ownerLabel = new QLabel("Owner: Player 2", this);
    ownerLabel->setAlignment(Qt::AlignLeft);
    ownerLabel->setStyleSheet("color: white;");
    mainLayout->addWidget(ownerLabel);

    bioLabel = new QLabel("Relief: mountain", this);
    bioLabel->setWordWrap(true);
    bioLabel->setAlignment(Qt::AlignLeft);
    bioLabel->setStyleSheet("color: white;");
    mainLayout->addWidget(bioLabel);

    setLayout(mainLayout);
    setFixedSize(320, 450);
}

void NodeInfoWidget::updateNodeInfo(MapLayer* layer) {
    if (!layer) return;

    auto vertex = layerToVertex[layer];
    titleLabel->setText(QString::fromStdString(vertex->label()));

    troopCountLabel->setText(QString("Troop Count: %1").arg(layer->getTroopCount()));
    ownerLabel->setText(QString("Owner: Player %1").arg(vertex->player.getPlayerId()));
    if(vertex->army.armyType()== ArmyType::HAJDUK){
        setStyleSheet(
            "NodeInfoWidget { "
            "   border-image: url(:/resources/border1.png) 30 stretch; "
            "   border-width: 4px; "
            "   background-color: rgba(74, 47, 47,180); "
            "   color: white; "
            "}");
    }else{
        setStyleSheet(
            "NodeInfoWidget { "
            "   border-image: url(:/resources/border1.png) 30 stretch; "
            "   border-width: 4px; "
            "   background-color: rgba(3, 66, 5,180); "
            "   color: white; "
            "}");
    }
    QPixmap pixmap = layer->get_m_originalPixmap();
    imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    if (vertex->terrain.getTerrain() == TerrainType::HILL) {
        QPixmap hillPixmap(":/resources/hills.png");
        backgroundImageLabel->setPixmap(hillPixmap.scaled(backgroundImageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else if (vertex->terrain.getTerrain() == TerrainType::FIELD) {
        QPixmap fieldPixmap(":/resources/field.png");
        backgroundImageLabel->setPixmap(fieldPixmap.scaled(backgroundImageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else if (vertex->terrain.getTerrain() == TerrainType::FOREST) {
        QPixmap forestPixmap(":/resources/forest.png");
        backgroundImageLabel->setPixmap(forestPixmap.scaled(backgroundImageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else {
        QPixmap mountainPixmap(":/resources/mountain.png");
        backgroundImageLabel->setPixmap(mountainPixmap.scaled(backgroundImageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    QString relief = QString("Relief: %1\n")
                         .arg(QString::fromStdString(vertex->terrain.to_string(vertex->terrain.getTerrain())));
    bioLabel->setText(relief);

    cityLabel->setText(QString("CityLevel: %1").arg(vertex->city->getLevel()));

    if (vertex->region) {
        regionLabel->setText(QString("Region: %1").arg(QString::fromStdString(vertex->region->getRegionName())));
    } else {
        regionLabel->setText("Region: None");
    }
}
