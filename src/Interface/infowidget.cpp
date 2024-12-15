#include "infowidget.h"

NodeInfoWidget::NodeInfoWidget(QMap<MapLayer*, graph::Vertex*> layerToVertex, QWidget* parent)
    : QWidget(parent), layerToVertex(layerToVertex) {
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    setStyleSheet("background-color: rgba(0, 0, 0, 153); border-radius: 10px; color: white; padding: 10px;");

    QVBoxLayout* layout = new QVBoxLayout(this);

    QHBoxLayout* titleLayout = new QHBoxLayout();
    titleLabel = new QLabel("Node Information", this);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 16px; background-color: rgba(255, 255, 255, 153); padding: 5px;");
    titleLabel->setAlignment(Qt::AlignLeft);
    titleLabel->setStyleSheet("opacity: 0.6;");

    closeButton = new QPushButton("X", this);
    closeButton->setStyleSheet(
        "QPushButton { "
        "   background-color: red; "
        "   border-radius: 5px; "
        "   color: white; "
        "} "
        "QPushButton:hover { "
        "   background-color: darkRed; "
        "} "
        );
    closeButton->setFixedSize(20, 20);
    connect(closeButton, &QPushButton::clicked, this, &NodeInfoWidget::hide);

    titleLayout->addWidget(titleLabel);
    titleLayout->addWidget(closeButton, 0, Qt::AlignRight);
    layout->addLayout(titleLayout);

    imageLabel = new QLabel(this);
    imageLabel->setFixedSize(80, 80);
    imageLabel->setStyleSheet("border: 1px solid white; border-radius: 10px;");
    layout->addWidget(imageLabel, 0, Qt::AlignCenter);

    troopCountLabel = new QLabel("Troop Count: 0", this);
    layout->addWidget(troopCountLabel);

    ownerLabel = new QLabel("Owner: None", this);
    layout->addWidget(ownerLabel);

    bioLabel = new QLabel("Relief: N/A\nExplanation: N/A", this);
    bioLabel->setWordWrap(true);
    layout->addWidget(bioLabel);

    setLayout(layout);
    setFixedSize(300, 400);
}

void NodeInfoWidget::updateNodeInfo(MapLayer* layer) {
    if (!layer) return;

    auto vertex = layerToVertex[layer];
    titleLabel->setText(QString::fromStdString(vertex->label()));

    troopCountLabel->setText(QString("Troop Count: %1").arg(layer->getTroopCount()));
    ownerLabel->setText(QString("Owner: Player %1").arg(vertex->player.getPlayerId()));

    QPixmap pixmap = layer->get_m_originalPixmap();
    imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QString relief = QString("Relief: %1\n")
                         .arg(QString::fromStdString(vertex->terrain.to_string(vertex->terrain.getTerrain())));
    bioLabel->setText(relief);
}
