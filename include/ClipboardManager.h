#pragma once
#include <QClipboard>
#include <QCommandLineParser>
#include <QMap>
#include <QMimeData>
#include <QObject>

typedef QList<QPair<QString, QString>> KeyValuePairs;

enum ClipboardDataType { IMAGE, HTML, TEXT };

class ClipboardManager : public QObject {
  Q_OBJECT

 public:
  ClipboardManager(const QClipboard *, const QCommandLineParser &);

 public Q_SLOTS:
  void onClipboardDataChange() const;

 private:
  const QClipboard *m_clipboard;
  QString m_inputFile;
  QString m_outputFile;
  QMap<ClipboardDataType, QString> m_map;

  QString morphFile(QString) const;
  void writeIntoFile(const QString &, const QString &) const;
  QString readFileAll(const QString &) const;
  const QString storeImage(const QStringList, const QVariant) const;
  void renderOutput(const ClipboardDataType &, const QString &) const;
  const QString substituteTemplate(const ClipboardDataType &,
                                   const KeyValuePairs &) const;
};
