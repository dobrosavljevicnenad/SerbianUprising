#include <QJsonDocument>
#include <QFile>

#include "jsonserializer.h"

void JSONSerializer::toJson(const Serializable &serializable, const QString &filePath)
{
    QJsonDocument JSONdoc = QJsonDocument::fromVariant(serializable.toVariant());
    QFile file(filePath);
    if (!file.open(QFile::WriteOnly)) {
        qWarning() << "Could not open file:" << filePath;
        return;
    }
    file.write(JSONdoc.toJson(QJsonDocument::JsonFormat::Indented));
    file.close();
}

void JSONSerializer::fromJson(Serializable &serializable, const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QFile::ReadOnly)) {
        qWarning() << "Could not open file:" << filePath;
        return;
    }
    QJsonDocument JSONdoc = QJsonDocument::fromJson(file.readAll());
    file.close();
    serializable.fromVariant(JSONdoc.toVariant());
}
