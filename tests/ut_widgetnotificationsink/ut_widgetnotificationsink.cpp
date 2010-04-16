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

#include <MRemoteAction>
#include "ut_widgetnotificationsink.h"
#include "widgetnotificationsink.h"
#include "../stubs/testnotificationparameters.h"
#include "genericnotificationparameterfactory.h"
#include "notificationmanager_stub.h"
#include "eventtypestore_stub.h"
#include <QImageReader>
#include <MApplication>
#include "sysuid_stub.h"

// xlib.h defines Status, undef it so we can use QSettings::Status
#ifdef Status
#undef Status
#endif

// List of event type files
QStringList eventTypeFilesList;

class TestWidgetNotificationSink : public WidgetNotificationSink
{
public:
    MInfoBanner *createInfoBanner(const Notification &);
    MInfoBanner *createInfoBanner(MInfoBanner::BannerType, uint groupId, const NotificationParameters &);
    void addNotification(const Notification &);
    void removeNotification(uint);
    QString determineIconId(const NotificationParameters &);
    void updateActions(MInfoBanner *infoBanner, const Notification &notification);
};

MInfoBanner *TestWidgetNotificationSink::createInfoBanner(const Notification &n)
{
    return WidgetNotificationSink::createInfoBanner(n);
}

MInfoBanner *TestWidgetNotificationSink::createInfoBanner(MInfoBanner::BannerType type, uint groupId, const NotificationParameters &parameters)
{
    return WidgetNotificationSink::createInfoBanner(type, groupId, parameters);
}

void TestWidgetNotificationSink::addNotification(const Notification &)
{
}

void TestWidgetNotificationSink::removeNotification(uint)
{
}

QString TestWidgetNotificationSink::determineIconId(const NotificationParameters &params)
{
    return WidgetNotificationSink::determineIconId(params);
}

void TestWidgetNotificationSink::updateActions(MInfoBanner *infoBanner, const Notification &notification)
{
    WidgetNotificationSink::updateActions(infoBanner, notification.parameters());
}

maemosec::storage::~storage()
{
}

// QFileInfo stubs
bool QFileInfo::exists() const
{
    return true;
}

// QDir Stubs
bool QDir::exists() const
{
    return true;
}

QStringList QDir::entryList(const QStringList &, Filters, SortFlags) const
{
    return eventTypeFilesList;
}

// QGraphicsWidget stubs (used by MCompositorNotificationSink)
void QGraphicsWidget::addAction(QAction *action)
{
    Ut_WidgetNotificationSink::actions[this].append(action);

    MRemoteAction *dra = dynamic_cast<MRemoteAction *>(action);
    if (dra != NULL) {
        Ut_WidgetNotificationSink::contents.append(dra->toString());
    }
}

void QGraphicsWidget::removeAction(QAction *action)
{
    Ut_WidgetNotificationSink::actions[this].removeAll(action);

    MRemoteAction *dra = dynamic_cast<MRemoteAction *>(action);
    if (dra != NULL) {
        Ut_WidgetNotificationSink::contents.removeAll(dra->toString());
    }
}

QList<QAction *> QGraphicsWidget::actions() const
{
    return Ut_WidgetNotificationSink::actions[this];
}

void QAction::activate(ActionEvent)
{
    Ut_WidgetNotificationSink::actionTriggeredCount++;
}

//constants for loading icon image from path tests
static const int ICON_DEFAULT_SIZE = 64;
static const int ICON_SMALL_SIZE = 40;
static const int ICON_BIG_SIZE = 100;
static const int ICON_TOO_BIG_SIZE = 101;

// QImageReader stubs
static bool imageReaderCanRead = false;

QSize QImageReader::size() const
{
    return Ut_WidgetNotificationSink::testImage->size();
}

bool QImageReader::canRead() const
{
    return imageReaderCanRead;
}

void QImageReader::setScaledSize(const QSize &size)
{
    *Ut_WidgetNotificationSink::testImage = Ut_WidgetNotificationSink::testImage->scaled(size);
}

