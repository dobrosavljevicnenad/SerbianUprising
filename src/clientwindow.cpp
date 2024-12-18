#include "clientwindow.h"
#include <QVBoxLayout>
#include <QGraphicsProxyWidget>
#include <QInputDialog>
#include <QMessageBox>
#include <QGraphicsView>
#include<QListWidget>
#include<QDebug>
#include <qapplication.h>
#include <qgraphicseffect.h>

ClientWindow::ClientWindow(ClientGameManager *existingGameManager,QWidget *parent)
    : QMainWindow(parent),
    scene(new MapScene(this)),
    gameManager(existingGameManager),
    selectedLayer(nullptr),
    moveList(new QListWidget())
{
    setupGame();
    setupUI();
    connectSignals();
    gameManager->initializeUI(headerLabel, endTurnButton, moveButton, infoButton,moveList,
                              armyButton,reliefButton,regionsButton,cityButton,defaultButton,nodeInfoWidget);
    connect(gameManager, &ClientGameManager::gameYearUpdated, this, &ClientWindow::updateYearLabel);

}

ClientWindow::~ClientWindow() {
    qDebug() << "Destroying ClientWindow...";
}

void ClientWindow::setupGame() {
    view = new ZoomableGraphicsView(this);
    view->setScene(scene);
    view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    view->setFocusPolicy(Qt::StrongFocus);
    view->setFocus();
    setCentralWidget(view);
    gameManager->setScene(scene);
    gameManager->initializeGraphics();
}

