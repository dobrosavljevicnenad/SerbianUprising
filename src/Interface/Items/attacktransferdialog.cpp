#include "attacktransferdialog.h"

AttackTransferDialog::AttackTransferDialog(const QString &source, const QString &target,
                                           int sourceArmies, int targetArmies,
                                           const QPixmap &sourceImage, const QPixmap &targetImage,
                                           QWidget *parent)
    : QDialog(parent) {
    setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet("background-color: #2C2C2C; color: white;");

    QLabel *titleLabel = new QLabel("Attack or Transfer");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: white;");

    sourceLabel = new QLabel(QString("%1 (%2 armies)").arg(source).arg(sourceArmies));
    targetLabel = new QLabel(QString("%1 (%2 armies)").arg(target).arg(targetArmies));
    sourceLabel->setAlignment(Qt::AlignCenter);
    targetLabel->setAlignment(Qt::AlignCenter);

    sourceImageLabel = new QLabel();
    sourceImageLabel->setPixmap(sourceImage.scaled(100, 100, Qt::IgnoreAspectRatio));
    sourceImageLabel->setAlignment(Qt::AlignCenter);

    targetImageLabel = new QLabel();
    targetImageLabel->setPixmap(targetImage.scaled(100, 100, Qt::IgnoreAspectRatio));
    targetImageLabel->setAlignment(Qt::AlignCenter);

    slider = new QSlider(Qt::Horizontal);
    slider->setRange(1, sourceArmies);
    slider->setValue(1);

    spinBox = new QSpinBox();
    spinBox->setRange(1, sourceArmies);
    spinBox->setValue(1);
    spinBox->setStyleSheet("background-color: #444; color: white; border: 1px solid #32CD32;");

    connect(slider, &QSlider::valueChanged, spinBox, &QSpinBox::setValue);
    connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), slider, &QSlider::setValue);

    okButton = new QPushButton("Okay");
    cancelButton = new QPushButton("Cancel");

    okButton->setStyleSheet("background-color: #4CAF50; color: white; padding: 5px; border-radius: 3px;");
    cancelButton->setStyleSheet("background-color: #f44336; color: white; padding: 5px; border-radius: 3px;");

    okButton->setDefault(true);

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    QVBoxLayout *layout = new QVBoxLayout();

    layout->addWidget(titleLabel);

    QHBoxLayout *territoryLayout = new QHBoxLayout();

    QVBoxLayout *sourceLayout = new QVBoxLayout();
    QVBoxLayout *targetLayout = new QVBoxLayout();

    sourceLayout->addWidget(sourceImageLabel);
    sourceLayout->addWidget(sourceLabel);
    targetLayout->addWidget(targetImageLabel);
    targetLayout->addWidget(targetLabel);

    QLabel *arrowLabel = new QLabel();
    QPixmap arrowPixmap(100, 100);
    arrowPixmap.fill(Qt::transparent);

    QPainter painter(&arrowPixmap);
    painter.setPen(QPen(Qt::red, 8));
    painter.setBrush(Qt::red);

    QPolygon arrow;
    arrow << QPoint(10, 40) << QPoint(10, 60) << QPoint(60, 60) << QPoint(60, 80) << QPoint(90, 50) << QPoint(60,20) << QPoint(60,40);
    painter.drawPolygon(arrow);

    arrowLabel->setPixmap(arrowPixmap);

    territoryLayout->addLayout(sourceLayout);
    territoryLayout->addWidget(arrowLabel);
    territoryLayout->addLayout(targetLayout);

    layout->addLayout(territoryLayout);

    QHBoxLayout *sliderLayout = new QHBoxLayout();
    sliderLayout->addWidget(new QLabel("Armies:"));
    sliderLayout->addWidget(slider);
    sliderLayout->addWidget(spinBox);

    layout->addLayout(sliderLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(okButton);

    layout->addLayout(buttonLayout);

    setLayout(layout);

    spinBox->setFocus();

    if (!parent) {
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect screenGeometry = screen->availableGeometry();
        int screenWidth = screenGeometry.width();
        int screenHeight = screenGeometry.height();

        int x = (screenWidth - width()) / 2;
        int y = (screenHeight - height()) / 2;
        move(x, y);
    }

}

int AttackTransferDialog::getSelectedArmies() const {
    return slider->value();
}

AttackTransferDialog::~AttackTransferDialog() {
    delete sourceLabel;
    delete targetLabel;
    delete sourceImageLabel;
    delete targetImageLabel;
    delete slider;
    delete spinBox;
    delete okButton;
    delete cancelButton;
}
