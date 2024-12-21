#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QJsonObject>
#include <QFile>
#include <QJsonDocument>
#include <QDebug>

class FileManager {
public:
    static bool saveToFile(const QString& filePath, const QJsonObject& data);
    static QJsonObject loadFromFile(const QString& filePath);
    static bool fileExists(const QString& filePath);
};

#endif // FILEMANAGER_H
