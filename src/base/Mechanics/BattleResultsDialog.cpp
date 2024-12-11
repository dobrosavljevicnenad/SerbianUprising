#include "BattleResultsDialog.h"
#include <QHeaderView>
#include <QPushButton>

BattleResultsDialog::BattleResultsDialog(QWidget *parent)
    : QDialog(parent), titleLabel(new QLabel("Results", this)), resultsTable(new QTableWidget(this)) {

    // Setup layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(titleLabel);
    layout->addWidget(resultsTable);

    // Set dialog title and layout
    setWindowTitle("Battle Results");

    // Configure titleLabel to be centered
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold;");

    // Configure the table widget
    resultsTable->setColumnCount(4); // Four columns: Vertex ID, Defender Soldiers, Attacker Soldiers, Army Types
    resultsTable->setHorizontalHeaderLabels({"Vertex ID", "Defender Soldiers", "Attacker Soldiers", "Army Types"});
    resultsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Stretch columns to fit
    resultsTable->verticalHeader()->setVisible(false); // Hide the vertical header
}

void BattleResultsDialog::setResults(const QVector<QStringList> &results) {
    // Set the row count based on the number of results
    resultsTable->setRowCount(results.size());

    // Fill in the table with results data
    for (int i = 0; i < results.size(); ++i) {
        for (int j = 0; j < results[i].size(); ++j) {
            resultsTable->setItem(i, j, new QTableWidgetItem(results[i][j]));
        }
    }
}