QImage QImageReader::read()
{
    return *Ut_WidgetNotificationSink::testImage;
}

// Test images as base64 strings
// This is a black image size of 88x88
static const QString imageStrBase64 = QString("iVBORw0KGgoAAAANSUhEUgAAAFgAAABYCAAAAABU/m/oAAAAAXNSR0IArs4c6QAAAAlwSFlzAAAL"
                                      "EwAACxMBAJqcGAAAAAd0SU1FB9oBEgogIWx7kmcAAAAIdEVYdENvbW1lbnQA9syWvwAAAB5JREFU"
                                      "WMPtwTEBAAAAwqD1T20KP6AAAAAAAAAA4GcemAABAQpkmQAAAABJRU5ErkJggg==");

// This is a broken black image size of 88x88
static const QString badImageStrBase64 = QString("iiVBORw0KGgoAAAANSUhEUgAAAFgAAABYCAAAAABU/m/oAAAAAXNSR0IArs4c6QAAAAlwSFlzAAAL"
        "EwAACxMBAJqcGAAAAAd0SU1FB9oBEgogIWx7kmcAAAAIdEVYdENvbW1lbnQA9syWvwAAAB5JREFU"
        "WMPtwTEBAAAAwqD1T20KP6AAAAAAAAAA4GcemAABAQpkmQAAAABJRU5ErkJggg==");

void initImage(const QString &imageStr, int iconWidth, int iconHeight)
{
    QByteArray imageBA = QByteArray::fromBase64(imageStr.toUtf8());
    if (!Ut_WidgetNotificationSink::testImage->loadFromData(imageBA)) {
        qWarning() << "Error creating image!";
    }
    *Ut_WidgetNotificationSink::testImage = Ut_WidgetNotificationSink::testImage->scaled(QSize(iconWidth, iconHeight));
}

QList<QString> Ut_WidgetNotificationSink::contents;
QHash<const QGraphicsWidget *, QList<QAction *> > Ut_WidgetNotificationSink::actions;
int Ut_WidgetNotificationSink::actionTriggeredCount;
QImage *Ut_WidgetNotificationSink::testImage = NULL;

void Ut_WidgetNotificationSink::initTestCase()
{
    // Create a MAapplication
    static int argc = 1;
    static char *app_name = (char *)"./ut_widgetnotificationsink";
    app = new MApplication(argc, &app_name);
    testImage = new QImage();
}

void Ut_WidgetNotificationSink::cleanupTestCase()
{
    delete testImage;
    // Destroy MApplication
    delete app;
}

void Ut_WidgetNotificationSink::init()
{
    eventTypeFilesList.append("message-received.conf");
    eventTypeFilesList.append("voicemail.conf");
    m_subject = new TestWidgetNotificationSink();
    contents.clear();
    actions.clear();
    actionTriggeredCount = 0;
}

void Ut_WidgetNotificationSink::cleanup()
{
    imageReaderCanRead = false;
    *testImage = QImage();
    delete m_subject;
    eventTypeFilesList.clear();
}

void Ut_WidgetNotificationSink::testWithEventTypeAndIconId()
{
    QSettings *settings = new QSettings;
    settings->setValue(NotificationWidgetParameterFactory::iconIdKey(), "Icon-messages");
    gEventTypeStoreStub->stubSetReturnValue("settingsForEventType", settings);

    TestNotificationParameters parameters;
    parameters.add(GenericNotificationParameterFactory::eventTypeKey(), "message-received");
    parameters.add(NotificationWidgetParameterFactory::createIconIdParameter("icon0"));
    QCOMPARE(m_subject->determineIconId(parameters), QString("icon0"));
}

