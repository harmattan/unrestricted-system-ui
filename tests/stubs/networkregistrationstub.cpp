#include "networkregistrationstub.h"

namespace Cellular
{

static NetworkRegistration::Mode currentMode = NetworkRegistration::Automatic;

AvailableOperator::AvailableOperator(const QString &mnc, const QString &mcc, const QString &name, AvailableOperator::Availability availability)
{
    setMnc(mnc);
    setMcc(mcc);
    setName(name);
    setAvailability(availability);
}

AvailableOperator::~AvailableOperator()
{
}

void AvailableOperator::setMnc(const QString &mnc)
{
    currentMnc = mnc;
}

void AvailableOperator::setMcc(const QString &mcc)
{
    currentMcc = mcc;
}

void AvailableOperator::setName(const QString &name)
{
    currentName = name;
}

void AvailableOperator::setAvailability(AvailableOperator::Availability availability)
{
    currentAvailibility = availability;
}

QString AvailableOperator::mnc() const
{
    return currentMnc;
}

QString AvailableOperator::mcc() const
{
    return currentMcc;
}

QString AvailableOperator::name() const
{
    return currentName;
}

AvailableOperator::Availability AvailableOperator::availability() const
{
    return currentAvailibility;
}

NetworkRegistration::NetworkRegistration(QObject *parent) :
        QObject(parent)
{    
    operators << (new AvailableOperator(QString("Radiolinja"), QString("Radiolinja"), QString("Radiolinja"), AvailableOperator::Available));
    operators << (new AvailableOperator(QString("Tele"), QString("Tele"), QString("Tele"), AvailableOperator::Available));
    operators << (new AvailableOperator(QString("DNA"), QString("DNA"), QString("DNA"), AvailableOperator::Current));
}

NetworkRegistration::~NetworkRegistration()
{
    for(int i=0; i<operators.size(); ++i) {
        AvailableOperator *oper = operators.at(i);
        delete oper;
        oper = NULL;
    }
}

NetworkRegistration::Mode NetworkRegistration::mode() const
{
    return currentMode;
}

void NetworkRegistration::selectOperator()
{
    currentMode = NetworkRegistration::Automatic;    
    emit selectionCompleted(true, QString(""));    
}

void NetworkRegistration::selectOperator(const QString &mnc, const QString &mcc)
{
    currentMode = NetworkRegistration::Manual;
    for(int i=0; i<operators.size(); ++i) {        
        if(operators.at(i)->mnc() == mnc && operators.at(i)->mcc() == mcc)
            operators.at(i)->setAvailability(AvailableOperator::Current);
        else
            operators.at(i)->setAvailability(AvailableOperator::Available);
    }
    emit selectionCompleted(true, QString(""));
}

void NetworkRegistration::queryAvailableOperators()
{    
    emit availableOperators(true, operators, QString(""));
}

}
