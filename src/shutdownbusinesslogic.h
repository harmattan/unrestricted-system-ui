/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef SHUTDOWNBUSINESSLOGIC_H
#define SHUTDOWNBUSINESSLOGIC_H

#include <qmsystem/qmsystemstate.h>
#include <QObject>

class ShutdownUI;

/*
 * If this macro is defined the ShutdownBusinessLogic will emulate some shutdown
 * signals 5 seconds after it has been created. I know it is a crude test
 * mechanism, but it was very simple to implement and I could test the messages
 * with it.
 */
//#define TEST_SHUTDOWN_LOGIC

/*!
 * \brief Business logic for the shutdown subsystem. 
 *
 * This class connects the systemStateChanged() signal of the QmSystem library
 * and notify the user about the state changes. The class does nothing else.
 * The messages are sent, when:
 * 1) Shutdown is happening.
 * 2) Thermal emergency shutdown is happening.
 * 3) Battery low shutdown is happening.
 * 4) The shutdown is aborted because the USB is connected.
 *
 * FIXME: The shutdown system has no adaptor, the DBus interface is not
 * implemented. There is no documentation, so we don't know if we have to
 * implement a DBus interface and if yes, what exactly we should to implement.
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
    #ifdef TEST_SHUTDOWN_LOGIC
    void testTimeoutSlot ();
    #endif

private:
    void thermalShutdown ();
    void batteryShutdown ();
    void shutdownDeniedUSB ();

private:
    ShutdownUI             *m_Ui;
    Maemo::QmSystemState   *m_State;
};

#endif
