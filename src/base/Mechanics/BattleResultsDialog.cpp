#include "BattleResultsDialog.h"
#include <iostream>

BattleResultsDialog::BattleResultsDialog(const std::vector<Results> battleResults, QWidget *parent)
    : QDialog(parent), battleResults(battleResults){
    // Setup grid layout to hold multiple tables in rows and columns
    gridLayout = new QGridLayout(this);  // Use member gridLayout
    setLayout(gridLayout);

    // Set dialog title
    setWindowTitle("Battle Results");
    setObjectName("BattleResultsDialog");    // Set the background image for the dialog window
    setBackgroundImage(":/resources/pozadina.png");
    // Path to your background image
    resize(600, 400);
}

void BattleResultsDialog::setBackgroundImage(const QString &imagePath) {
    QString styleSheet = QString("QDialog#BattleResultsDialog { "
                                 "background-image: url(%1); "
                                 "background-repeat: no-repeat; "
                                 "background-position: center; }").arg(imagePath);
    setStyleSheet(styleSheet);  // Apply the background image only to BattleResultsDialog
}
void BattleResultsDialog::setResults(const QVector<QStringList> &results) {
    // Clear any existing widgets
    QLayoutItem *child;
    while ((child = this->layout()->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    // Create a new table for each result set
    int row = 0; // Starting row
    int col = 0; // Starting column
    const int maxColumns = 5; // Max number of tables per row (5 columns)

    for (int i = 0; i < results.size(); ++i) {
        // Create a new table widget for each result
        QTableWidget *table = new QTableWidget(this);
        table->setColumnCount(2); // Two columns: Vertex ID and Defender/Attacker info
        table->setRowCount(3);    // Three rows for the specified format
        connect(table, &QTableWidget::cellClicked, this, [this, i](int row, int col) {
            onTableCellClicked(i, battleResults[i]); // Pass index to identify which table was clicked
        });
        // Set a specific style for the table to avoid inheriting from the dialog
        QString tableStyle = "QTableWidget { "
                             "background-color: transparent; "
                             "font-size: 14px; "
                             "font-weight: bold; "
                             "color: white; "
                             "margin: 0px; "
                             "padding: 0px; "
                             "border: 2px solid black; }"; // Add black border around the table
        table->setStyleSheet(tableStyle);

        // Set transparency and background color based on winner type
        QString winnerType = results[i][5]; // Get the winner type from the results
        QColor backgroundColor;

        if (winnerType == "Hajduk") {
            backgroundColor = QColor(198,54,60,255);  // Red for Hajduk
        } else if (winnerType == "Janissary") {
            backgroundColor = QColor(0,149,48,255);  // Green for Janissary
        } else {
            backgroundColor = QColor(255, 255, 255);  // Default to white if not Hajduk or Janissary
        }
        // Set the background color with transparency for the table
        table->setStyleSheet(QString("QTableWidget { "
                                     "background-color: rgba(%1, %2, %3, %4); "
                                     "border: 2px solid black; }")  // Ensure border is applied
                                 .arg(backgroundColor.red())
                                 .arg(backgroundColor.green())
                                 .arg(backgroundColor.blue())
                                 .arg(1));
        // Remove headers and gridlines
        table->setHorizontalHeaderLabels({"", ""}); // Empty headers
        table->setVerticalHeaderLabels({"", "", ""}); // Empty vertical headers
        table->horizontalHeader()->setVisible(false);   // Hide horizontal header
        table->verticalHeader()->setVisible(false);     // Hide vertical header
        table->setShowGrid(false);                       // Hide gridlines

        // Merge the first row to span two columns for Vertex ID
        table->setSpan(0, 0, 1, 2); // Vertex ID spans across two columns (row 0, column 0 and 1)

        // Fill in the first row with Vertex ID in both columns
        QTableWidgetItem *vertexIDItem = new QTableWidgetItem(results[i][0]);
        vertexIDItem->setTextAlignment(Qt::AlignCenter);  // Center the text
        vertexIDItem->setFont(QFont("Arial", 12, QFont::Bold));  // Bold font
        vertexIDItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled); // Make item non-editable
        table->setItem(0, 0, vertexIDItem);

        // Fill in the second row (Defender Type and Attacker Type)
        QTableWidgetItem *defenderTypeItem = new QTableWidgetItem(results[i][3]);
        defenderTypeItem->setTextAlignment(Qt::AlignCenter);  // Center the text
        defenderTypeItem->setFont(QFont("Arial", 12, QFont::Bold));  // Bold font
        defenderTypeItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled); // Make item non-editable
        table->setItem(1, 0, defenderTypeItem);

        QTableWidgetItem *attackerTypeItem = new QTableWidgetItem(results[i][4]);
        attackerTypeItem->setTextAlignment(Qt::AlignCenter);  // Center the text
        attackerTypeItem->setFont(QFont("Arial", 12, QFont::Bold));  // Bold font
        attackerTypeItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled); // Make item non-editable
        table->setItem(1, 1, attackerTypeItem);

        // Fill in the third row (Defender Soldiers and Attacker Soldiers)
        QTableWidgetItem *defenderSoldiersItem = new QTableWidgetItem(results[i][1]);
        defenderSoldiersItem->setTextAlignment(Qt::AlignCenter);  // Center the text
        defenderSoldiersItem->setFont(QFont("Arial", 12, QFont::Bold));  // Bold font
        defenderSoldiersItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled); // Make item non-editable
        table->setItem(2, 0, defenderSoldiersItem);

        QTableWidgetItem *attackerSoldiersItem = new QTableWidgetItem(results[i][2]);
        attackerSoldiersItem->setTextAlignment(Qt::AlignCenter);  // Center the text
        attackerSoldiersItem->setFont(QFont("Arial", 12, QFont::Bold));  // Bold font
        attackerSoldiersItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled); // Make item non-editable
        table->setItem(2, 1, attackerSoldiersItem);

        // Resize columns and rows to fit their contents
        table->resizeColumnsToContents();  // Resize columns based on the content
        table->resizeRowsToContents();     // Resize rows based on the content

        // Adjust the table size to match the total width of columns
        int totalWidth = 5;
        for (int colIdx = 0; colIdx < table->columnCount(); ++colIdx) {
            totalWidth += table->columnWidth(colIdx);  // Sum up the column widths
        }

        // Set the table width to the total width of the columns
        table->setFixedWidth(totalWidth);  // Set the table's width based on the total column width

        // Adjust the table height to match the total height of rows
        int totalHeight = 5;
        for (int rowIdx = 0; rowIdx < table->rowCount(); ++rowIdx) {
            totalHeight += table->rowHeight(rowIdx);  // Sum up the row heights
        }

        // Set the table height to the total height of the rows
        table->setFixedHeight(totalHeight);  // Set the table's height based on the total row height

        // Set size policy to ensure the table shrinks to fit its content
        table->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);  // Prevent expansion

        // Disable scrollbars explicitly
        table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Disable horizontal scrollbar
        table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // Disable vertical scrollbar

        // Add table to the grid layout (position it in the grid)
        gridLayout->addWidget(table, row, col);

        // Move to the next column
        col++;

        // If we reached the maximum columns per row, move to the next row
        if (col >= maxColumns) {
            col = 0;  // Reset column to 0
            row++;    // Move to the next row
        }
    }
}

