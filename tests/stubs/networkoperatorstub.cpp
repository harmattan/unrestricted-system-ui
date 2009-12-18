#include "networkoperatorstub.h"

namespace Cellular
{

NetworkOperator::NetworkOperator(QObject *parent) :
        QObject(parent)
{
}

NetworkOperator::~NetworkOperator()
{
}

QString NetworkOperator::name() const
{
    return currentName;
}

QString NetworkOperator::mnc() const
{
    return currentMnc;
}

QString NetworkOperator::mcc() const
{
    return currentMcc;
}

void NetworkOperator::setName(const QString &name)
{
    currentName = name;
}

void NetworkOperator::setMnc(const QString &mnc)
{
    currentMnc = mnc;
}

void NetworkOperator::setMcc(const QString &mcc)
{
    currentMcc = mcc;
}

bool NetworkOperator::isValid()
{
    return true;
}

}
