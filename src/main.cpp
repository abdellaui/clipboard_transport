#include <QClipboard>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>
#include <QGuiApplication>

#include "ClipboardManager.h"

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  QCoreApplication::setApplicationName("clipboard transporter");
  QCoreApplication::setApplicationVersion("1.0");

  QCommandLineParser parser;
  parser.setApplicationDescription("Test helper");
  parser.addHelpOption();
  parser.addVersionOption();
  parser.addOption(
      {{"i", "input"},
       "directory of (templates) image.html, html.html, text.html.",
       "input",
       "~/playground/clipboard_transport/assets/in"});
  parser.addOption({{"o", "output"},
                    "path of output file.",
                    "output",
                    "~/playground/clipboard_transport/assets/out"});
  parser.process(app);

  QClipboard *clipboard = QGuiApplication::clipboard();

  ClipboardManager *cm = new ClipboardManager(clipboard, parser);

  QObject::connect(clipboard, &QClipboard::dataChanged, cm,
                   &ClipboardManager::onClipboardDataChange);

  qInfo() << "I'm watching your clipboard!";
  return app.exec();
}
