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
#include "testnotificationparameters.h"
#include "genericnotificationparameterfactory.h"
#include <MApplication>
#include <MLocale>
#include <MGConfItem>

// List of event type files
QStringList eventTypeFilesList;

// QPixmap stubs
bool gPixmapLoaded;
bool QPixmap::load(const QString &fileName, const char *format, Qt::ImageConversionFlags flags) {
    Q_UNUSED(format);
    Q_UNUSED(flags);
    if (!fileName.isEmpty()) {
        gPixmapLoaded = true;
    } else {
        gPixmapLoaded = false;
    }
    return gPixmapLoaded;
}

bool QPixmap::isNull() const {
    return false;
}

// MBanner stubs
bool gPixmapSet;
void MBanner::setPixmap(const QPixmap &pixmap) {
    Q_UNUSED(pixmap);
    if (gPixmapLoaded) {
        gPixmapSet = true;
    }
}

class TestWidgetNotificationSink : public WidgetNotificationSink
{
public:
    MBanner *createInfoBanner(const Notification &);
    MBanner *createInfoBanner(Notification::NotificationType, uint groupId, const NotificationParameters &);
    void addNotification(const Notification &);
    void removeNotification(uint);
    void updateImage(MBanner *infoBanner, const NotificationParameters &);
    void updateActions(MBanner *infoBanner, const Notification &notification);
};

MBanner *TestWidgetNotificationSink::createInfoBanner(const Notification &n)
{
    return WidgetNotificationSink::createInfoBanner(n);
}

MBanner *TestWidgetNotificationSink::createInfoBanner(Notification::NotificationType type, uint groupId, const NotificationParameters &parameters)
{
    return WidgetNotificationSink::createInfoBanner(type, groupId, parameters);
}

void TestWidgetNotificationSink::addNotification(const Notification &)
{
}

void TestWidgetNotificationSink::removeNotification(uint)
{
}

void TestWidgetNotificationSink::updateImage(MBanner *infoBanner, const NotificationParameters &params)
{
    WidgetNotificationSink::updateImage(infoBanner, params);
}

void TestWidgetNotificationSink::updateActions(MBanner *infoBanner, const Notification &notification)
{
    WidgetNotificationSink::updateActions(infoBanner, notification.parameters());
}

QString qtTrId(const char *id, int count)
{
    if(QString(id) == "translationid") {
        if (1 == count) {
            return "translatedstring %1";
        } else if (count > 1) {
            return "translatedpluralstring %1";
        }
    }
    return QString();
}

// MLocale stubs
QString gInstalledTrCatalog;
MLocale *gInstalledCatalogLocale;
void MLocale::installTrCatalog(const QString &name)
{
    gInstalledCatalogLocale = this;
    gInstalledTrCatalog = name;
}

MLocale *gSetDefaultLocale;
void MLocale::setDefault(const MLocale &locale)
{
    gSetDefaultLocale = const_cast<MLocale*>(&locale);
}