void Ut_WidgetNotificationSink::testWithEventTypeWithoutIconId()
{
    QSettings *settings = new QSettings;
    settings->setValue(NotificationWidgetParameterFactory::iconIdKey(), "Icon-messages");
    gEventTypeStoreStub->stubSetReturnValue("settingsForEventType", settings);

    TestNotificationParameters parameters;
    parameters.add(GenericNotificationParameterFactory::eventTypeKey(), "message-received");
    parameters.add(NotificationWidgetParameterFactory::createIconIdParameter(""));
    QCOMPARE(m_subject->determineIconId(parameters), QString("Icon-messages"));
}

void Ut_WidgetNotificationSink::testWithoutEventTypeOrIconId()
{
    TestNotificationParameters parameters;
    parameters.add(GenericNotificationParameterFactory::eventTypeKey(), "");
    parameters.add(NotificationWidgetParameterFactory::createIconIdParameter(""));
    QCOMPARE(m_subject->determineIconId(parameters), QString("default"));
}

void Ut_WidgetNotificationSink::testWithoutEventTypeWithIconId()
{
    TestNotificationParameters parameters;
    parameters.add(GenericNotificationParameterFactory::eventTypeKey(), "");
    parameters.add(NotificationWidgetParameterFactory::createIconIdParameter("icon0"));
    QCOMPARE(m_subject->determineIconId(parameters), QString("icon0"));
}

void Ut_WidgetNotificationSink::testUpdateActions()
{
    // Create notification parameters
    Notification notification;
    TestNotificationParameters parameters;
    parameters.add(NotificationWidgetParameterFactory::createActionParameter("content0 0 0 0"));
    notification.setParameters(parameters);

    // Create an info banner with one action
    MInfoBanner infoBanner(MInfoBanner::Event);
    QAction *action = new QAction(&infoBanner);
    infoBanner.addAction(action);

    // There shouldn't be any MRemoteActions at this point but the action should be added
    QCOMPARE(contents.count(), 0);
    QCOMPARE(actions.count(), 1);
    QCOMPARE(actions[&infoBanner].at(0), action);

    // Update the actions
    m_subject->updateActions(&infoBanner, notification);

    // There should be a MRemoteAction at this point
    QCOMPARE(contents.count(), 1);
    QCOMPARE(contents[0], QString("content0 0 0 0"));
    QCOMPARE(actions.count(), 1);
    QVERIFY(dynamic_cast<MRemoteAction *>(actions[&infoBanner].at(0)) != NULL);
    QCOMPARE(dynamic_cast<MRemoteAction *>(actions[&infoBanner].at(0))->toString(), QString("content0 0 0 0"));
}

void Ut_WidgetNotificationSink::testInfoBannerClicking()
{
    uint notificationID = 0;

    // Create info banner
    QApplication::processEvents();
    TestNotificationParameters parameters;
    parameters.add(NotificationWidgetParameterFactory::createActionParameter("content0 0 0 0"));
    MInfoBanner *infoBanner = m_subject->createInfoBanner(Notification(notificationID, 0, 1, parameters, Notification::ApplicationEvent, 1000));

    // Listen to triggered signals of info banner action
    QCOMPARE(actions.count(), 1);
    MRemoteAction *action = dynamic_cast<MRemoteAction *>(actions[infoBanner].at(0));
    QVERIFY(action != NULL);

    // Listen to notification removal requests of WidgetNotificationSink
    QSignalSpy notificationRemovalRequested(m_subject, SIGNAL(notificationRemovalRequested(uint)));

    // Click the infoBanner
    connect(this, SIGNAL(click()), infoBanner, SIGNAL(clicked()));
    emit click();
    disconnect(this, SIGNAL(click()), infoBanner, SIGNAL(clicked()));
    QApplication::processEvents();

    // Verify that action was triggered
    QCOMPARE(actionTriggeredCount, 1);

    // Verify that request for notification removal was emitted
    QCOMPARE(notificationRemovalRequested.count(), 1);
    QCOMPARE(notificationRemovalRequested.first().at(0).toUInt(), notificationID);



    // Create a group info banner
    uint groupID = 1;
    TestNotificationParameters groupParameters;
    groupParameters.add(NotificationWidgetParameterFactory::createActionParameter("content1 1 1 1"));
    infoBanner = m_subject->createInfoBanner(MInfoBanner::Information, groupID, groupParameters);

    // Listen to triggered signals of info banner action
    QCOMPARE(actions.count(), 2);
    action = dynamic_cast<MRemoteAction *>(actions[infoBanner].at(0));
    QVERIFY(action != NULL);

    // Listen to notification removal requests of WidgetNotificationSink
    QSignalSpy notificationGroupClearingRequested(m_subject, SIGNAL(notificationGroupClearingRequested(uint)));

    // Click the infoBanner
    connect(this, SIGNAL(click()), infoBanner, SIGNAL(clicked()));
    emit click();
    disconnect(this, SIGNAL(click()), infoBanner, SIGNAL(clicked()));
    QApplication::processEvents();

    // Verify that action was triggered
    QCOMPARE(actionTriggeredCount, 2);

    // Verify that request for notification removal was emitted
    QCOMPARE(notificationGroupClearingRequested.count(), 1);
    QCOMPARE(notificationGroupClearingRequested.first().at(0).toUInt(), groupID);
}

