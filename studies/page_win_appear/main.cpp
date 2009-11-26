/*
 * Test application.
 * Tests page and window show & hide frequently.
 */

#include "main.h"

#include <DuiButton>
#include <DuiApplication>
#include <DuiApplicationWindow>
#include <QDebug>

/* TestPage */
TestPage::TestPage()
{
    qDebug() << Q_FUNC_INFO;
    setFullscreen(true);
    setDisplayMode(0);
    setBackButtonEnabled(false);
    setTitle("Test page");
    createContent();
}

TestPage::~TestPage()
{
    qDebug() << Q_FUNC_INFO;
}

/*******************
 *   TestObj
 ********************/
TestObj::TestObj(bool enableBack) : QObject(), time(1)
{
    p = new TestPage();
    p->setBackButtonEnabled(enableBack);
    DuiApplicationPage::DisplayMode mod = 0;
    if(enableBack){
        mod = DuiApplicationPage::EscapeButtonVisible;
    }
    p->setDisplayMode( mod );
    handlePage();
}

TestObj::~TestObj()
{
    delete p;
}

void TestObj::handlePage()
{
    switch(time)
    {
        case 1: // create page
            time = 4;
            qDebug() << Q_FUNC_INFO << "appear, wait" << time;
            QTimer::singleShot(time*1000, this, SLOT(handlePage()));
            p->appearNow();
            DuiApplication::activeApplicationWindow()->show();
            qDebug() << Q_FUNC_INFO << "win->isHidden()" << DuiApplication::activeApplicationWindow()->isHidden();
        break;
        case 4: // delete page and wait 2 sec
            time = 1;
            qDebug() << Q_FUNC_INFO << "disappear, wait" << time;
            QTimer::singleShot(time*1000, this, SLOT(handlePage()));
            p->disappearNow();
            DuiApplication::activeApplicationWindow()->hide();
            qDebug() << Q_FUNC_INFO << "win->isHidden()" << DuiApplication::activeApplicationWindow()->isHidden();
        break;
    }
    qDebug() << Q_FUNC_INFO << "out";
}

void flagHint(char* v0)
{
    qDebug() << "\n\nUse -f to set window flags defined in Qt::WindowFlags as hex. Default is 0x0";
    qDebug() << "\n\nUse -e to enable escape button. Default is disabled";
    qDebug() << " e.g. to enable escape button and set Qt::FramelessWindowHint and Qt::WindowStaysOnTopHint:";
    qDebug() << v0 << "-e -f 800 40000\n\n";
    sleep(3);
}

int getFlags(int argc, char** argv, int &i)
{
    int flags(0);
    QString vflag;
    for (; i < argc; ++i){
        vflag = argv[i];
        if(vflag == QString("-e")){
            --i;
            break;
        }
        bool ok;
        int f = (Qt::WindowType)vflag.toInt(&ok, 16);
        flags |= f;
    }
    return flags;
}

int main(int argc, char** argv)
{
    if(argc == 1){
        flagHint(argv[0]);
    }

    DuiApplication app(argc, argv);
    DuiApplicationWindow w;
    w.setWindowOpacity(0);

    Qt::WindowFlags flags(0);
    bool enableBack(false);
    if(argc >= 2){
        if(argc == 2 || (argc > 2 &&
           QString(argv[1]) != QString("-f") &&
           QString(argv[1]) != QString("-e") )){
            flagHint(argv[0]);
        }else {
            for(int i = 1; i < argc; ++i){
               if (QString(argv[i]) == QString("-e")) {
                    ++i;
                    enableBack = true;
                }
                if (QString(argv[i]) == QString("-f")) {
                    ++i;
                    flags = (Qt::WindowType)getFlags(argc, argv, i);
                }
            }
        }
    }
    qDebug() << Q_FUNC_INFO << "enable back:" << enableBack << "window flags:" << flags;
    w.setWindowFlags(flags);

    TestObj obj(enableBack);
    return app.exec();
}

