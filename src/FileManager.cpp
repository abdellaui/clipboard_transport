#include "FileManager.h"

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QImage>

QString FileManager::storeImage(const QString& outputFilePrefix,
                                const QStringList formats,
                                const QVariant imageData)
{
    QString fileName = QString::number(QDateTime::currentSecsSinceEpoch());
    QString fileExtention = "png";
    QString searchWord = "image/";

    for (QString format : formats)
    {
        if (format.startsWith(searchWord))
        {
            fileExtention = format.replace(searchWord, "");
            break;
        }
    }

    fileName = QString("%1.%2").arg(fileName).arg(fileExtention);
    QString fileAbsPath = QString("%1/img/%2").arg(outputFilePrefix).arg(fileName);

    QImage img = qvariant_cast<QImage>(imageData);
    img.save(fileAbsPath, fileExtention.toStdString().c_str());

    return fileName;
}

bool FileManager::writeIntoFile(const QString& path, const QString& data)
{
    bool returnVal = false;
    QFile file(path);

    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream fout(&file);
        fout << data;
        returnVal = true;
    }
    else
    {
        qInfo() << "couldnt save file: " << path;
    }

    file.close();
    return returnVal;
}

QString FileManager::readFileAll(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return "";
    }
    return QString(file.readAll());
}

QString FileManager::morphFile(QString path)
{
    if (path.startsWith("~/"))
    {
        path.replace(0, 1, QDir::homePath());
    }
    return path;
}

bool FileManager::createPath(const QString& path)
{
    QDir dir(path);
    if (!dir.exists())
    {
        dir.mkpath(".");
        return true;
    }
    return false;
}
