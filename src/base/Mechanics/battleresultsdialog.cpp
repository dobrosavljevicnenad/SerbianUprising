#include "battleresultsdialog.h"
#include <qapplication.h>

BattleResultsDialog::BattleResultsDialog(const std::vector<Results> battleResults, QWidget *parent)
    : QDialog(parent), battleResults(battleResults) {

    gridLayout = new QGridLayout(this);
    setLayout(gridLayout);

    tablesContainer = new QWidget(this);
    tablesContainer->setGeometry(265, 255, 606, 262);
    tablesContainer->setStyleSheet("background: transparent;");

    closeButton = new QPushButton("Close", this);
    closeButton->setStyleSheet(getButtonStyle());
    connect(closeButton, &QPushButton::clicked, this, &QDialog::reject);
    closeButton->setFixedSize(100, 40);

    containerLayout = new QGridLayout(tablesContainer);
    tablesContainer->setLayout(containerLayout);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowTitle("Battle Results");
    setObjectName("BattleResultsDialog");
    setBackgroundImage(":/resources/Images/Board.png");
    resize(1084, 905);
    closeButton->move(10, 10);

    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect availableGeometry = screen->availableGeometry();
        resize(availableGeometry.size());
        showMaximized();
        int ax = availableGeometry.width()*265/1084;
        int ay = availableGeometry.height()*255/905;
        int aw = availableGeometry.width()*606/1084;
        int ah = availableGeometry.height()*262/905;
        qDebug() << ax<<ay<<aw<<ah;
        tablesContainer->setGeometry(ax,ay,aw,ah);
        closeButton->move(10, 10);
    }
    setFixedSize(size());
}
QString BattleResultsDialog::getButtonStyle(){
    return QString(
        "QPushButton {"
        "    background-color: #FFD700;"
        "    color: black;"
        "    border: 2px solid #FFD700;"
        "    border-radius: 10px;"
        "    padding: 10px 20px;"
        "    font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #FFCC00;"
        "    border-color: #FFCC00;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #E5B800;"
        "    border-color: #E5B800;"
        "}");
}

void BattleResultsDialog::setBackgroundImage(const QString &imagePath) {
    QString styleSheet = QString("QDialog#BattleResultsDialog { "
                                 "border-image: url(%1);}").arg(imagePath);
    setStyleSheet(styleSheet);
}

void BattleResultsDialog::setResults(const QVector<QStringList> &results) {
    QLayoutItem *child;
    while ((child = this->layout()->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    int row = 0;
    int col = 0;
    const int maxColumns = 4;

    for (int i = 0; i < results.size(); ++i) {
        QTableWidget *table = new QTableWidget(tablesContainer);
        table->setColumnCount(2);
        table->setRowCount(3);

        connect(table, &QTableWidget::cellClicked, this, [this, i](int row, int col) {
            onTableCellClicked(i, battleResults[i]);
        });

        QString tableStyle = "QTableWidget { "
                             "background-color: transparent; "
                             "font-size: 12px; "
                             "font-weight: bold; "
                             "color: white; "
                             "margin: 0px; "
                             "padding: 0px; "
                             "border: 2px solid black; }";
        table->setStyleSheet(tableStyle);

        QString winnerType = results[i][5];
        QColor backgroundColor;

        if (winnerType == "Hajduk") {
            backgroundColor = QColor(241, 84, 102, 255);
        } else if (winnerType == "Janissary") {
            backgroundColor = QColor(109, 210, 72, 255);
        } else {
            backgroundColor = QColor(255, 255, 255);
        }

        table->setStyleSheet(QString("QTableWidget { "
                                     "background-color: rgba(%1, %2, %3, %4); "
                                     "border: 2px solid black; }")
                                 .arg(backgroundColor.red())
                                 .arg(backgroundColor.green())
                                 .arg(backgroundColor.blue())
                                 .arg(1));

        table->setHorizontalHeaderLabels({"", ""});
        table->setVerticalHeaderLabels({"", "", ""});
        table->horizontalHeader()->setVisible(false);
        table->verticalHeader()->setVisible(false);
        table->setShowGrid(false);

        table->setSpan(0, 0, 1, 2);

        QTableWidgetItem *vertexIDItem = new QTableWidgetItem(results[i][0]);
        vertexIDItem->setTextAlignment(Qt::AlignCenter);
        vertexIDItem->setFont(QFont("Arial", 11, QFont::Bold));
        vertexIDItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        table->setItem(0, 0, vertexIDItem);

        QTableWidgetItem *defenderTypeItem = new QTableWidgetItem(results[i][3]);
        defenderTypeItem->setTextAlignment(Qt::AlignCenter);
        defenderTypeItem->setFont(QFont("Arial", 11, QFont::Bold));
        defenderTypeItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        table->setItem(1, 0, defenderTypeItem);

        QTableWidgetItem *attackerTypeItem = new QTableWidgetItem(results[i][4]);
        attackerTypeItem->setTextAlignment(Qt::AlignCenter);
        attackerTypeItem->setFont(QFont("Arial", 11, QFont::Bold));
        attackerTypeItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        table->setItem(1, 1, attackerTypeItem);

        QTableWidgetItem *defenderSoldiersItem = new QTableWidgetItem(results[i][1]);
        defenderSoldiersItem->setTextAlignment(Qt::AlignCenter);
        defenderSoldiersItem->setFont(QFont("Arial", 11, QFont::Bold));
        defenderSoldiersItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        table->setItem(2, 0, defenderSoldiersItem);

        QTableWidgetItem *attackerSoldiersItem = new QTableWidgetItem(results[i][2]);
        attackerSoldiersItem->setTextAlignment(Qt::AlignCenter);
        attackerSoldiersItem->setFont(QFont("Arial", 11, QFont::Bold));
        attackerSoldiersItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        table->setItem(2, 1, attackerSoldiersItem);

        table->resizeColumnsToContents();
        table->resizeRowsToContents();
        int totalWidth = 5;
        for (int colIdx = 0; colIdx < table->columnCount(); ++colIdx) {
            totalWidth += table->columnWidth(colIdx);
        }

        table->setFixedWidth(totalWidth);

        int totalHeight = 5;
        for (int rowIdx = 0; rowIdx < table->rowCount(); ++rowIdx) {
            totalHeight += table->rowHeight(rowIdx);
        }

        table->setFixedHeight(totalHeight);

        table->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        containerLayout->addWidget(table, row, col);

        col++;

        if (col >= maxColumns) {
            col = 0;
            row++;
        }
    }
}

void BattleResultsDialog::onTableCellClicked(int tableIndex, Results results) {
    closeButton->hide();
    BattleReplayDialog *detailsDialog = new BattleReplayDialog(this, tableIndex, results);
    detailsDialog->exec();
    closeButton->show();

    QLayoutItem *child;
    int removedIndex = 0;
    bool allTablesHidden = true;

    for (int i = 0; i < containerLayout->count(); ++i) {
        child = containerLayout->itemAt(i);

        QTableWidget *tableWidget = qobject_cast<QTableWidget *>(child->widget());

        if (tableWidget) {
            if (tableIndex == removedIndex) {
                tableWidget->setVisible(false);
            }

            if (tableWidget->isVisible()) {
                allTablesHidden = false;
            }
        }
        removedIndex++;
    }

    if (allTablesHidden) {
        accept();
    }
}
