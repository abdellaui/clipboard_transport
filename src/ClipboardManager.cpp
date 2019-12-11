#include "ClipboardManager.h"

#include <QClipboard>
#include <QDateTime>
#include <QDebug>

#include "FileManager.h"
ClipboardManager::ClipboardManager(const QClipboard* cb,
                                   const QCommandLineParser& parser)
{
    m_clipboard = cb;

    m_outputFile = FileManager::morphFile(parser.value("output"));
    FileManager::createPath(m_outputFile + "/img/");

    QString inputFile = FileManager::morphFile(parser.value("input"));
    m_templateMap = QMap<ClipboardDataType, QString>
    {
        {
            ClipboardDataType::IMAGE,
            FileManager::readFileAll(inputFile + "/image.html")
        },
        {
            ClipboardDataType::HTML,
            FileManager::readFileAll(inputFile + "/html.html")
        },
        {
            ClipboardDataType::TEXT,
            FileManager::readFileAll(inputFile + "/text.html")
        },
    };


}

void ClipboardManager::onClipboardDataChange() const
{
    try
    {
        const QMimeData* mimeData = m_clipboard->mimeData(QClipboard::Clipboard);
        if (mimeData != nullptr)
        {
            if (mimeData->hasImage())
            {
                QString imgFileName = FileManager::storeImage(m_outputFile, mimeData->formats(), mimeData->imageData());
                renderOutput(ClipboardDataType::IMAGE, imgFileName);
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
    catch (...)
    {
        qInfo() << "something went wrong!";
    }
}

void ClipboardManager::renderOutput(const ClipboardDataType& mode,
                                    const QString& data) const
{
    QString timestamp = QString::number(QDateTime::currentSecsSinceEpoch());

    KeyValuePairs subsitutionMap
    {
        {"$data", data},
        {"$time", timestamp}
    };

    QString templateString = m_templateMap[mode];
    for (const auto& item : subsitutionMap)
    {
        templateString.replace(item.first, item.second);
    }

    FileManager::writeIntoFile(m_outputFile + "/index.html", templateString);
    FileManager::writeIntoFile(m_outputFile + "/time.txt", timestamp);
}
