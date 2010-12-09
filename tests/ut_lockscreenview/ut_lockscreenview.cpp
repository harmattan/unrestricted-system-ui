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
#include <MSceneManager>
#include "lockscreenheader_stub.h"
#include "ut_lockscreenview.h"

QStringList qPixmapLoadFilenames;
QList<QPixmap *> qPixmapLoadPixmaps;
bool QPixmap::load(const QString &fileName, const char *, Qt::ImageConversionFlags)
{
    qPixmapLoadPixmaps.append(this);
    qPixmapLoadFilenames.append(fileName);
    return true;
}

bool QPixmap::isNull() const
{
    return false;
}

QVariant mGConfItemValueLandscape;
QVariant mGConfItemValuePortrait;
QVariant MGConfItem::value() const
{
    if (key() == "/desktop/meego/background/landscape/picture_filename") {
        return mGConfItemValueLandscape;
    } else if (key() == "/desktop/meego/background/portrait/picture_filename") {
        return mGConfItemValuePortrait;
    }

    return QVariant();
}

QRectF qPainterDrawPixmapTargetRect;
const QPixmap *qPainterDrawPixmapPixmap = NULL;
QRectF qPainterDrawPixmapSourceRect;
void QPainter::drawPixmap(const QRectF &targetRect, const QPixmap &pixmap, const QRectF &sourceRect)
{
    qPainterDrawPixmapTargetRect = targetRect;
    qPainterDrawPixmapPixmap = &pixmap;
    qPainterDrawPixmapSourceRect = sourceRect;
}

bool mWidgetViewDrawBackgroundCalled = false;
void MWidgetView::drawBackground(QPainter *, const QStyleOptionGraphicsItem *) const
{
    mWidgetViewDrawBackgroundCalled = true;
}

QStringList mThemePixmapIds;
const QPixmap *mThemePixmapPixmap = NULL;
const QPixmap *MTheme::pixmap(const QString &id, const QSize &)
{
    mThemePixmapIds.append(id);
    return mThemePixmapPixmap;
}

void MTheme::releasePixmap(const QPixmap*)
{
}

M::Orientation mSceneManagerOrientation = M::Portrait;
M::Orientation MSceneManager::orientation() const
{
    return mSceneManagerOrientation;
}

void Ut_LockScreenView::init()
{
    mGConfItemValueLandscape = QVariant("/landscape.png");
    mGConfItemValuePortrait = QVariant("/portrait.png");
    controller = new MWidgetController;
    lockScreenView = new LockScreenView(controller);
    controller->setView(lockScreenView);
}

void Ut_LockScreenView::cleanup()
{
    delete controller;
    qPixmapLoadFilenames.clear();
    qPainterDrawPixmapTargetRect = QRectF();
    qPainterDrawPixmapPixmap = NULL;
    qPainterDrawPixmapSourceRect = QRectF();
    mWidgetViewDrawBackgroundCalled = false;
    mThemePixmapIds.clear();
    qPixmapLoadPixmaps.clear();
    mSceneManagerOrientation = M::Portrait;
}

void Ut_LockScreenView::initTestCase()
{
    int argc = 1;
    static char *app_name = (char *)"./ut_lockscreenview";
    app = new MApplication(argc, &app_name);
}

void Ut_LockScreenView::cleanupTestCase()
{
    delete app;
}

void Ut_LockScreenView::testGConfKeyConnections()
{
    QVERIFY(disconnect(lockScreenView->landscapePixmap.gConfItem, SIGNAL(valueChanged()), &lockScreenView->landscapePixmap, SLOT(updatePixmap())));
    QVERIFY(disconnect(lockScreenView->portraitPixmap.gConfItem, SIGNAL(valueChanged()), &lockScreenView->portraitPixmap, SLOT(updatePixmap())));
}

void Ut_LockScreenView::testUpdatePixmapWithNoImage()
{
    mGConfItemValueLandscape = QVariant();
    mGConfItemValuePortrait = QVariant();
    lockScreenView->landscapePixmap.updatePixmap();
    lockScreenView->portraitPixmap.updatePixmap();
    QCOMPARE(lockScreenView->portraitPixmap.pixmap, (QPixmap *)NULL);
    QCOMPARE(controller->styleName(), QString("LockScreenWithDefaultBackground"));
}

