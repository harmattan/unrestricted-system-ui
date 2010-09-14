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
****************************************************************************/

#include <MApplication>
#include <MGConfItem>
#include "ut_lockscreenview.h"

QStringList backgroundImageFilenames;
bool QPixmap::load(const QString  &fileName, const char *format, Qt::ImageConversionFlags flags)
{
    Q_UNUSED(format);
    Q_UNUSED(flags);
    backgroundImageFilenames.append(fileName);
    return true;
}

QVariant MGConfItem::value() const
{
    if(this->key() == "/desktop/meego/background/landscape/picture_filename") {
        return QVariant("Landscape");
    } else if(this->key() == "/desktop/meego/background/portrait/picture_filename") {
        return QVariant("Portrait");
    }
    return QVariant("");
}

const QPixmap *pixmapDrawn = NULL;
void QPainter::drawPixmap(const QPointF &p, const QPixmap &pm)
{
    Q_UNUSED(p)
    pixmapDrawn = &pm;
}

void Ut_LockScreenView::init()
{
    lockScreenView = new LockScreenView(&controller);
    pixmapDrawn = new QPixmap;
}

void Ut_LockScreenView::cleanup()
{
    backgroundImageFilenames.clear();
    delete pixmapDrawn;
    pixmapDrawn = NULL;

}

void Ut_LockScreenView::initTestCase()
{
    int   argc = 1;
    static char *app_name = (char *)"./ut_lockscreenview";
    app = new MApplication(argc, &app_name);
}

void Ut_LockScreenView::cleanupTestCase()
{
}

void Ut_LockScreenView::testWhenLockScreenViewIsCreatedGConfKeysAreConnectedForBackgroudImage()
{
    bool result = disconnect (lockScreenView->gconfBgLandscape, SIGNAL(valueChanged()), lockScreenView, SLOT(reloadLandscapeBackground()));
    QCOMPARE(result, true);
    result = disconnect (lockScreenView->gconfBgPortrait, SIGNAL(valueChanged()), lockScreenView, SLOT(reloadPortraitBackground()));
    QCOMPARE(result, true);
}

void Ut_LockScreenView::testWhenLockScreenCreatedBackgroundImagesLoaded()
{
    QCOMPARE(backgroundImageFilenames.count(), 2);
    QCOMPARE(backgroundImageFilenames.at(0), QString("Landscape"));
    QCOMPARE(backgroundImageFilenames.at(1), QString("Portrait"));
}

void Ut_LockScreenView::testWhenPortraitPortraitImageIsDrawn()
{
    lockScreenView->setGeometry(QRectF(0,0,20,30)); // height > width so portrait
    QCOMPARE(pixmapDrawn->width(), lockScreenView->portraitPixmap->width());
    QCOMPARE(pixmapDrawn->height(), lockScreenView->portraitPixmap->height());
}

void Ut_LockScreenView::testWhenLandscapeLndscapeImageIsDrawn()
{
    lockScreenView->setGeometry(QRectF(0,0,30,20)); // width > height so landscape
    QCOMPARE(pixmapDrawn->width(), lockScreenView->landscapePixmap->width());
    QCOMPARE(pixmapDrawn->height(), lockScreenView->landscapePixmap->height());
}

QTEST_APPLESS_MAIN(Ut_LockScreenView)
