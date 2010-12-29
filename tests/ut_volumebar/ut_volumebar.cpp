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

#include <QtTest/QtTest>
#include <MApplication>
#include <MScalableImage>
#include <MImageWidget>
#include <QGraphicsSceneMouseEvent>
#include "ut_volumebar.h"
#include "volumebar.h"
#include "volumebarstyle.h"

QList<qreal> qPainterOpacity;
void QPainter::setOpacity(qreal opacity)
{
    qPainterOpacity.append(opacity);
}

QList<QRectF> qPainterFillRectRect;
QList<QBrush> qPainterFillRectBrush;
void QPainter::fillRect(const QRectF &rect, const QBrush &brush)
{
    qPainterFillRectRect.append(rect);
    qPainterFillRectBrush.append(brush);
}

QList<qreal> mScalableImageDrawX;
QList<qreal> mScalableImageDrawY;
QList<qreal> mScalableImageDrawW;
QList<qreal> mScalableImageDrawH;
QList<QPainter *> mScalableImagePainter;
void MScalableImage::draw(qreal x, qreal y, qreal w, qreal h, QPainter *painter) const
{
    mScalableImageDrawX.append(x);
    mScalableImageDrawY.append(y);
    mScalableImageDrawW.append(w);
    mScalableImageDrawH.append(h);
    mScalableImagePainter.append(painter);
}

QString mImageWidgetSetImageId;
QSize mImageWidgetSetImageSize;
void MImageWidget::setImage(const QString &id, const QSize &size)
{
    mImageWidgetSetImageId = id;
    mImageWidgetSetImageSize = size;
}

bool mWidgetIsOnDisplay = true;
bool MWidget::isOnDisplay() const
{
    return mWidgetIsOnDisplay;
}

void Ut_VolumeBar::initTestCase()
{
    // Create a MApplication
    static int argc = 1;
    static char *app_name = (char *)"./ut_volumebar";
    app = new MApplication(argc, &app_name);
}

void Ut_VolumeBar::cleanupTestCase()
{
    // Destroy MApplication
    delete app;
}

void Ut_VolumeBar::init()
{
    volumeBar = new VolumeBar();
}

void Ut_VolumeBar::cleanup()
{
    delete volumeBar;
    qPainterOpacity.clear();
    qPainterFillRectRect.clear();
    qPainterFillRectBrush.clear();
    mScalableImageDrawX.clear();
    mScalableImageDrawY.clear();
    mScalableImageDrawW.clear();
    mScalableImageDrawH.clear();
    mScalableImagePainter.clear();
    mImageWidgetSetImageId.clear();
    mImageWidgetSetImageSize = QSize();
    mWidgetIsOnDisplay = true;
}

void Ut_VolumeBar::testAnimationSetup()
{
    QCOMPARE(volumeBar->percentageAnimation->propertyName().constData(), "currentPercentage");
    QCOMPARE(volumeBar->fadeInAnimation->propertyName().constData(), "opacity");
    QCOMPARE(volumeBar->fadeOutAnimation->propertyName().constData(), "opacity");
    QCOMPARE(volumeBar->fadeInAnimation->startValue().toReal(), (qreal)0);
    QCOMPARE(volumeBar->fadeInAnimation->endValue().toReal(), (qreal)1);
    QCOMPARE(volumeBar->fadeOutAnimation->endValue().toReal(), (qreal)0);
    QVERIFY(disconnect(&volumeBar->visibilityTimer, SIGNAL(timeout()), volumeBar->fadeOutAnimation, SLOT(start())));
    QVERIFY(disconnect(volumeBar->fadeOutAnimation, SIGNAL(finished()), volumeBar, SLOT(finishAnimations())));
}

void Ut_VolumeBar::testSetCurrentPercentage()
{
    qreal percentage = 0.7;
    volumeBar->setCurrentPercentage(percentage);
    QCOMPARE(volumeBar->currentPercentage(), percentage);
}

void Ut_VolumeBar::testApplyStyle()
{
    VolumeBarStyle *style = const_cast<VolumeBarStyle *>(volumeBar->style().operator ->());
    style->setFadeDuration(1500);
    style->setVisibleDuration(3000);
    volumeBar->applyStyle();

    // Check that the animators and timers are set up according to the style
    QCOMPARE(volumeBar->fadeInAnimation->duration(), style->fadeDuration());
    QCOMPARE(volumeBar->fadeOutAnimation->duration(), style->fadeDuration());
    QCOMPARE(volumeBar->visibilityTimer.interval(), style->visibleDuration());
}

