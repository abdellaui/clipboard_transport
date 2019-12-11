#pragma once
#include "typedefs.h"

#include <QClipboard>
#include <QCommandLineParser>
#include <QMimeData>
#include <QString>
#include <QObject>

enum ClipboardDataType { IMAGE, HTML, TEXT };

class ClipboardManager : public QObject
{
    Q_OBJECT

public:
    ClipboardManager(const QClipboard*, const QCommandLineParser&);

public Q_SLOTS:
    void onClipboardDataChange() const;

private:
    const QClipboard* m_clipboard;
    QString m_outputFile;
    QMap<ClipboardDataType, QString> m_templateMap;

    void renderOutput(const ClipboardDataType&, const QString&) const;
};