// MGConfItem stubs
bool gMGConfPrivateNotificationValue;
QVariant MGConfItem::value() const
{
    if(this->key() == "/desktop/meego/privacy/private_lockscreen_notifications") {
        return QVariant(gMGConfPrivateNotificationValue);
    }

    return QVariant();
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

QList<QString> Ut_WidgetNotificationSink::contents;
QHash<const QGraphicsWidget *, QList<QAction *> > Ut_WidgetNotificationSink::actions;
int Ut_WidgetNotificationSink::actionTriggeredCount;

void Ut_WidgetNotificationSink::initTestCase()
{
    // Create a MAapplication
    static int argc = 1;
    static char *app_name = (char *)"./ut_widgetnotificationsink";
    app = new MApplication(argc, &app_name);
}

void Ut_WidgetNotificationSink::cleanupTestCase()
{
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
    gPixmapSet = false;
    gInstalledTrCatalog = "";
    gInstalledCatalogLocale = NULL;
    gSetDefaultLocale = NULL;
    gMGConfPrivateNotificationValue = false;
}

void Ut_WidgetNotificationSink::cleanup()
{
    delete m_subject;
    eventTypeFilesList.clear();
}

void Ut_WidgetNotificationSink::testUpdateActions()
{
    // Create notification parameters
    Notification notification;
    TestNotificationParameters parameters;
    parameters.add(NotificationWidgetParameterFactory::createActionParameter("content0 0 0 0"));
    notification.setParameters(parameters);

    // Create an info banner with one action
    MBanner infoBanner;
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
    QScopedPointer<MBanner> infoBanner(m_subject->createInfoBanner(Notification(notificationID, 0, 1, parameters, Notification::ApplicationEvent, 1000)));

    // Listen to triggered signals of info banner action
    QCOMPARE(actions.count(), 1);
    MRemoteAction *action = dynamic_cast<MRemoteAction *>(actions[infoBanner.data()].at(0));
    QVERIFY(action != NULL);

    // Listen to notification removal requests of WidgetNotificationSink
    QSignalSpy notificationRemovalRequested(m_subject, SIGNAL(notificationRemovalRequested(uint)));

    // Click the infoBanner
    connect(this, SIGNAL(click()), infoBanner.data(), SIGNAL(clicked()));
    emit click();
    disconnect(this, SIGNAL(click()), infoBanner.data(), SIGNAL(clicked()));
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
    infoBanner.reset(m_subject->createInfoBanner(Notification::ApplicationEvent, groupID, groupParameters));

    // Listen to triggered signals of info banner action
    QCOMPARE(actions.count(), 2);
    action = dynamic_cast<MRemoteAction *>(actions[infoBanner.data()].at(0));
    QVERIFY(action != NULL);

    // Listen to notification removal requests of WidgetNotificationSink
    QSignalSpy notificationGroupClearingRequested(m_subject, SIGNAL(notificationGroupClearingRequested(uint)));

    // Click the infoBanner
    connect(this, SIGNAL(click()), infoBanner.data(), SIGNAL(clicked()));
    emit click();
    disconnect(this, SIGNAL(click()), infoBanner.data(), SIGNAL(clicked()));
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
    QScopedPointer<MBanner> infoBanner(m_subject->createInfoBanner(Notification(notificationID, 0, 1, parameters, Notification::ApplicationEvent, 1000)));

    // Listen to triggered signals of info banner action
    QCOMPARE(actions.count(), 1);
    MRemoteAction *action = dynamic_cast<MRemoteAction *>(actions[infoBanner.data()].at(0));
    QVERIFY(action != NULL);

    // Listen to notification removal requests of WidgetNotificationSink
    QSignalSpy notificationRemovalRequested(m_subject, SIGNAL(notificationRemovalRequested(uint)));

    // Click the infoBanner
    connect(this, SIGNAL(click()), infoBanner.data(), SIGNAL(clicked()));
    emit click();
    disconnect(this, SIGNAL(click()), infoBanner.data(), SIGNAL(clicked()));
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

void Ut_WidgetNotificationSink::testInfoBannerCreationWithRemoteAction()
{
    TestNotificationParameters parameters("title0", "subtitle0", "buttonicon0", "content0 0 0 0");

    QScopedPointer<MBanner> infoBanner(m_subject->createInfoBanner(Notification(3, 1, 0, parameters, Notification::ApplicationEvent, 1020)));
    QCOMPARE(infoBanner->objectName(), QString("EventBanner"));
    QCOMPARE(infoBanner->title(), QString("title0"));
    QCOMPARE(infoBanner->subtitle(), QString("subtitle0"));
    QCOMPARE(infoBanner->iconID(), QString("buttonicon0"));
    QCOMPARE(infoBanner->actions().count(), 1);
    MRemoteAction *remoteAction = dynamic_cast<MRemoteAction *>(infoBanner->actions().at(0));
    QVERIFY(remoteAction != NULL);
    QCOMPARE(remoteAction->toString(), QString("content0 0 0 0"));
}

void Ut_WidgetNotificationSink::testInfoBannerCreationWithoutRemoteAction()
{
    TestNotificationParameters parameters("title1", "subtitle1", "buttonicon1");
    QScopedPointer<MBanner> infoBanner(m_subject->createInfoBanner(Notification(3, 1, 0, parameters, Notification::ApplicationEvent, 1020)));
    QCOMPARE(infoBanner->objectName(), QString("EventBanner"));
    QCOMPARE(infoBanner->title(), QString("title1"));
    QCOMPARE(infoBanner->subtitle(), QString("subtitle1"));
    QCOMPARE(infoBanner->iconID(), QString("buttonicon1"));
    QCOMPARE(infoBanner->actions().count(), 0);
}

void Ut_WidgetNotificationSink::testInfoBannerCreationWithSystemEvent()
{
    TestNotificationParameters parameters("title1", "subtitle1", "buttonicon1");
    //Adding a timestamp to parameters to test that it doesnt get added to SystemBanners
    uint timestamp = QDateTime::currentDateTime().toTime_t();
    parameters.add("timestamp", timestamp);
    QScopedPointer<MBanner> infoBanner(m_subject->createInfoBanner(Notification(3, 1, 0, parameters, Notification::SystemEvent, 1020)));
    QCOMPARE(infoBanner->objectName(), QString("SystemBanner"));
    // Check that title is not set for system event
    QCOMPARE(infoBanner->title(), QString("subtitle1"));
    QCOMPARE(infoBanner->subtitle(), QString(""));
    QCOMPARE(infoBanner->iconID(), QString("buttonicon1"));
    QCOMPARE(infoBanner->actions().count(), 0);
    QVERIFY(infoBanner->bannerTimeStamp().isNull());
}

void Ut_WidgetNotificationSink::testInfoBannerCreationWithNotificationParameters()
{
    TestNotificationParameters parameters("title3", "subtitle3", "buttonicon3", "content1 2 3 4");
    uint timestamp = QDateTime::currentDateTime().toTime_t();
    parameters.add("timestamp", timestamp);
    QScopedPointer<MBanner> infoBanner(m_subject->createInfoBanner(Notification::ApplicationEvent, 1, parameters));
    QCOMPARE(infoBanner->objectName(), QString("EventBanner"));
    QCOMPARE(infoBanner->title(), QString("title3"));
    QCOMPARE(infoBanner->subtitle(), QString("subtitle3"));
    QCOMPARE(infoBanner->iconID(), QString("buttonicon3"));
    QCOMPARE(infoBanner->actions().count(), 1);
    QCOMPARE(infoBanner->bannerTimeStamp().toTime_t(), timestamp);
    MRemoteAction *remoteAction = dynamic_cast<MRemoteAction *>(infoBanner->actions().at(0));
    QVERIFY(remoteAction != NULL);
    QCOMPARE(remoteAction->toString(), QString("content1 2 3 4"));
}

void Ut_WidgetNotificationSink::testInfoBannerCreationWhenPrivacyIsHonoredButNotEnabled()
{
    m_subject->setHonorPrivacySetting(true);
    TestNotificationParameters parameters("title1", "subtitle1", "buttonicon1");
    QScopedPointer<MBanner> infoBanner(m_subject->createInfoBanner(Notification(3, 1, 0, parameters, Notification::ApplicationEvent, 1020)));
    QCOMPARE(infoBanner->objectName(), QString("EventBanner"));
    QCOMPARE(infoBanner->title(), QString("title1"));
    QCOMPARE(infoBanner->subtitle(), QString("subtitle1"));
    QCOMPARE(infoBanner->iconID(), QString("buttonicon1"));
}

void Ut_WidgetNotificationSink::testInfoBannerCreationWhenPrivacyIsHonoredAndEnabledButNoCatalogIdDefined()
{
    m_subject->setHonorPrivacySetting(true);
    gMGConfPrivateNotificationValue = true;
    TestNotificationParameters parameters("title1", "subtitle1", "buttonicon1");
    QScopedPointer<MBanner> infoBanner(m_subject->createInfoBanner(Notification(3, 1, 0, parameters, Notification::ApplicationEvent, 1020)));
    QCOMPARE(infoBanner->objectName(), QString("EventBanner"));
    QCOMPARE(infoBanner->title(), QString());
    QCOMPARE(infoBanner->subtitle(), QString());
    QCOMPARE(infoBanner->iconID(), QString("buttonicon1"));
}

void Ut_WidgetNotificationSink::testInfoBannerCreationWhenPrivacyIsHonoredAndEnabledButNoCatalogNameDefined()
{
    m_subject->setHonorPrivacySetting(true);
    gMGConfPrivateNotificationValue = true;
    TestNotificationParameters parameters("title1", "subtitle1", "buttonicon1");
    parameters.add(NotificationWidgetParameterFactory::genericTextIdKey(), "translationid");
    QScopedPointer<MBanner> infoBanner(m_subject->createInfoBanner(Notification(3, 1, 0, parameters, Notification::ApplicationEvent, 1020)));
    QCOMPARE(infoBanner->objectName(), QString("EventBanner"));
    QCOMPARE(infoBanner->title(), QString());
    QCOMPARE(infoBanner->subtitle(), QString());
    QCOMPARE(infoBanner->iconID(), QString("buttonicon1"));
}

void Ut_WidgetNotificationSink::testInfoBannerCreationWhenPrivacyIsHonoredAndEnabledAndCatalogPresent()
{
    QString catalogue("translationcatalogue");
    m_subject->setHonorPrivacySetting(true);
    gMGConfPrivateNotificationValue = true;
    TestNotificationParameters parameters("title1", "subtitle1", "buttonicon1");
    parameters.add(NotificationWidgetParameterFactory::genericTextIdKey(), "translationid");
    parameters.add(NotificationWidgetParameterFactory::genericTextCatalogueKey(), catalogue);
    parameters.add(GenericNotificationParameterFactory::countKey(), "1");
    QScopedPointer<MBanner> infoBanner(m_subject->createInfoBanner(Notification(3, 1, 1, parameters, Notification::ApplicationEvent, 1020)));
    QCOMPARE(infoBanner->objectName(), QString("EventBanner"));
    QCOMPARE(infoBanner->title(), QString("translatedstring 1"));
    QCOMPARE(infoBanner->subtitle(), QString());
    QCOMPARE(infoBanner->iconID(), QString("buttonicon1"));
    QCOMPARE(gInstalledTrCatalog, catalogue);
    QCOMPARE(gSetDefaultLocale, gInstalledCatalogLocale);
}

void Ut_WidgetNotificationSink::testInfoBannerCreationWhenPrivacyIsHonoredAndEnabledAndCatalogPresentPlural()
{
    QString catalogue("translationcatalogue");
    m_subject->setHonorPrivacySetting(true);
    gMGConfPrivateNotificationValue = true;
    TestNotificationParameters parameters("title1", "subtitle1", "buttonicon1");
    parameters.add(NotificationWidgetParameterFactory::genericTextIdKey(), "translationid");
    parameters.add(NotificationWidgetParameterFactory::genericTextCatalogueKey(), catalogue);
    parameters.add(GenericNotificationParameterFactory::countKey(), "2");
    QScopedPointer<MBanner> infoBanner(m_subject->createInfoBanner(Notification(3, 1, 2, parameters, Notification::ApplicationEvent, 1020)));
    QCOMPARE(infoBanner->objectName(), QString("EventBanner"));
    QCOMPARE(infoBanner->title(), QString("translatedpluralstring 2"));
    QCOMPARE(infoBanner->subtitle(), QString());
    QCOMPARE(infoBanner->iconID(), QString("buttonicon1"));
    QCOMPARE(gInstalledTrCatalog, catalogue);
    QCOMPARE(gSetDefaultLocale, gInstalledCatalogLocale);
}

void Ut_WidgetNotificationSink::testUserRemovablePropertyIsSetWhenBannerIsCreated()
{
    TestNotificationParameters parameters;

    parameters.add(NotificationWidgetParameterFactory::createUserRemovableParameter(true));
    QScopedPointer<MBanner> infoBanner(m_subject->createInfoBanner(Notification::ApplicationEvent, 1, parameters));
    QCOMPARE(infoBanner->property(WidgetNotificationSink::USER_REMOVABLE_PROPERTY).toBool(), true);

    parameters.add(NotificationWidgetParameterFactory::createUserRemovableParameter(false));
    infoBanner.reset(m_subject->createInfoBanner(Notification::ApplicationEvent, 1, parameters));
    QCOMPARE(infoBanner->property(WidgetNotificationSink::USER_REMOVABLE_PROPERTY).toBool(), false);
}

void Ut_WidgetNotificationSink::testWhenNotificationsCreatedThenImageIsSetCorrectly_data()
{
    QTest::addColumn<QString>("iconId");
    QTest::addColumn<QString>("imageId");

    QTest::addColumn<QString>("verifyImageId");
    QTest::addColumn<bool>("verifyPixmap");

    QTest::newRow("Only icon id") << "icon-id" << "" << "icon-id" << false;
    QTest::newRow("Only image id") << "" << "image-id" << "image-id" << false;
    QTest::newRow("Both image and icon id") << "icon-id" << "image-id" << "image-id" << false;
    QTest::newRow("Image as path") <<  "" << "/absolute/path/image.png" << "" << true;
    QTest::newRow("Icon id and image as path") <<  "icon-id" << "/absolute/path/image.png" << "" << true;
}

void Ut_WidgetNotificationSink::testWhenNotificationsCreatedThenImageIsSetCorrectly()
{
    QFETCH(QString, iconId);
    QFETCH(QString, imageId);
    QFETCH(QString, verifyImageId);
    QFETCH(bool, verifyPixmap);

    TestNotificationParameters parameters("", "", iconId);
    parameters.add(NotificationWidgetParameterFactory::createImageIdParameter(imageId));
    QScopedPointer<MBanner> infoBanner(m_subject->createInfoBanner(Notification(3, 1, 0, parameters, Notification::ApplicationEvent, 1020)));
    QCOMPARE(infoBanner->iconID(), verifyImageId);
    QCOMPARE(gPixmapLoaded, verifyPixmap);
    QCOMPARE(gPixmapSet, verifyPixmap);
}

void Ut_WidgetNotificationSink::testPrivacySettingValueEmittedWhenHonoringChanges_data()
{
    QTest::addColumn<bool>("honorPrivacy");
    QTest::addColumn<bool>("privacyEnabled");
    QTest::addColumn<bool>("emittedValue");

    QTest::newRow("Privacy enabled and honored") << true << true << true;
    QTest::newRow("Privacy disabled and honored") << true << false << false;
    QTest::newRow("Privacy enabled but not honored") << false << true << false;
    QTest::newRow("Privacy disabled and not honored") << false << false << false;
}

void Ut_WidgetNotificationSink::testPrivacySettingValueEmittedWhenHonoringChanges()
{
    QFETCH(bool, honorPrivacy);
    QFETCH(bool, privacyEnabled);
    QFETCH(bool, emittedValue);

    // First set the value to the opposite of what is wanted
    m_subject->setHonorPrivacySetting(!honorPrivacy);

    QSignalSpy privacySettingSpy(m_subject, SIGNAL(privacySettingChanged(bool)));
    gMGConfPrivateNotificationValue = privacyEnabled;
    m_subject->setHonorPrivacySetting(honorPrivacy);
    QCOMPARE(privacySettingSpy.count(), 1);
    QCOMPARE(privacySettingSpy.at(0).at(0).toBool(), emittedValue);

    // Setting the same value again should cause no new signal
    m_subject->setHonorPrivacySetting(honorPrivacy);
    QCOMPARE(privacySettingSpy.count(), 1);
}

void Ut_WidgetNotificationSink::testPrivacySettingValueEmittedWhenPrivacySettingChanges()
{
    m_subject->setHonorPrivacySetting(true);
    QVERIFY(disconnect(m_subject->privacySetting, SIGNAL(valueChanged()), m_subject, SLOT(emitPrivacySettingValue())));
}

void Ut_WidgetNotificationSink::testWhenNotificationsCreatedAreNotClickableWhenClickingThemDoesNotWork()
{
    TestNotificationParameters parameters("title1", "subtitle1", "buttonicon1");
    m_subject->setNotificationsClickable(false);
    QScopedPointer<MBanner> infoBanner(m_subject->createInfoBanner(Notification(3, 1, 0, parameters, Notification::ApplicationEvent, 1020)));
    bool ret = disconnect(infoBanner.data(), SIGNAL(clicked()), m_subject, SLOT(infoBannerClicked()));
    QCOMPARE(ret, false);
}

void Ut_WidgetNotificationSink::testNotificationShownOnlyIfItContainsText()
{
    TestNotificationParameters parameters("", "", "");
    bool ret1( m_subject->containsText(Notification(3, 1, 0, parameters, Notification::ApplicationEvent, 1020)) );
    QCOMPARE(false, ret1);
    TestNotificationParameters parameters2("title", "subtitle", "buttonicon1");
    bool ret2( m_subject->containsText(Notification(3, 1, 0, parameters2, Notification::ApplicationEvent, 1020)) );
    QCOMPARE(true, ret2);
}

QTEST_APPLESS_MAIN(Ut_WidgetNotificationSink)