void Ut_VolumeBar::testDrawBackgroundFillsBackgroundAndDrawsVolumeBar()
{
    MScalableImage image;
    VolumeBarStyle *style = const_cast<VolumeBarStyle *>(volumeBar->style().operator ->());
    style->setBackgroundColor(QColor(Qt::darkRed));
    style->setBackgroundOpacity(0.5);
    style->setBackgroundImage(&image);
    style->setVolumeBarOpacity(0.5);
    volumeBar->setCurrentPercentage(0.2);

    QPainter painter;
    volumeBar->setGeometry(QRectF(0, 0, 50, 30));
    volumeBar->drawBackground(&painter, NULL);

    // Check that the background is filled with the given style
    QCOMPARE(qPainterOpacity.count(), 2);
    QCOMPARE(qPainterOpacity.at(0), style->backgroundOpacity() * volumeBar->effectiveOpacity());
    QCOMPARE(qPainterFillRectRect.count(), 1);
    QCOMPARE(qPainterFillRectRect.at(0), volumeBar->boundingRect());
    QCOMPARE(qPainterFillRectBrush.at(0), QBrush(style->backgroundColor()));

    // Check that the given background image is drawn in the correct size
    QCOMPARE(qPainterOpacity.at(1), style->volumeBarOpacity() * volumeBar->effectiveOpacity());
    QCOMPARE(mScalableImageDrawX.count(), 1);
    QCOMPARE(mScalableImageDrawX.at(0), (qreal)0);
    QCOMPARE(mScalableImageDrawY.at(0), (1 - volumeBar->currentPercentage()) * volumeBar->size().height());
    QCOMPARE(mScalableImageDrawW.at(0), volumeBar->size().width());
    QCOMPARE(mScalableImageDrawH.at(0), volumeBar->currentPercentage() * volumeBar->size().height());
}

void Ut_VolumeBar::testSetTargetPercentageSetsIconAndStartsAnimation()
{
    VolumeBarStyle *style = const_cast<VolumeBarStyle *>(volumeBar->style().operator ->());
    style->setIconId("iconId");
    style->setIconSize(QSize(50, 30));

    // Set target to a non-muted value
    qreal percentage = 0.5;
    volumeBar->setTargetPercentage(percentage);
    QCOMPARE(volumeBar->fadeOutAnimation->state(), QAbstractAnimation::Stopped);
    QCOMPARE(mImageWidgetSetImageId, style->iconId());
    QCOMPARE(mImageWidgetSetImageSize, style->iconSize());
    QCOMPARE(volumeBar->percentageAnimation->state(), QAbstractAnimation::Running);
    QCOMPARE(volumeBar->percentageAnimation->startValue().toReal(), volumeBar->currentPercentage());
    QCOMPARE(volumeBar->percentageAnimation->endValue().toReal(), percentage);
}

void Ut_VolumeBar::testSetTargetPercentageWhenMutedSetsMutedIcon()
{
    VolumeBarStyle *style = const_cast<VolumeBarStyle *>(volumeBar->style().operator ->());
    style->setMutedIconId("mutedIconId");
    style->setIconSize(QSize(50, 30));

    // Set target to a muted value
    volumeBar->setTargetPercentage(0);
    QCOMPARE(mImageWidgetSetImageId, style->mutedIconId());
    QCOMPARE(mImageWidgetSetImageSize, style->iconSize());
}

void Ut_VolumeBar::testSetTargetPercentageWhenNotOnDisplayUpdatesPercentageInstantly()
{
    // Check that current percentage is immediately set if the widget is not on display
    mWidgetIsOnDisplay = false;
    volumeBar->setTargetPercentage(1);
    QCOMPARE(volumeBar->percentageAnimation->state(), QAbstractAnimation::Stopped);
    QCOMPARE(volumeBar->currentPercentage(), (qreal)1);
}

void Ut_VolumeBar::testMousePressesAndMovesUpdatePercentage()
{
    volumeBar->setGeometry(QRectF(0, 0, 50, 30));
    mWidgetIsOnDisplay = false;

    QGraphicsSceneMouseEvent event;
    event.setPos(QPointF(25, 10));
    volumeBar->mousePressEvent(&event);
    QCOMPARE(event.isAccepted(), true);
    QCOMPARE(volumeBar->currentPercentage(), (volumeBar->geometry().height() - event.pos().y()) / volumeBar->geometry().height());

    event.setPos(QPointF(35, 20));
    volumeBar->mouseMoveEvent(&event);
    QCOMPARE(volumeBar->currentPercentage(), (volumeBar->geometry().height() - event.pos().y()) / volumeBar->geometry().height());
}

QTEST_APPLESS_MAIN(Ut_VolumeBar)