void Ut_WidgetNotificationSink::testInfoBannerClickingWhenNotUserRemovable(TestNotificationParameters &parameters)
{
    // Create info banner from the parameters by first adding an action into the parameters
    QApplication::processEvents();
    uint notificationID = 0;
    parameters.add(NotificationWidgetParameterFactory::createActionParameter("content0 0 0 0"));
    MInfoBanner *infoBanner = m_subject->createInfoBanner(Notification(notificationID, 0, 1, parameters, Notification::ApplicationEvent, 1000));

    // Listen to triggered signals of info banner action
    QCOMPARE(actions.count(), 1);
    MRemoteAction *action = dynamic_cast<MRemoteAction *>(actions[infoBanner].at(0));
    QVERIFY(action != NULL);

    // Listen to notification removal requests of WidgetNotificationSink
    QSignalSpy notificationRemovalRequested(m_subject, SIGNAL(notificationRemovalRequested(uint)));

    // Click the infoBanner
    connect(this, SIGNAL(click()), infoBanner, SIGNAL(clicked()));
    emit click();
    disconnect(this, SIGNAL(click()), infoBanner, SIGNAL(clicked()));
    QApplication::processEvents();

    // Verify that action was triggered even though the info banner is not user removable
    QCOMPARE(actionTriggeredCount, 1);

    // Verify that request for notification removal was not emitted since the info banner is not user removable
    QCOMPARE(notificationRemovalRequested.count(), 0);
}

void Ut_WidgetNotificationSink::testInfoBannerClickingWhenNotUserRemovableInParameters()
{
    TestNotificationParameters parameters;
    parameters.add(NotificationWidgetParameterFactory::createUserRemovableParameter(false));

    testInfoBannerClickingWhenNotUserRemovable(parameters);
}

void Ut_WidgetNotificationSink::testInfoBannerClickingWhenNotUserRemovableByEventType()
{
    QSettings settings;
    settings.setValue(NotificationWidgetParameterFactory::userRemovableKey(), false);
    gEventTypeStoreStub->stubSetReturnValue("settingsForEventType", &settings);

    TestNotificationParameters parameters;
    parameters.add(GenericNotificationParameterFactory::eventTypeKey(), "voicemail");

    testInfoBannerClickingWhenNotUserRemovable(parameters);
}


void Ut_WidgetNotificationSink::testInfoBannerCreationWithRemoteAction()
{
    TestNotificationParameters parameters("icon0", "body0", "buttonicon0", "content0 0 0 0");

    MInfoBanner *infoBanner = m_subject->createInfoBanner(Notification(3, 1, 0, parameters, Notification::ApplicationEvent, 1020));
    QCOMPARE(infoBanner->bannerType(), MInfoBanner::Event);
    QCOMPARE(infoBanner->imageID(), QString("icon0"));
    QCOMPARE(infoBanner->bodyText(), QString("body0"));
    QCOMPARE(infoBanner->iconID(), QString("buttonicon0"));
    QCOMPARE(infoBanner->actions().count(), 1);
    MRemoteAction *remoteAction = dynamic_cast<MRemoteAction *>(infoBanner->actions().at(0));
    QVERIFY(remoteAction != NULL);
    QCOMPARE(remoteAction->toString(), QString("content0 0 0 0"));
    delete infoBanner;
}

