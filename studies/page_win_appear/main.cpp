/*
 * Test application.
 * Tests page and window show & hide frequently.
 */

#include "main.h"

#include <DuiButton>
#include <DuiApplication>
#include <DuiApplicationWindow>
#include <QDebug>
#include <QDBusConnection>

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
TestObj::TestObj(bool enableBack, int time) :
        QDBusAbstractAdaptor(DuiApplication::activeApplicationWindow()),
        time(time)
{
    QDBusConnection bus = QDBusConnection::sessionBus();
    if(!bus.registerObject("/com/nokia/page_win_appear/dbus", DuiApplication::activeApplicationWindow())) {
        qDebug() << Q_FUNC_INFO << "failed to register bus objects";
    } else if(!bus.registerService("com.nokia.page_win_appear.dbus")) {
        qDebug() << Q_FUNC_INFO << "failed to register bus service";
    }

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
        case 1: // show page
            time = 4;
            qDebug() << Q_FUNC_INFO << "appear, wait" << time;
            QTimer::singleShot(time*1000, this, SLOT(handlePage()));
            Show();
        break;
        case 4: // hide page
            time = 1;
            qDebug() << Q_FUNC_INFO << "disappear, wait" << time;
            QTimer::singleShot(time*1000, this, SLOT(handlePage()));
            Hide();
        break;
    }
    qDebug() << Q_FUNC_INFO << "out";
}

void TestObj::Show()
{
    p->appearNow();
    DuiApplication::activeApplicationWindow()->show();
    qDebug() << Q_FUNC_INFO << "win->isHidden()" << DuiApplication::activeApplicationWindow()->isHidden();
}

void TestObj::Hide()
{
    p->disappearNow();
    DuiApplication::activeApplicationWindow()->hide();
    qDebug() << Q_FUNC_INFO << "win->isHidden()" << DuiApplication::activeApplicationWindow()->isHidden();
}

void TestObj::Start()
{
    if(DuiApplication::activeApplicationWindow()->isHidden()){
        time = 1;
    }else{
        time = 2;
    }
    QTimer::singleShot(time*1000, this, SLOT(handlePage()));
    qDebug() << Q_FUNC_INFO << "wait" << time;
}

void flagHint(char* v0)
{
    qDebug() << "\n\nUse -e to enable escape button. Default is disabled";
    qDebug() << "Use -i to start application in idle state. Default is to start in a loop";
    qDebug() << "Use -f to set window flags defined in Qt::WindowFlags as hex. Default is 0x0";
    qDebug() << " e.g. start application in idle with escape button and set Qt::FramelessWindowHint and Qt::WindowStaysOnTopHint:";
    qDebug() << v0 << "-i -e -f 800 40000\n\n";
    sleep(3);
}

int getFlags(int argc, char** argv, int &i)
{
    int flags(0);
    QString vflag;
    for (; i < argc; ++i){
        vflag = argv[i];
        if( QString("-e") == vflag || QString("-i") == vflag ){
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
    int time = 1;
    if(argc >= 2){
        if( (argc == 2 &&
             QString(argv[1]) != QString("-i") &&
             QString(argv[1]) != QString("-e") )
            || ( argc > 2 && QString(argv[1]) != QString("-f") ) ){
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
                if (QString(argv[i]) == QString("-i")) {
                    ++i;
                    time = 0;
                }
            }
        }
    }
    qDebug() << Q_FUNC_INFO << "idle:" << (time == 0 ? true : false) << "- enable back:" << enableBack << "- window flags:" << flags;
    w.setWindowFlags(flags);

    TestObj obj(enableBack, time);
    return app.exec();
}

