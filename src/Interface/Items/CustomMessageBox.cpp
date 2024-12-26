#include "CustomMessageBox.h"

CustomMessageBox::CustomMessageBox(const QString &message, QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("");
    setFixedSize(400, 200);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setStyleSheet("background-color: #2D2D2D; color: white; border-radius: 10px;");

    messageLabel = new QLabel(message, this);
    messageLabel->setStyleSheet("font-size: 16px; padding: 20px;");
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setWordWrap(true);

    okButton = new QPushButton("Okay", this);
    okButton->setStyleSheet(
        "background-color: #117733; color: white; font-size: 14px; padding: 10px 20px; border-radius: 5px;"
        );
    connect(okButton, &QPushButton::clicked, this, &CustomMessageBox::accept);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(messageLabel);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);
}

void CustomMessageBox::showMessage(const QString &message, QWidget *parent) {
    CustomMessageBox box(message, parent);
    box.exec();
}