void Ut_WidgetNotificationSink::testInfoBannerCreationWithoutRemoteAction()
{
    TestNotificationParameters parameters("icon1", "body1", "buttonicon1", "");
    MInfoBanner *infoBanner = m_subject->createInfoBanner(Notification(3, 1, 0, parameters, Notification::SystemEvent, 1020));
    QCOMPARE(infoBanner->bannerType(), MInfoBanner::Information);
    QCOMPARE(infoBanner->imageID(), QString("icon1"));
    QCOMPARE(infoBanner->bodyText(), QString("body1"));
    QCOMPARE(infoBanner->iconID(), QString("buttonicon1"));
    QCOMPARE(infoBanner->actions().count(), 0);
    delete infoBanner;
}

void Ut_WidgetNotificationSink::testInfoBannerCreationWithNotificationParameters()
{
    TestNotificationParameters parameters("icon3", "body3", "buttonicon3", "content1 2 3 4");
    MInfoBanner *infoBanner = m_subject->createInfoBanner(MInfoBanner::Event, 1, parameters);
    QCOMPARE(infoBanner->bannerType(), MInfoBanner::Event);
    QCOMPARE(infoBanner->imageID(), QString("icon3"));
    QCOMPARE(infoBanner->bodyText(), QString("body3"));
    QCOMPARE(infoBanner->iconID(), QString("buttonicon3"));
    QCOMPARE(infoBanner->actions().count(), 1);
    MRemoteAction *remoteAction = dynamic_cast<MRemoteAction *>(infoBanner->actions().at(0));
    QVERIFY(remoteAction != NULL);
    QCOMPARE(remoteAction->toString(), QString("content1 2 3 4"));
    delete infoBanner;
}

void Ut_WidgetNotificationSink::testLoadIconDefaultSize()
{
    imageReaderCanRead = true;
    initImage(imageStrBase64, ICON_DEFAULT_SIZE, ICON_DEFAULT_SIZE);

    TestNotificationParameters parameters("/path/image.png");
    MInfoBanner *infoBanner = m_subject->createInfoBanner(MInfoBanner::Event, 1, parameters);
    QCOMPARE(infoBanner->pixmap().size(), QSize(ICON_DEFAULT_SIZE, ICON_DEFAULT_SIZE));
    delete infoBanner;
}

void Ut_WidgetNotificationSink::testLoadIconSmallerThanDefault()
{
    imageReaderCanRead = true;
    initImage(imageStrBase64, ICON_SMALL_SIZE, ICON_SMALL_SIZE);

    TestNotificationParameters parameters("/path/image.png");
    MInfoBanner *infoBanner = m_subject->createInfoBanner(MInfoBanner::Event, 1, parameters);
    QCOMPARE(infoBanner->pixmap().size(), QSize(ICON_DEFAULT_SIZE, ICON_DEFAULT_SIZE));
    delete infoBanner;
}

void Ut_WidgetNotificationSink::testLoadIconScaleDownToFit()
{
    imageReaderCanRead = true;
    initImage(imageStrBase64, ICON_BIG_SIZE, ICON_BIG_SIZE);

    TestNotificationParameters parameters("/path/image.png");
    MInfoBanner *infoBanner = m_subject->createInfoBanner(MInfoBanner::Event, 1, parameters);
    QCOMPARE(infoBanner->pixmap().size(), QSize(ICON_DEFAULT_SIZE, ICON_DEFAULT_SIZE));
    delete infoBanner;
}

