#include "infowidget.h"

NodeInfoWidget::NodeInfoWidget(QMap<MapLayer*, graph::Vertex*> layerToVertex, QWidget* parent)
    : QWidget(parent), layerToVertex(layerToVertex) {
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_StyledBackground, true);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30); // Padding inside the border
    mainLayout->setSpacing(15);

    QHBoxLayout* titleLayout = new QHBoxLayout();
    titleLayout->setAlignment(Qt::AlignTop);
    titleLayout->setContentsMargins(0, 0, 0, 0);

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
    //QHBoxLayout* picturesLayout = new QHBoxLayout();
    imageLabel = new QLabel(this);
    imageLabel->setFixedSize(80, 80);
    imageLabel->setStyleSheet("background: transparent;");
    mainLayout->addWidget(imageLabel, 0, Qt::AlignCenter);

    troopCountLabel = new QLabel("Troop Count: 10", this);
    troopCountLabel->setAlignment(Qt::AlignCenter);
    troopCountLabel->setStyleSheet("color: white;");
    mainLayout->addWidget(troopCountLabel);

    ownerLabel = new QLabel("Owner: Player 2", this);
    ownerLabel->setAlignment(Qt::AlignCenter);
    ownerLabel->setStyleSheet("color: white;");
    mainLayout->addWidget(ownerLabel);

    bioLabel = new QLabel("Relief: mountain", this);
    bioLabel->setWordWrap(true);
    bioLabel->setAlignment(Qt::AlignCenter);
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
            "   border-image: url(:/resources/border1.png) 30 30 30 30 stretch stretch; "
            "   border-width: 4px; "
            "background-color: rgba(74, 47, 47,180); "
            "   color: white; "
            "}");
    }else{
        setStyleSheet(
            "NodeInfoWidget { "
            "   border-image: url(:/resources/border1.png) 30 30 30 30 stretch stretch; "
            "   border-width: 4px; "
            "   background-color: rgba(3, 66, 5,180); "
            "   color: white; "
            "}");
    }
    QPixmap pixmap = layer->get_m_originalPixmap();
    imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QString relief = QString("Relief: %1\n")
                         .arg(QString::fromStdString(vertex->terrain.to_string(vertex->terrain.getTerrain())));
    bioLabel->setText(relief);
}
