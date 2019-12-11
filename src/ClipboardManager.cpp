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
      {ClipboardDataType::IMAGE, "image.html"},
      {ClipboardDataType::HTML, "html.html"},
      {ClipboardDataType::TEXT, "text.html"},
  };
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
  qInfo() << "Rendered from: " << m_map[mode];

  QString outputFile = m_outputFile + "/index.html";
  QString inputFile = m_inputFile + "/" + m_map[mode];
  KeyValuePairs *subsitutionMap = new KeyValuePairs{{"$data", data}};

  QFile file(outputFile);
  if (file.open(QIODevice::WriteOnly)) {
    QTextStream fout(&file);
    fout << substituteTemplate(inputFile, subsitutionMap);
  }
  file.close();
}

const QString ClipboardManager::substituteTemplate(
    const QString &path, const KeyValuePairs *subsitution) const {
  QFile file(path);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return "";

  QString templateString(file.readAll());

  for (const auto &item : *subsitution) {
    templateString.replace(item.first, item.second);
  }
  file.close();
  return templateString;
}
