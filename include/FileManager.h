#pragma once
#include "typedefs.h"

#include<QString>
#include<QStringList>
#include<QVariant>

class FileManager
{
public:
    static QString storeImage(const QString&, const QStringList, const QVariant);
    static bool writeIntoFile(const QString&, const QString&);
    static QString readFileAll(const QString&);
    static QString morphFile(QString);
    static bool createPath(const QString&);
};
