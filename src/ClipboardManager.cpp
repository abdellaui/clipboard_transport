#include "ClipboardManager.h"
#include <QBuffer>
#include <QClipboard>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QImage>

ClipboardManager::ClipboardManager(const QClipboard *cb,
                                   const QCommandLineParser &parser) {
  m_clipboard = cb;
  m_inputFile = morphFile(parser.value("input"));
  m_outputFile = morphFile(parser.value("output"));

  QDir dir(m_outputFile + "/img/");
  if (!dir.exists()) {
    dir.mkpath(".");
  }

  m_map = QMap<ClipboardDataType, QString>{
      {ClipboardDataType::IMAGE, readFileAll(m_inputFile + "/image.html")},
      {ClipboardDataType::HTML, readFileAll(m_inputFile + "/html.html")},
      {ClipboardDataType::TEXT, readFileAll(m_inputFile + "/text.html")},
  };
}

QString ClipboardManager::writeIntoFile(const QString &path,
                                        const QString &data) const {
  QFile file(path);
  if (file.open(QIODevice::WriteOnly)) {
    QTextStream fout(&file);
    fout << data;
  } else {
    qInfo() << "couldnt save file: " << path;
  }
  file.close();
}

QString ClipboardManager::readFileAll(const QString &path) const {
  QFile file(path);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return "";
  return QString(file.readAll());
}

QString ClipboardManager::morphFile(QString path) const {
  if (path.startsWith("~/")) path.replace(0, 1, QDir::homePath());
  return path;
}

void ClipboardManager::onClipboardDataChange() const {
  const QMimeData *mimeData = m_clipboard->mimeData(QClipboard::Clipboard);
  if (mimeData != nullptr) {
    if (mimeData->hasImage()) {
      renderOutput(ClipboardDataType::IMAGE, storeImage(mimeData));
    } else if (mimeData->hasHtml()) {
      renderOutput(ClipboardDataType::HTML, mimeData->html());
    } else if (mimeData->hasText()) {
      renderOutput(ClipboardDataType::TEXT, mimeData->text());

    } else {
      qInfo() << "not allowed to share!";
    }
  }
}

const QString ClipboardManager::storeImage(const QMimeData *mimeData) const {
  QString searchWord = "image/";
  QString fileName = QString::number(QDateTime::currentSecsSinceEpoch());
  QString fileExtention = "png";

  for (QString &format : mimeData->formats()) {
    if (format.startsWith(searchWord)) {
      fileExtention = format.replace(searchWord, "");
      break;
    }
  }

  fileName = QString("%1.%2").arg(fileName).arg(fileExtention);

  QString filePath = QString("%1/img/%2").arg(m_outputFile).arg(fileName);
  QImage img = qvariant_cast<QImage>(mimeData->imageData());
  img.save(filePath, fileExtention.toStdString().c_str());

  return fileName;
}

void ClipboardManager::renderOutput(const ClipboardDataType &mode,
                                    const QString &data) const {
  QString timestamp = QString::number(QDateTime::currentSecsSinceEpoch());

  KeyValuePairs *subsitutionMap =
      new KeyValuePairs{{"$data", data}, {"$time", timestamp}};

  writeIntoFile(m_outputFile + "/index.html",
                substituteTemplate(mode, subsitutionMap));
  writeIntoFile(m_outputFile + "time.txt", timestamp);
}

const QString ClipboardManager::substituteTemplate(
    const ClipboardDataType &mode, const KeyValuePairs *subsitution) const {
  QString templateString = m_map[mode];

  for (const auto &item : *subsitution) {
    templateString.replace(item.first, item.second);
  }

  return templateString;
}