void Ut_LockScreenView::testUpdatePixmapFromTheme()
{
    qPixmapLoadFilenames.clear();
    qPixmapLoadPixmaps.clear();

    QPixmap pixmap;
    mThemePixmapPixmap = &pixmap;
    mGConfItemValueLandscape = QVariant("fromThemeLandscape");
    mGConfItemValuePortrait = QVariant("fromThemePortrait");
    lockScreenView->landscapePixmap.updatePixmap();
    lockScreenView->portraitPixmap.updatePixmap();
    QCOMPARE(mThemePixmapIds.count(), 2);
    QCOMPARE(mThemePixmapIds.at(0), mGConfItemValueLandscape.toString());
    QCOMPARE(mThemePixmapIds.at(1), mGConfItemValuePortrait.toString());
    QCOMPARE(qPixmapLoadFilenames.count(), 0);
    QCOMPARE(lockScreenView->landscapePixmap.pixmap, mThemePixmapPixmap);
    QCOMPARE(lockScreenView->portraitPixmap.pixmap, mThemePixmapPixmap);
    QCOMPARE(controller->styleName(), QString("LockScreenWithCustomBackground"));
}

void Ut_LockScreenView::testUpdatePixmapFromFile()
{
    qPixmapLoadFilenames.clear();
    qPixmapLoadPixmaps.clear();

    mGConfItemValueLandscape = QVariant("/fromFileLandscape.png");
    mGConfItemValuePortrait = QVariant("/fromFilePortrait.png");
    lockScreenView->landscapePixmap.updatePixmap();
    lockScreenView->portraitPixmap.updatePixmap();
    QCOMPARE(qPixmapLoadFilenames.count(), 2);
    QCOMPARE(qPixmapLoadFilenames.at(0), mGConfItemValueLandscape.toString());
    QCOMPARE(qPixmapLoadFilenames.at(1), mGConfItemValuePortrait.toString());
    QCOMPARE(mThemePixmapIds.isEmpty(), true);
    QCOMPARE(qPixmapLoadPixmaps.count(), 2);
    QCOMPARE(lockScreenView->landscapePixmap.pixmap, qPixmapLoadPixmaps.at(0));
    QCOMPARE(lockScreenView->portraitPixmap.pixmap, qPixmapLoadPixmaps.at(1));
    QCOMPARE(controller->styleName(), QString("LockScreenWithCustomBackground"));
}

void Ut_LockScreenView::testInitialImageLoading()
{
    QCOMPARE(qPixmapLoadFilenames.count(), 2);
    QCOMPARE(qPixmapLoadFilenames.at(0), mGConfItemValueLandscape.toString());
    QCOMPARE(qPixmapLoadFilenames.at(1), mGConfItemValuePortrait.toString());
}

void Ut_LockScreenView::testDrawBackgroundWithDefaultPixmap()
{
    mGConfItemValuePortrait = QVariant();
    lockScreenView->portraitPixmap.updatePixmap();

    QPainter painter;
    lockScreenView->drawBackground(&painter, NULL);
    QCOMPARE(mWidgetViewDrawBackgroundCalled, true);
}

void Ut_LockScreenView::testDrawBackgroundWithLoadedPixmap_data()
{
    QTest::addColumn<M::Orientation>("orientation");

    QTest::newRow("Landscape") << M::Landscape;
    QTest::newRow("Portrait") << M::Portrait;
}

void Ut_LockScreenView::testDrawBackgroundWithLoadedPixmap()
{
    QFETCH(M::Orientation, orientation);

    mSceneManagerOrientation = orientation;
    LockScreenBackgroundPixmap *pixmap = orientation == M::Landscape ? &lockScreenView->landscapePixmap : &lockScreenView->portraitPixmap;

    QPainter painter;
    controller->setGeometry(QRectF(0, 0, 20, 30));
    lockScreenView->drawBackground(&painter, NULL);
    QCOMPARE(mWidgetViewDrawBackgroundCalled, false);
    QCOMPARE(qPainterDrawPixmapTargetRect, QRectF(QPointF(), controller->geometry().size()));
    QCOMPARE(qPainterDrawPixmapPixmap, pixmap->pixmap);
    QCOMPARE(qPainterDrawPixmapSourceRect, QRectF(0, 0, pixmap->pixmap->width(), pixmap->pixmap->height()));
}

QTEST_APPLESS_MAIN(Ut_LockScreenView)
