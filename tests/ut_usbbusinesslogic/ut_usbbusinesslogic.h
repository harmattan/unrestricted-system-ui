/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#ifndef UT_USBBUSINESSLOGIC_H
#define UT_USBBUSINESSLOGIC_H

#include <QtTest/QtTest>
#include <QObject>

class MApplication;
class UsbBusinessLogic;
class UsbBusinessLogicAdaptor;

class Ut_UsbBusinessLogic : public QObject 
{
Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

    void testUsbModedRelation ();
    void testDbusAdaptor ();
    
private:
    MApplication                *m_app;
    UsbBusinessLogic            *m_logic;
    UsbBusinessLogicAdaptor     *m_adaptor;
};

#endif
