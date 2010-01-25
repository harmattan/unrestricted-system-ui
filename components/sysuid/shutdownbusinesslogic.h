/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef SHUTDOWNBUSINESSLOGIC_H
#define SHUTDOWNBUSINESSLOGIC_H

#include <qmsystem/qmsystemstate.h>

#include <QObject>

class ShutdownUI;

/*!
 * Business logic for the shutdown subsystem. 
 * FIXME: The shutdown system has no adaptor, the DBus interface is not
 * implemented. There is no documentation, so we don't know what to implement.
 */
class ShutdownBusinessLogic : public QObject
{
    Q_OBJECT

public:
    ShutdownBusinessLogic (QObject *parent = 0);
    virtual ~ShutdownBusinessLogic();

    void showUI();

public slots:
    void systemStateChanged (Maemo::QmSystemState::StateIndication what);

private:
    void thermalShutdown ();
    void batteryShutdown ();
    void shutdownDeniedUSB ();

private:
    ShutdownUI             *m_Ui;
    Maemo::QmSystemState   *m_State;
};

#endif
