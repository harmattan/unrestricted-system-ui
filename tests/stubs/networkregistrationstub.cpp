/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of systemui.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
#include "networkregistrationstub.h"

#include <QStringList>
#include <QDebug>

namespace Cellular
{

static NetworkRegistration::Mode currentMode = NetworkRegistration::Automatic;

AvailableOperator::AvailableOperator(const QString &mnc, const QString &mcc, const QString &name,
                                     AvailableOperator::Availability availability, QObject *parent) :
        QObject(parent)
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
        QObject(parent),
        success(true),
        networkOperator(NULL)
{
}

NetworkRegistration::~NetworkRegistration()
{
    for (int i = 0; i < operators.size(); ++i) {
        AvailableOperator *oper = operators.at(i);
        delete oper;
        oper = NULL;
    }
}

void NetworkRegistration::init(NetworkOperator *networkOperator)
{
    this->networkOperator = networkOperator;
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
    for (int i = 0; i < operators.size(); ++i) {
        if (operators.at(i)->mnc() == mnc && operators.at(i)->mcc() == mcc) {
            operators.at(i)->setAvailability(AvailableOperator::Current);
            if (networkOperator != NULL) {
                networkOperator->setName(operators.at(i)->name());
                networkOperator->setMnc(operators.at(i)->mnc());
                networkOperator->setMcc(operators.at(i)->mcc());
            }
        } else
            operators.at(i)->setAvailability(AvailableOperator::Available);
    }

    emit selectionCompleted(true, QString(""));
}

void NetworkRegistration::queryAvailableOperators()
{
    if (success)
        emit availableOperators(true, operators, QString(""));
    else
        emit availableOperators(false, operators, QString("empty"));
}

void NetworkRegistration::addOperator(const QString &name, const QString &mnc, const QString &mcc, AvailableOperator::Availability availability)
{
    operators << new AvailableOperator(mnc, mcc, name, availability);
}

void NetworkRegistration::setQuerySuccess(bool success)
{
    this->success = success;
}

}