void ClientWindow::setupUI() {

    nodeInfoWidget = new NodeInfoWidget(gameManager->layerToVertex,this);
    nodeInfoWidget->hide();

    layoutContainer = new QWidget(view->viewport());
    layoutContainer->setStyleSheet("background-color: rgba(0, 0, 0, 128);"
                                   "border-radius: 15px;");
    QVBoxLayout *mainLayout = new QVBoxLayout();

    mainLayout->setSpacing(5);
    mainLayout->setContentsMargins(2, 2, 2, 2);

    // Header Label
    QString turnLabel = QString("Turn %1").arg(1);
    headerLabel = new QLabel(turnLabel);
    QFont font = headerLabel->font();
    font.setBold(true);
    font.setPointSize(12);
    headerLabel->setFont(font);
    headerLabel->setAlignment(Qt::AlignCenter);
    headerLabel->setStyleSheet("color: black; background-color: transparent;");
    mainLayout->addWidget(headerLabel);

    QString yearLabel = QString("Year: %1").arg(gameManager->year().getCurrentDateString());
    yearDisplayLabel = new QLabel(yearLabel);
    QFont yearFont = yearDisplayLabel->font();
    yearFont.setBold(true);
    yearFont.setPointSize(10);
    yearDisplayLabel->setFont(yearFont);
    yearDisplayLabel->setAlignment(Qt::AlignCenter);
    yearDisplayLabel->setStyleSheet("color: darkGreen; background-color: transparent;");
    mainLayout->addWidget(yearDisplayLabel);


    // Buttons Row
    QHBoxLayout *buttonRowLayout = new QHBoxLayout();
    infoButton = new QPushButton("Info");
    moveButton = new QPushButton("Move");
    armyButton = new QPushButton("Army");

    QString buttonStyle =
        "QPushButton { "
        "   background-color: darkGray; "
        "   border-radius: 20px; "
        "} "
        "QPushButton:hover { "
        "   background-color: Green; "
        "} "
        "QPushButton:pressed { "
        "   background-color: darkGreen; "
        "} ";

    infoButton->setStyleSheet(buttonStyle);
    moveButton->setStyleSheet(buttonStyle);
    armyButton->setStyleSheet(
        "QPushButton { "
        "   background-color: darkGreen; "
        "   border-radius: 20px; "
        "} "
        );

    infoButton->setFixedSize(40, 40);
    moveButton->setFixedSize(40, 40);
    armyButton->setFixedSize(40, 40);
    this->activeButton = armyButton;

    buttonRowLayout->addWidget(infoButton);
    buttonRowLayout->addWidget(moveButton);
    buttonRowLayout->addWidget(armyButton);
    buttonRowLayout->setSpacing(3);
    buttonRowLayout->setContentsMargins(0, 0, 0, 0);

    mainLayout->addLayout(buttonRowLayout);

    // End Turn Button
    endTurnButton = new QPushButton("End Turn");
    endTurnButton->setFixedSize(160, 30);
    endTurnButton->setStyleSheet(
        "QPushButton { "
        "   background-color: black; "
        "   color: white; "
        "   border-radius: 10px; "
        "   padding: 5px; "
        "} "
        "QPushButton:hover { "
        "   background-color: darkGray; "
        "} "
        "QPushButton:pressed { "
        "   background-color: black; "
        "}"
        );
    mainLayout->addWidget(endTurnButton, 0, Qt::AlignCenter);

    // Move List
    moveList = new QListWidget(view->viewport());
    moveList->setFixedSize(200, 300);
    moveList->setStyleSheet(
        "QListWidget { "
        "   background-color: darkGray; "
        "   border: 1px solid black; "
        "   border-radius: 5px; "
        "}"
        );
    mainLayout->addWidget(moveList, 0, Qt::AlignCenter);

    QPushButton* toggleButton = new QPushButton("⮝", layoutContainer);
    toggleButton->setFixedSize(40, 40);
    toggleButton->setStyleSheet(
        "QPushButton { "
        "   background-color: lightGray; "
        "   border-radius: 10px; "
        "} "
        "QPushButton:hover { "
        "   background-color: darkGray; "
        "} "
        "QPushButton:pressed { "
        "   background-color: black; "
        "   color: white; "
        "}"
        );

    mainLayout->addWidget(toggleButton, 0, Qt::AlignCenter);

    layoutContainer->setLayout(mainLayout);
    layoutContainer->setGeometry(10, 10, 250, 520);

    mapModeContainer = new QWidget(view->viewport());
    mapModeContainer->setStyleSheet("background-color: rgba(0, 0, 0, 128); border-radius: 10px;");

    QHBoxLayout* mapModeLayout = new QHBoxLayout();
    mapModeLayout->setSpacing(5);
    mapModeLayout->setContentsMargins(5, 5, 5, 5);

    // Map Mode Buttons
    reliefButton = new QPushButton("Relief");
    regionsButton = new QPushButton("Regions");
    cityButton = new QPushButton("City");
    defaultButton = new QPushButton("Main");

    // Button Style
    QString mapButtonStyle =
        "QPushButton { background-color: darkGray; color: white; border-radius: 5px; padding: 5px; }"
        "QPushButton:hover { background-color: gray; }"
        "QPushButton:pressed { background-color: darkGreen; }";

    reliefButton->setStyleSheet(mapButtonStyle);
    regionsButton->setStyleSheet(mapButtonStyle);
    cityButton->setStyleSheet(mapButtonStyle);
    defaultButton->setStyleSheet(mapButtonStyle);

    // Add buttons to layout
    mapModeLayout->addWidget(reliefButton,0, Qt::AlignCenter);
    mapModeLayout->addWidget(regionsButton,0, Qt::AlignCenter);
    mapModeLayout->addWidget(cityButton,0, Qt::AlignCenter);
    mapModeLayout->addWidget(defaultButton,0, Qt::AlignCenter);

    mapModeContainer->setLayout(mapModeLayout);
    // Move to bottom-left corner of the viewport
    mapModeContainer->move(view->viewport()->rect().bottomLeft() + QPoint(10, -60)); // Adjust position



    QList<QWidget*> layoutWidgets = {headerLabel, infoButton, moveButton, armyButton, endTurnButton, moveList};

    connect(toggleButton, &QPushButton::clicked, this, [=]() {
        static bool isExpanded = true;

        if (isExpanded) {
            for (auto* widget : layoutWidgets) {
                widget->hide();
            }
            layoutContainer->setFixedSize(250, toggleButton->height() + 10);
            layoutContainer->setStyleSheet("background-color: transparent");
            toggleButton->setText("⮟");
        } else {
            for (auto* widget : layoutWidgets) {
                widget->show();
            }
            layoutContainer->setFixedSize(250, 520);
            layoutContainer->setStyleSheet("background-color: rgba(0, 0, 0, 128);"
                                           "border-radius: 15px;");
            toggleButton->setText("⮝");
        }
        isExpanded = !isExpanded;
    });

}


