#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./Interface/gamemenu.h"

GameMenu *gameMenu;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(new MapScene(this))
    , selectedLayer(nullptr)
{
    ui->setupUi(this);

    gameMenu = new GameMenu(this);
    setCentralWidget(gameMenu);

    //MapScene *scene = new MapScene(this);

    view = new QGraphicsView(scene, this);
    //setCentralWidget(view);

    gameManager = new GameManager(scene);

    connect(gameMenu, &GameMenu::startGame, this, [this]() {
        if(!scene->items().isEmpty()) {
            scene->clear();
        }
        gameManager->initializeMap();
        setCentralWidget(view);
    });

    connect(gameMenu, &GameMenu::exitGame, this, &QMainWindow::close);

    connect(gameMenu, &GameMenu::fullScreenClicked, this, [this] () {
        if(isFullScreen()) {
            showNormal();
        } else {
            showFullScreen();
        }
    });


    QPushButton *changePlayerButton = new QPushButton("Change Player");
    QPushButton *endTurnButton = new QPushButton("End Turn");
    QTextBrowser *textField = new QTextBrowser();

    QGraphicsProxyWidget *changePlayerButtonProxy = scene->addWidget(changePlayerButton);
    QGraphicsProxyWidget *endTurnButtonProxy = scene->addWidget(endTurnButton);
    QGraphicsProxyWidget *textFieldProxy = scene->addWidget(textField);

    changePlayerButtonProxy->setZValue(1);
    endTurnButtonProxy->setZValue(1);
    textFieldProxy->setZValue(1);

    changePlayerButtonProxy->setPos(850, 10);
    endTurnButtonProxy->setPos(1000, 10);
    textFieldProxy->setPos(840, 50);


    // and also need to implement on moveArmy to put on buffer also buffer need

    connect(gameManager, &GameManager::layerClicked, this, &MainWindow::onLayerClicked);
    connect(ui->changePlayerButton, &QPushButton::clicked, this, &MainWindow::onChangePlayerClicked);
    connect(ui->endTurnButton, &QPushButton::clicked, this, &MainWindow::onEndTurnClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onChangePlayerClicked() {
    turnManager.changePlayer();
    int currentPlayer = turnManager.getCurrentPlayer();
    ui->textField->append(QString("Player %1 is now active.").arg(currentPlayer));
}

void MainWindow::onEndTurnClicked() {
    ui->textField->append(turnManager.processBuffer());
    turnManager.endTurn();
    int currentPlayer = turnManager.getCurrentPlayer();
    ui->textField->append(QString("Player %1's turn starts.").arg(currentPlayer));
}

void MainWindow::onLayerClicked(MapLayer *layer) {
    if (selectedLayer == nullptr) {
        selectedLayer = layer;
    } else {
        if(selectedLayer == layer) {
            QMessageBox::warning(this, tr("Error"), tr("You selected the same layer. Select another layer."));
            selectedLayer = nullptr;
            return ;
        }

        graph::Vertex* selected_vertex = gameManager->layerToVertex[selectedLayer];
        graph::Vertex* vertex = gameManager->layerToVertex[layer];

        int currentPlayer = turnManager.getCurrentPlayer();
        if(currentPlayer == 1){
            if(selected_vertex->army.armyType() == ArmyType::JANISSARY){
                QMessageBox::warning(this, tr("Error"), tr("You have to select your territory first."));
                selectedLayer = nullptr;
                return;
            }
        } else {
            if(selected_vertex->army.armyType() == ArmyType::HAJDUK){
                QMessageBox::warning(this, tr("Error"), tr("You have to select your territory first."));
                selectedLayer = nullptr;
                return;
            }
        }

        bool ok;
        int maxTroops = selected_vertex->army.getSoldiers();
        int troopsToTransfer = QInputDialog::getInt(this, tr("Transfer Troops"), tr("Enter the number of soldiers to transfer:"), 0, 0, maxTroops, 1, &ok);
        if (ok) {
            if(troopsToTransfer > selected_vertex->army.getSoldiers()) {
                QMessageBox::warning(this, tr("Error"), tr("You don`t have enough troops to transfer."));
            } else {
                gameManager->ma.executeMove(selected_vertex,vertex,troopsToTransfer);

                selectedLayer->setTroopCount(selected_vertex->army.getSoldiers());
                layer->setTroopCount(vertex->army.getSoldiers());
                layer->setArmyColor(vertex->army.armyType());

                std::cout << selected_vertex->army.getSoldiers() << " " << vertex->army.getSoldiers() << std::endl;
            }
        }

        selectedLayer = nullptr;
    }
}
