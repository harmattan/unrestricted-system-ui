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
#include "ut_volumebarwindow.h"
#include <volumebarwindow.h>
#include <volumebarlogic.h>
#include <volumebar.h>
#include <MApplication>
#include <QGraphicsItem>
#include <MWindow>
#include <MOverlay>

#ifdef HAVE_LIBRESOURCEQT
#include <policy/resource-set.h>
/*********************************************************************************
 * Stub for ResourcePolicy
 */

namespace ResourcePolicy
{
    ResourceSet::ResourceSet(const QString&, QObject*)
    {
    }

    bool ResourceSet::setAlwaysReply ()
    {
        return true;
    }

    ScaleButtonResource::ScaleButtonResource()
    {
    }

    void ResourceSet::addResourceObject (ResourcePolicy::Resource* rsc)
    {
        Q_UNUSED(rsc);
        return;
    }

    bool ResourceSet::acquire ()
    {
        return true;
    }

    void ResourceSet::deleteResource (ResourcePolicy::ResourceType)
    {
        return;
    }
}
#endif // HAVE_LIBRESOURCEQT

#ifdef HAVE_QMSYSTEM
#include <qmkeys.h>
/*********************************************************************************
 * Stub for MeeGo::QmKeys
 */
namespace MeeGo
{
    QmKeys::QmKeys(QObject *parent)
    {
        Q_UNUSED(parent);
    }

    QmKeys::~QmKeys()
    {
    }
}
#endif

/*********************************************************************************
 * Stub for VolumeBar
 */
VolumeBar::VolumeBar(QGraphicsItem *parent)
{
    Q_UNUSED(parent);
}

VolumeBar::~VolumeBar()
{

}

void VolumeBar::updateVolume(int val, int max)
{
    Q_UNUSED(val);
    Q_UNUSED(max);
}

void VolumeBar::updateContents()
{

}

void VolumeBar::applyStyle()
{

}

void VolumeBar::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED (event);
}

void VolumeBar::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED (event);
}

void VolumeBar::finishAnimations()
{

}

/*********************************************************************************
 * Stub for VolumeBarLogic
 */
VolumeBarLogic::VolumeBarLogic()
{
    currentvolume = 20;
}

VolumeBarLogic::~VolumeBarLogic()
{
}

quint32 VolumeBarLogic::volume()
{
    return currentvolume;
}

quint32 VolumeBarLogic::maxVolume()
{
    return 100;
}

void VolumeBarLogic::ping()
{
}

void VolumeBarLogic::setVolume(quint32 value)
{
    currentvolume = value;
}

void VolumeBarLogic::stepsUpdated(quint32 value, quint32 maxvalue)
{
    Q_UNUSED(value);
    Q_UNUSED(maxvalue);
}

void VolumeBarLogic::initValues()
{
}

void VolumeBarLogic::addSignalMatch()
{
}

void VolumeBarLogic::openConnection(bool init)
{
    Q_UNUSED(init);
}

/*********************************************************************************
 * stub for MWindow
 */
bool isWindowVisible = false;

void MWindow::setVisible(bool visible)
{
    isWindowVisible = visible;
}

/*********************************************************************************
 * The Ut_VolumeBarWindow implements the unit tests.
 */
void Ut_VolumeBarWindow::init()
{
    volumeBarWindow = new VolumeBarWindow;
}

void Ut_VolumeBarWindow::cleanup()
{
    delete volumeBarWindow;
}

static int argc = 1;
static char *argv[] = { (char *) "./ut_volumebarwindow", 0 };

void Ut_VolumeBarWindow::initTestCase()
{
    app = new MApplication(argc, argv);
}

void Ut_VolumeBarWindow::cleanupTestCase()
{
    app->deleteLater();
}

void Ut_VolumeBarWindow::testVolumeBarChanged()
{
    volumeBarWindow->volumeBarChanged(32);
    QCOMPARE(volumeBarWindow->logic->volume(), 32u);
}

#ifdef HAVE_QMSYSTEM
void Ut_VolumeBarWindow::testHwKeyEvent()
{
    // when current_volume >= max_volume
    volumeBarWindow->volumeBarChanged(120);
    volumeBarWindow->hwKeyEvent(MeeGo::QmKeys::VolumeUp, MeeGo::QmKeys::KeyDown);
    QCOMPARE(volumeBarWindow->logic->volume(), 99u);

    // Turn up the volume
    volumeBarWindow->volumeBarChanged(20);
    volumeBarWindow->hwKeyEvent(MeeGo::QmKeys::VolumeUp, MeeGo::QmKeys::KeyDown);
    QCOMPARE(volumeBarWindow->logic->volume(), 21u);

    // Turn down the volume
    volumeBarWindow->volumeBarChanged(20);
    volumeBarWindow->hwKeyEvent(MeeGo::QmKeys::VolumeDown, MeeGo::QmKeys::KeyDown);
    QCOMPARE(volumeBarWindow->logic->volume(), 19u);

    volumeBarWindow->volumeBarChanged(20);
    volumeBarWindow->hwKeyEvent(MeeGo::QmKeys::VolumeUp, MeeGo::QmKeys::KeyUp);
    QCOMPARE(volumeBarWindow->logic->volume(), 20u);

    // push some other hw key
    volumeBarWindow->volumeBarChanged(20);
    volumeBarWindow->hwKeyEvent(MeeGo::QmKeys::Camera, MeeGo::QmKeys::KeyDown);
    QCOMPARE(volumeBarWindow->logic->volume(), 20u);
}
#endif

QTEST_APPLESS_MAIN(Ut_VolumeBarWindow)
