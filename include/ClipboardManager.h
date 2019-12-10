#pragma once
#include <QObject>
#include <QClipboard>
#include <QCommandLineParser>
#include <QMap>

typedef QList<QPair<QString, QString>> KeyValuePairs;

enum ClipboardDataType
{
    IMAGE,
    HTML,
    TEXT
};

class ClipboardManager : public QObject
{
    Q_OBJECT

public:
    ClipboardManager(const QClipboard*, const QCommandLineParser&);

public Q_SLOTS:
    void onClipboardDataChange() const;

private:
    const QClipboard* m_clipboard;
    QString m_inputFile;
    QString m_outputFile;
    QMap<ClipboardDataType, QString> m_map;

    const QString encodeBase64Image(const QVariant) const;
    void renderOutput(const ClipboardDataType, const QString) const;
    const QString substituteTemplate(const QString, const KeyValuePairs&) const;
};
