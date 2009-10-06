#ifndef UT_UNLOCKSLIDERWIDGET_H
#define UT_UNLOCKSLIDERWIDGET_H

#include <QtTest/QtTest>
#include <QObject>

#include "unlockslider.h"

//Q_DECLARE_METATYPE(DuiButton*);

class Ut_UnlockSlider : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    //void initTestCase();
    //void cleanupTestCase();

    void testConstructionAndDestruction();
    void testHandlePressed();
    void testPosition();
    void testIconVisible();
    void testMagnetic();
    void testOrientation();
    void testReset();

    // signals:
    // -unlocked
    // -released

    // blink

private:
    UnlockSlider* m_subject;
    UnlockSlider* m_tmp;
};

#endif