void ClientWindow::updateYearLabel(QString newYear) {
    yearDisplayLabel->setText(newYear);
}
void ClientWindow::connectSignals() {
    connect(reliefButton, &QPushButton::clicked, this, [this]() {
        gameManager->applyMapMode(ClientGameManager::MapMode::Relief);
    });
    connect(regionsButton, &QPushButton::clicked, this, [this]() {
        gameManager->applyMapMode(ClientGameManager::MapMode::Regions);
    });
    connect(cityButton, &QPushButton::clicked, this, [this]() {
        gameManager->applyMapMode(ClientGameManager::MapMode::CityLevel);
    });
    connect(defaultButton, &QPushButton::clicked, this, [this]() {
        gameManager->applyMapMode(ClientGameManager::MapMode::Default);
    });
    connect(view->horizontalScrollBar(), &QScrollBar::valueChanged, this, [this]() { repositionFixedWidgets(); });
    connect(view->verticalScrollBar(), &QScrollBar::valueChanged, this, [this]() { repositionFixedWidgets(); });
    connect(gameManager, &ClientGameManager::layerClicked, this, &ClientWindow::onLayerClicked);
    connect(endTurnButton, &QPushButton::clicked, this, &ClientWindow::onEndTurnClicked);
    connect(moveList, &QListWidget::itemClicked, this, &ClientWindow::onMoveClicked);
    connect(infoButton, &QPushButton::clicked, this, &ClientWindow::onInfoButtonClicked);
    connect(moveButton, &QPushButton::clicked, this, &ClientWindow::onMoveButtonClicked);
    connect(armyButton, &QPushButton::clicked, this, &ClientWindow::onPlaceButtonClicked);
}

void ClientWindow::onEndTurnClicked() {
    if (!gameManager) {
        qWarning() << "Game manager is not available!";
        return;
    }
    if(gameManager->getArmyManager().totalTroops != 0){
        QMessageBox::warning(nullptr, "Army", "Deploy all army first!");
        return;
    }
    QVector<Action> actions = gameManager->actionBuffer;
    int clientId = gameManager->ClientId;

    gameManager->EndTurnClicked(actions, clientId);
    gameManager->disableInteractions();
}

void ClientWindow::repositionFixedWidgets()
{
    layoutContainer->move(view->viewport()->rect().topLeft() + QPoint(10, 10));
    mapModeContainer->move(view->viewport()->rect().bottomLeft() + QPoint(10, -60));
}

void ClientWindow::processEndTurnClicked(){
     // Show dialog modally
}

void ClientWindow::onMoveClicked(QListWidgetItem* item) {
    if (!item) return;

    QString actionType = item->data(Qt::UserRole + 2).toString();

    if (actionType == "Move") {
        QVariant data = item->data(Qt::UserRole);
        if (!data.isValid()) return;

        int actionId = data.toInt();
        gameManager->removeActionById(actionId);
        gameManager->removeArrowByActionId(actionId);
        delete item;
    } else if (actionType == "Place") {
        int layerId = item->data(Qt::UserRole).toInt();
        int troopsToRemove = item->data(Qt::UserRole + 1).toInt();

        MapLayer* layer = nullptr;
        for (auto pair : gameManager->layerToVertex) {
            if (pair->id() == layerId) {
                layer = pair->map_layer;
                break;
            }
        }

        if (layer) {
            AddArmyManager& armyManager = gameManager->getArmyManager();
            armyManager.decreaseAvailableTroops(-troopsToRemove);
            QVariant data = item->data(Qt::UserRole + 3);
            int actionId = data.toInt();
            gameManager->removePlaceAction(actionId);
        }

        delete moveList->takeItem(moveList->row(item));
    }
}

void ClientWindow::onLayerClicked(MapLayer* layer) {

    if (activeButton == moveButton) {
        handleMoveArmy(layer);
    } else if (activeButton == armyButton) {
        handlePlaceArmy(layer);
    } else if (activeButton == infoButton) {
        handleInfomation(layer);
    } else {
        QMessageBox::warning(this, tr("Unknown Action"), tr("This action is not supported."));
    }

}

void ClientWindow::handleInfomation(MapLayer* layer){
    if (layer) {
        nodeInfoWidget->updateNodeInfo(layer);
        QPoint bottomRight = view->viewport()->rect().bottomRight();
        QPoint globalPos = view->mapToGlobal(bottomRight) - QPoint(nodeInfoWidget->width() + 10, nodeInfoWidget->height() + 10);
        nodeInfoWidget->move(globalPos);
        nodeInfoWidget->show();
    } else {
        QMessageBox::information(this, "Info", "No layer selected!");
    }
}