void BattleResultsDialog::onTableCellClicked(int tableIndex, Results results) {
    // Pokažite dijalog sa detaljima
    BattleReplayDialog *detailsDialog = new BattleReplayDialog(this, tableIndex, results);
    detailsDialog->exec(); // Prikazivanje dijaloga sa detaljima

    // Nakon što je dijalog zatvoren, sakrijte odgovarajući `QTableWidget` iz layout-a
    QLayoutItem *child;
    int removedIndex = 0; // Pokazaćemo koji je indeks obrisan
    bool allTablesHidden = true;  // Flag koji prati da li su sve tabele nevidljive

    // Iterirajte kroz sve widgete u layout-u i pronađite odgovarajući `QTableWidget` koji treba sakriti
    for (int i = 0; i < gridLayout->count(); ++i) {
        child = gridLayout->itemAt(i); // Dobijamo widget na određenom indeksu

        QTableWidget *tableWidget = qobject_cast<QTableWidget *>(child->widget()); // Pokušajte da kastujete widget u QTableWidget

        if (tableWidget) {
            if (tableIndex == removedIndex) {
                // Ako je našao odgovarajući `QTableWidget`, sakrijte ga
                tableWidget->setVisible(false); // Sakrivanje tabele
            }

            // Proveravamo da li je tabela još uvek vidljiva
            if (tableWidget->isVisible()) {
                allTablesHidden = false; // Ako je bilo koja tabela vidljiva, postaviće se flag na false
            }
        }
        removedIndex++;
    }

    // Ako su sve tabele postale nevidljive, zatvori dijalog
    if (allTablesHidden) {
        accept(); // Zatvori dijalog
    }
}


