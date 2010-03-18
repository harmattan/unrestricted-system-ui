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
#ifndef NETWORKREGISTRATION_H
#define NETWORKREGISTRATION_H

#include "networkoperatorstub.h"

#include <QObject>

namespace Cellular
{
//! The AvailableOperator class provides a network operator
class AvailableOperator : public QObject
{
public:

    enum Availability {
        UnknownAvailability = -1, //!< Availability unknown
        Available, //!< Operator is available
        Current, //!< Operator is the current registered operator
        NotAvailable //!< Operator is not available
    };

    AvailableOperator(const QString &mnc, const QString &mcc, const QString &name, AvailableOperator::Availability availability, QObject *parent = 0);
    ~AvailableOperator();

    //! Set operator MNC
    void setMnc(const QString &mnc);
    //! Set operator MCC
    void setMcc(const QString &mcc);
    //! Set operator Name
    void setName(const QString &name);
    //! Set operator availability
    void setAvailability(AvailableOperator::Availability availability);

    //! Get operator MNC
    QString mnc() const;
    //! Get operator MCC
    QString mcc() const;
    //! Get operator name
    QString name() const;
    //! Get operator availability
    AvailableOperator::Availability availability() const;

private:
    QString currentMnc;
    QString currentMcc;
    QString currentName;
    AvailableOperator::Availability currentAvailibility;

};

class NetworkRegistration : public QObject
{
    Q_OBJECT

public:
    NetworkRegistration(QObject *parent = 0);
    ~NetworkRegistration();

    enum Mode {
        UnknownMode = -1, //!< Network access mode is unknown
        Manual, //!< Manual network access selection
        Automatic //!< Automatic network access selection
    };

    //! Get access selection mode
    NetworkRegistration::Mode mode() const;
    //! Automatically select any available operator
    void selectOperator();
    //! Select an operator based on MNC/MCC code
    void selectOperator(const QString &mnc, const QString &mcc);
    //! Query for available networks
    void queryAvailableOperators();

    // for unit test usage
    void addOperator(const QString &name, const QString &mnc, const QString &mcc, AvailableOperator::Availability availability);
    void setQuerySuccess(bool success);
    void init(NetworkOperator *networkOperator);

signals:
    //! Access selection mode changed
    void modeChanged(int mode);
    //! Asynchronous response to available network scan
    void availableOperators(bool success, const QList<AvailableOperator*> &operators, const QString &reason);
    //! Asynchronous response to network selection
    void selectionCompleted(bool success, const QString &reason);

private:
    bool success;
    NetworkOperator *networkOperator;
    QList<AvailableOperator*> operators;

};
}

#endif // NETWORKREGISTRATION_H