void ClientWindow::handleMoveArmy(MapLayer* layer){
    if (selectedLayer == nullptr) {
        selectedLayer = layer;
        graph::Vertex* selected_vertex = gameManager->layerToVertex[selectedLayer];
        if(selected_vertex->player.getPlayerId() != gameManager->ClientId){
            selectedLayer = nullptr;
            return ;
        }
    } else {
        if(selectedLayer == layer) {
            QMessageBox::warning(this, tr("Error"), tr("You selected the same layer. Select another layer."));
            selectedLayer = nullptr;
            return ;
        }
        graph::Vertex* selected_vertex = gameManager->layerToVertex[selectedLayer];
        graph::Vertex* vertex = gameManager->layerToVertex[layer];

        bool ok;
        int maxTroops = selected_vertex->army.getSoldiers();

        int troopsToTransfer = QInputDialog::getInt(this, tr("Transfer Troops"), tr("Enter the number of soldiers to transfer:"), 0, 0, maxTroops, 1, &ok);
        if (ok) {
            //Action
            ActionType type = (selected_vertex->army.armyType() == vertex->army.armyType()) ? ActionType::MOVE_ARMY : ActionType::ATTACK;
            if(selected_vertex == vertex)
                type = ActionType::PLACE_ARMY;

            int pid = gameManager->ClientId;
            int source = selected_vertex->id();
            int target = vertex->id();

            Action newAction(type, pid, source, target, troopsToTransfer);

            selected_vertex->army.setSoldiers(maxTroops - troopsToTransfer);
            selectedLayer->setTroopCount(selected_vertex->army.getSoldiers());


            gameManager->drawArrow(gameManager->ClientId,selectedLayer, layer, troopsToTransfer, newAction.id);
            gameManager->addAction(newAction);


            //TODO
            //ONLY HIGHLIGHT NEIGHBOR PROVINCE WHEN PRESSED AND ALSO
            //DON'T ALLOW CLICKS ON NOT NEIGHBOUR PROVINCE OF FIRST CLICKED
            //buffer and textfield
            QString move = gameManager->GetCurrentAction(newAction);
            QListWidgetItem* item = new QListWidgetItem(move);
            item->setData(Qt::UserRole, newAction.id);
            item->setData(Qt::UserRole + 2, "Move");
            moveList->addItem(item);
        }
        selectedLayer = nullptr;
    }
}

void ClientWindow::handlePlaceArmy(MapLayer* layer){
    int currentPlayerId = gameManager->ClientId;
    graph::Vertex* selected_vertex = gameManager->layerToVertex[layer];
    qDebug() << selected_vertex->player.getPlayerId() << "=" << gameManager->ClientId;
    if ( selected_vertex->player.getPlayerId() == currentPlayerId ) {
        AddArmyManager& armyManager = gameManager->getArmyManager();

        bool ok;

        if(armyManager.totalTroops == 0){
            QMessageBox::warning(this, tr("Error"), tr("Not enought avalible soldiers"));
            selectedLayer = nullptr;
            return;
        }
        std::string message = "Enter the number of soldiers up to " + std::to_string(armyManager.totalTroops) + " to place: ";
        int troopsToAdd = QInputDialog::getInt(this, tr("Place Army"),
                                               tr(message.c_str()), 0, 0, armyManager.totalTroops, 1, &ok);
        if (ok && troopsToAdd > 0) {
            layer->setTroopCount(layer->getTroopCount() + troopsToAdd);
            int pid = gameManager->ClientId;
            int source = selected_vertex->id();

            Action newAction(ActionType::PLACE_ARMY, pid, source, 0, troopsToAdd);
            gameManager->addAction(newAction);
            armyManager.decreaseAvailableTroops(troopsToAdd);

            selected_vertex->army.setSoldiers(selected_vertex->army.getSoldiers()+troopsToAdd);

            QString moveDescription = QString("Placed %1 troops on %2").arg(troopsToAdd).arg(selected_vertex->id());
            QListWidgetItem* item = new QListWidgetItem(moveDescription);
            item->setData(Qt::UserRole, QVariant::fromValue(selected_vertex->id()));
            item->setData(Qt::UserRole + 1, QVariant(troopsToAdd));
            item->setData(Qt::UserRole + 2, "Place");
            item->setData(Qt::UserRole + 3, newAction.id);

            moveList->addItem(item);
        }
    }
}


