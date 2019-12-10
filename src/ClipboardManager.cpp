#include "ClipboardManager.h"
#include <QDebug>
#include <QClipboard>
#include <QMimeData>
#include <QImage>
#include <QBuffer>
#include <QFile>

ClipboardManager::ClipboardManager(const QClipboard* cb, const QCommandLineParser& parser)
{
    m_clipboard = cb;
    m_inputFile =  parser.value("input");
    m_outputFile = parser.value("output");

    m_map = QMap<ClipboardDataType, QString>
    {
        {ClipboardDataType::IMAGE, "image.html"},
        {ClipboardDataType::HTML, "html.html"},
        {ClipboardDataType::TEXT, "text.html"},
    };

}


void ClipboardManager::onClipboardDataChange() const
{
    const QMimeData* mimeData = m_clipboard->mimeData(QClipboard::Clipboard);
    if (mimeData != nullptr)
    {
        if (mimeData->hasImage())
        {
            renderOutput(ClipboardDataType::IMAGE, encodeBase64Image(mimeData->imageData()));
        }
        else if (mimeData->hasHtml())
        {
            renderOutput(ClipboardDataType::HTML, mimeData->html());
        }
        else if (mimeData->hasText())
        {
            renderOutput(ClipboardDataType::TEXT, mimeData->text());

        }
        else
        {
            qInfo() << "not allowed to share!";
        }
    }
}


const QString ClipboardManager::encodeBase64Image(const QVariant image) const
{
    QImage img = qvariant_cast<QImage>(image);
    img = img.convertToFormat(QImage::Format_RGB32);
    QByteArray ba;
    QBuffer bu(&ba);
    img.save(&bu, "PNG");

    return ba.toBase64();
}



void ClipboardManager::renderOutput(const ClipboardDataType mode, const QString data) const
{
    QString outputFile = m_outputFile + "/index.html";
    QString inputFile = m_inputFile + "/" + m_map[mode];
    KeyValuePairs* subsitutionMap = new KeyValuePairs
    {
        {"$data", data}
    };


    QFile file(outputFile);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream fout(&file);
        fout << substituteTemplate(inputFile, *subsitutionMap);
    }
    file.close();
}


const QString ClipboardManager::substituteTemplate(const QString path, const KeyValuePairs& subsitution) const
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "";

    QString templateString(file.readAll());

    foreach (auto item, subsitution)
    {
        templateString.replace(item.first, item.second);
    }
    file.close();
    return templateString;
}

