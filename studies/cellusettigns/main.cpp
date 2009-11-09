#include <QTimer>
#include <DuiButton>
#include <DuiApplication>

#include "main.h"

#include <RadioAccess>

#include <QDBusInterface>
#include <QDebug>


TestObj::TestObj(DuiApplicationWindow &w) : QObject() {
}
TestObj::~TestObj() { }
void TestObj::doStuff()
{
    RadioAccess ra;
    RadioAccess::Mode raMode;
    ra.setMode(RadioAccess::UnknownMode);
    raMode = ra.mode();
    qDebug() << raMode;
    ra.setMode(RadioAccess::DualMode);
    raMode = ra.mode();
    qDebug() << raMode;
    ra.setMode(RadioAccess::GSMMode);
    raMode = ra.mode();
    qDebug() << raMode;
    ra.setMode(RadioAccess::UMTSMode);
    raMode = ra.mode();
    qDebug() << raMode;

    NetworkRegistration nr;
    NetworkRegistration::Mode nrMode;
    nr.setMode(NetworkRegistration::UnknownMode);
    nrMode = nr.mode();
    qDebug() << nrMode;
    nr.setMode(NetworkRegistration::Manual);
    nrMode = nr.mode();
    qDebug() << nrMode;
    nr.setMode(NetworkRegistration::Automatic);
    nrMode = nr.mode();
    qDebug() << nrMode;

    networkRegistration = new NetworkRegistration();
    connect(networkRegistration, SIGNAL(availableOperators(bool, const QList<AvailableOperator*> &, const QString &)),
           this, SLOT(availableNetworksReceived(bool, const QList<AvailableOperator*> &, const QString &)));
    networkRegistration->queryAvailableOperators();

}

void TestObj::availableNetworksReceived(bool success, const QList<AvailableOperator*> &operators, const QString &reason)
{
    networkRegistration->deleteLater();

    if(!success) {
        qDebug() << "not succeded because of " << reason;
        return;
    } else
	qDebug() << "success";




    for(int i=0; i<operators.size(); ++i) {
	qDebug() << i;
        if(operators.at(i)->availability() == AvailableOperator::NotAvailable)
            qDebug() << operators.at(i)->name() << " is not available";
        if(operators.at(i)->availability() == AvailableOperator::Current)
            qDebug() << operators.at(i)->name() << " is current operator";
        if(operators.at(i)->availability() == AvailableOperator::Available)
            qDebug() << operators.at(i)->name() << " is available";
    }
    qDebug() << "asdf";
    if(operators.size() > 0) {
	qDebug() << operators.at(0);
        connect(operators.at(0), SIGNAL(selectionCompleted(bool,const QString &)), this, SLOT(selectionCompleted(bool, const QString &)));
        operators.at(0)->select();
    }
    qDebug() << "asdfas";

}

void TestObj::selectionCompleted(bool success, const QString &reason)
{
    if(success)
        qDebug() << "Operator selected sucesfully";
    else
        qDebug() << "Operator selecttion failed because of " << reason;
}

int main(int argc, char** argv)
{
    DuiApplication app(argc, argv);
    qInstallMsgHandler(0);
    DuiApplicationWindow w;

    DuiApplicationPage p;
    DuiButton button(QString("Just another widget"), &p);

    p.appear();
    w.show();

    TestObj obj(w);
    //QTimer *timer = new QTimer(&app);
    //Btimer->setInterval(3*1000);
    //timer->start();
    //QObject::connect(timer, SIGNAL(timeout()), &obj, SLOT(doStuff()));


    QDBusInterface dbus("com.nokia.csd.CSNet",
                        "/com/nokia/csd/csnet/radio",
                       "com.nokia.csd.CSNet.RadioAccess",
                       QDBusConnection::systemBus());
    qDebug() << "Mode: " << dbus.property("SelectionMode");
    qDebug() << dbus.lastError();

    obj.doStuff();


    return app.exec();
}