void Ut_WidgetNotificationSink::testLoadIconScaleToFitBigHeight()
{
    imageReaderCanRead = true;
    initImage(imageStrBase64, ICON_DEFAULT_SIZE, ICON_BIG_SIZE);

    QSize testSize = testImage->size();
    testSize.scale(ICON_DEFAULT_SIZE, ICON_DEFAULT_SIZE, Qt::KeepAspectRatio);

    TestNotificationParameters parameters("/path/image.png");
    MInfoBanner *infoBanner = m_subject->createInfoBanner(MInfoBanner::Event, 1, parameters);
    QCOMPARE(infoBanner->pixmap().size(), testSize);
    delete infoBanner;
}

void Ut_WidgetNotificationSink::testLoadIconScaleToFitBigWidth()
{
    imageReaderCanRead = true;
    initImage(imageStrBase64, ICON_BIG_SIZE, ICON_DEFAULT_SIZE);

    QSize testSize = testImage->size();
    testSize.scale(ICON_DEFAULT_SIZE, ICON_DEFAULT_SIZE, Qt::KeepAspectRatio);

    TestNotificationParameters parameters("/path/image.png");
    MInfoBanner *infoBanner = m_subject->createInfoBanner(MInfoBanner::Event, 1, parameters);
    QCOMPARE(infoBanner->pixmap().size(), testSize);
    delete infoBanner;
}

void Ut_WidgetNotificationSink::testLoadIconTooBig()
{
    imageReaderCanRead = true;
    initImage(imageStrBase64, ICON_TOO_BIG_SIZE, ICON_TOO_BIG_SIZE);

    TestNotificationParameters parameters("/path/image.png");
    MInfoBanner *infoBanner = m_subject->createInfoBanner(MInfoBanner::Event, 1, parameters);
    QCOMPARE(infoBanner->pixmap().isNull(), true);
    delete infoBanner;
}

void Ut_WidgetNotificationSink::testLoadIconHeightTooBig()
{
    imageReaderCanRead = true;
    initImage(imageStrBase64, ICON_DEFAULT_SIZE, ICON_TOO_BIG_SIZE);

    TestNotificationParameters parameters("/path/image.png");
    MInfoBanner *infoBanner = m_subject->createInfoBanner(MInfoBanner::Event, 1, parameters);
    QCOMPARE(infoBanner->pixmap().isNull(), true);
    delete infoBanner;
}

void Ut_WidgetNotificationSink::testLoadIconWidthTooBig()
{
    imageReaderCanRead = true;
    initImage(imageStrBase64, ICON_TOO_BIG_SIZE, ICON_DEFAULT_SIZE);

    TestNotificationParameters parameters("/path/image.png");
    MInfoBanner *infoBanner = m_subject->createInfoBanner(MInfoBanner::Event, 1, parameters);
    QCOMPARE(infoBanner->pixmap().isNull(), true);
    delete infoBanner;
}

void Ut_WidgetNotificationSink::testLoadIconBrokenImage()
{
    imageReaderCanRead = true;
    initImage(badImageStrBase64, ICON_DEFAULT_SIZE, ICON_DEFAULT_SIZE);

    TestNotificationParameters parameters("/path/image.png");
    MInfoBanner *infoBanner = m_subject->createInfoBanner(MInfoBanner::Event, 1, parameters);
    QCOMPARE(infoBanner->pixmap().isNull(), true);
    delete infoBanner;
}

void Ut_WidgetNotificationSink::testLoadIconCannotReadImage()
{
    imageReaderCanRead = false;
    initImage(imageStrBase64, ICON_DEFAULT_SIZE, ICON_DEFAULT_SIZE);

    TestNotificationParameters parameters("/path/image.png");
    MInfoBanner *infoBanner = m_subject->createInfoBanner(MInfoBanner::Event, 1, parameters);
    QCOMPARE(infoBanner->pixmap().isNull(), true);
    delete infoBanner;
}

QTEST_APPLESS_MAIN(Ut_WidgetNotificationSink)
