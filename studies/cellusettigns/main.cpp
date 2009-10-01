#include <QTimer>
#include <DuiButton>
#include <DuiApplication>

#include "main.h"

#include <RadioAccess>


TestObj::TestObj(DuiApplicationWindow &w) : QObject() {
    this->win = &w;
    this->visible = false;    
}
TestObj::~TestObj() { }
void TestObj::doStuff()
{
    static int counter = 0;
    qDebug() << this->visible << counter++;
    if (this->visible = !this->visible)
        this->win->show();
    else
        this->win->hide();


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
    delete networkRegistration;

    if(!success) {
        qDebug() << "not succeded because of " << reason;
        return;
    }

    for(int i=0; operators.size(); ++i) {
        if(operators.at(i)->availability() == AvailableOperator::NotAvailable)
            qDebug() << operators.at(i)->name() << " is not available";
        if(operators.at(i)->availability() == AvailableOperator::Current)
            qDebug() << operators.at(i)->name() << " is current operator";
        if(operators.at(i)->availability() == AvailableOperator::Available)
            qDebug() << operators.at(i)->name() << " is available";
    }
    if(operators.size() > 0) {        
        connect(operators.at(0), SIGNAL(selectionCompleted(bool,const QString &)), this, SLOT(selectionCompleted(bool, const QString &)));
        operators.at(0)->select();
    }

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

    TestObj obj(w);
    QTimer *timer = new QTimer(&app);
    timer->setInterval(3*1000);
    timer->start();
    QObject::connect(timer, SIGNAL(timeout()), &obj, SLOT(doStuff()));

    return app.exec();
}