void ClientWindow::onInfoButtonClicked() {
    setActiveButton(qobject_cast<QPushButton*>(sender()));
}

void ClientWindow::onMoveButtonClicked() {
    setActiveButton(qobject_cast<QPushButton*>(sender()));
}

void ClientWindow::onPlaceButtonClicked() {
    setActiveButton(qobject_cast<QPushButton*>(sender()));
}

void ClientWindow::setActiveButton(QPushButton* clickedButton) {
    QString defaultStyle =
        "QPushButton { "
        "   background-color: darkGray; "
        "   border-radius: 20px; "
        "} "
        "QPushButton:hover { "
        "   background-color: Green; "
        "} "
        "QPushButton:pressed { "
        "   background-color: darkGreen; "
        "} ";

    QString activeStyle =
        "QPushButton { "
        "   background-color: Green; "
        "   color: white; "
        "   border-radius: 20px; "
        "} ";

    if (activeButton) {
        activeButton->setStyleSheet(defaultStyle);
    }

    if (clickedButton) {
        clickedButton->setStyleSheet(activeStyle);
        activeButton = clickedButton;
    }
}

void ClientWindow::clearExplosions()
{
    gameManager->clearExplosions();
}

void ClientWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_I) {  // Kada pritisnete 'I', aktivirajte info dugme
        setActiveButton(infoButton);
    } else if (event->key() == Qt::Key_M) {  // Kada pritisnete 'M', aktivirajte move dugme
        setActiveButton(moveButton);
    } else if (event->key() == Qt::Key_A) {  // Kada pritisnete 'A', aktivirajte army dugme
        setActiveButton(armyButton);
    } else if (event->key() == Qt::Key_Escape) {
        showPauseMenu();
    } else {
        QMainWindow::keyPressEvent(event);
    }
}

void ClientWindow::showPauseMenu() {
    if (findChild<QWidget*>("PauseMenuOverlay")) {
        findChild<QWidget*>("PauseMenuOverlay")->deleteLater();
        return;
    }

    QWidget *overlay = new QWidget(this);
    overlay->setObjectName("PauseMenuOverlay");
    overlay->setStyleSheet("background-color: rgba(0, 0, 0, 120);");
    overlay->setGeometry(this->rect());

    QWidget *menuWidget = new QWidget(overlay);
    menuWidget->setStyleSheet(
        "background-color: rgba(50, 50, 50, 200); "
        "border-radius: 10px;"
        );
    menuWidget->setFixedSize(300, 350);
    menuWidget->move((width() - menuWidget->width()) / 2, (height() - menuWidget->height()) / 2);

    QVBoxLayout *layout = new QVBoxLayout(menuWidget);

    QLabel *label = new QLabel("Game paused");
    label->setAlignment(Qt::AlignCenter);
    QFont font = label->font();
    font.setBold(true);
    font.setPointSize(14);
    label->setFont(font);
    label->setStyleSheet("color: white;");
    layout->addWidget(label);

    QPushButton *continueButton = new QPushButton("Continue Game");
    QPushButton *saveButton = new QPushButton("Save Game");
    QPushButton *optionsButton = new QPushButton("Options");
    QPushButton *quitButton = new QPushButton("Quit Game");

    QString buttonStyle =
        "QPushButton { "
        "   background-color: gray; "
        "   color: white; "
        "   border-radius: 10px; "
        "   padding: 10px; "
        "} "
        "QPushButton:hover { "
        "   background-color: darkGray; "
        "} ";
    continueButton->setStyleSheet(buttonStyle);
    saveButton->setStyleSheet(buttonStyle);
    optionsButton->setStyleSheet(buttonStyle);
    quitButton->setStyleSheet(buttonStyle);

    layout->addWidget(continueButton);
    layout->addWidget(saveButton);
    layout->addWidget(optionsButton);
    layout->addWidget(quitButton);

    connect(continueButton, &QPushButton::clicked, overlay, &QWidget::deleteLater);
    connect(quitButton, &QPushButton::clicked, this, &QApplication::quit);
    connect(saveButton, &QPushButton::clicked, this, []() {
        QMessageBox::information(nullptr, "Save Game", "Game saved successfully!");
    });
    connect(optionsButton, &QPushButton::clicked, this, []() {
        QMessageBox::information(nullptr, "Options", "Options menu under construction.");
    });

    overlay->show();
}
