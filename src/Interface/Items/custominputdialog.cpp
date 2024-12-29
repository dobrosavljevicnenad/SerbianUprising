#include "custominputdialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>

CustomInputDialog::CustomInputDialog(const QString& title, const QString& labelText, QWidget* parent, int maxValue)
    : QDialog(parent), inputLineEdit(new QLineEdit(this)), maxTroops(maxValue) {
    setWindowTitle(title);
    setModal(true);
    setWindowFlags(Qt::FramelessWindowHint);

    setStyleSheet("background-color: #2D2D2D; color: white;");

    if(maxValue>0)
        setFixedSize(400,100);
    else
        setFixedSize(200, 100);
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* label = new QLabel(labelText, this);
    label->setStyleSheet("font-size: 14px; color: white;");

    inputLineEdit->setStyleSheet("padding: 5px; border: 1px solid #aaa; width: 300px;");

    if (maxTroops > 0) {
        qDebug()<<maxTroops;
        inputLineEdit->setValidator(new QIntValidator(0, maxTroops, this));
    }


    QPushButton* okButton = new QPushButton("OK", this);
    QPushButton* cancelButton = new QPushButton("Cancel", this);

    okButton->setStyleSheet("background-color: #4CAF50; color: white; padding: 5px; border-radius: 3px;");
    cancelButton->setStyleSheet("background-color: #f44336; color: white; padding: 5px; border-radius: 3px;");

    okButton->setDefault(true);
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    layout->addWidget(label);
    layout->addWidget(inputLineEdit);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &CustomInputDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &CustomInputDialog::reject);
    inputLineEdit->setFocus();
    if (!parent) {
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect screenGeometry = screen->availableGeometry();
        int screenWidth = screenGeometry.width();
        int screenHeight = screenGeometry.height();

        int x = (screenWidth - width()) / 2;
        int y = (screenHeight - height()) / 2;

        setGeometry(x, y, width(), height());
    }
    connect(inputLineEdit, &QLineEdit::textChanged, [this]() {
        if(maxTroops > 0){
            bool ok;
            int value = inputLineEdit->text().toInt(&ok);
            if (ok && value > maxTroops) {
                qDebug() << "Entered value exceeds maxTroops!";
                inputLineEdit->setText(QString::number(maxTroops));
            }
        }
    });
}

QString CustomInputDialog::getInputText() const {
    return inputLineEdit->text();
}

int CustomInputDialog::getInputInt() const {
    bool ok;
    int value = inputLineEdit->text().toInt(&ok);
    if (ok) {
        return value;
    } else {
        return 0;
    }
}
CustomInputDialog::~CustomInputDialog() {
    delete inputLineEdit;
}
