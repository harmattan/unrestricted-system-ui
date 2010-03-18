/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef Ut_LockScreenUIWIDGET_H
#define Ut_LockScreenUIWIDGET_H

#include <QtTest/QtTest>
#include <QObject>

class Ut_LockScreenUI : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

private:
};

#endif
