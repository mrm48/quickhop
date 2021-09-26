#include <cstdlib>
#include <QApplication>
#include <QCommandLineParser>
#include <KAboutData>
#include <KLocalizedString>
#include "mainwindow.h"
 
int main (int argc, char *argv[])
{
    QApplication app(argc, argv);
    KLocalizedString::setApplicationDomain("quickhop");
    
    KAboutData aboutData(
                         QStringLiteral("quickhop"),
                         i18n("Quickhop"),
                         QStringLiteral("1.0"),
                         i18n("An application to put a gui around post-install scripts"),
                         KAboutLicense::GPL,
                         i18n("(c) 2021"),
                         i18n("Currently Requires no dependencies"),
                         QStringLiteral("https://github.com/mrm48/quickhop"),
                         QStringLiteral("submit@bugs.kde.org"));

    KAboutData::setApplicationData(aboutData);
 
    QCommandLineParser parser;
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);
    
    MainWindow *window = new MainWindow();
    window->show();
    
    return app.exec();
}
