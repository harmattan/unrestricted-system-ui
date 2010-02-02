/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "profilewidget.h"
#include "profilecontainer.h"
#include "dcpprofile.h"
#include "profilebuttons.h"

#include <QDebug>
#include <QGraphicsLinearLayout>

#include <DuiButton>
#include <DuiContainer>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiGridLayoutPolicy>
#include <DuiLinearLayoutPolicy>
#include <QDebug>

#define DEBUG
#include "../debug.h"

ProfileWidget::ProfileWidget(QGraphicsWidget *parent) :
    DcpWidget (parent),
    m_ProfileButtons (0),
    m_ProfileIf (0)
{
    SYS_DEBUG ("");
    initWidget ();
}

ProfileWidget::~ProfileWidget ()
{
    delete m_ProfileIf;
    m_ProfileIf = NULL;
}


void 
ProfileWidget::initWidget ()
{
    //create dbus if
    m_ProfileIf = new ProfileDataInterface();

    // catch profile If actions
    connect (m_ProfileIf, SIGNAL(currentProfile(int)), 
            this, SLOT(setProfile(int)));
    connect (m_ProfileIf, SIGNAL(volumeLevel(int, int)), 
            this, SLOT(setVolume(int, int)));
    connect (m_ProfileIf, SIGNAL(vibrationValue(int, bool)), 
            this, SLOT(setVibration(int, bool)));

    m_ProfileButtons = new ProfileButtons ();

    // get init values
    initProfiles ();
}

void 
ProfileWidget::initProfiles ()
{
    QMap<int, QString> map;
    QList<ProfileDataInterface::ProfileData> l = m_ProfileIf->getProfilesData();
    
    SYS_DEBUG ("We have %d profiles.", l.count());

    // create profile containers
    for (int i = 0; i < l.count(); ++i) {
        ProfileDataInterface::ProfileData d = l.at(i);
        ProfileContainer* cont = new ProfileContainer(
            d.profileId,
            d.profileName,
            d.volumeLevel,
            d.vibrationEnabled);
        connect (cont, SIGNAL(sliderValueChanged(int)), 
                this, SLOT(sliderValueChanged(int)));
        connect (cont, SIGNAL(vibrationChanged(bool)), 
                this, SLOT(vibrationChanged(bool)));
        m_Containers.insert(d.profileId, cont);

        map.insert(d.profileId, d.profileName);
    }

    m_ProfileButtons->init (map, m_ProfileIf->getCurrentProfile());
    connect (m_ProfileButtons, SIGNAL(profileSelected(int)), 
            this, SLOT(profileSelected(int)));

    DuiContainer *contentContainer = createContainer();

    /*
     * mainLayout
     */
    QGraphicsLinearLayout *mainLayout = new QGraphicsLinearLayout();
    mainLayout->addItem (contentContainer);
    this->setLayout (mainLayout);
}

/*!
 * FIXME: The UI spec has been changed, the portrait and the landscape policy is
 * the same. The code is not changed yet, I'm afraid the UI spec might be
 * changed back...
 */
DuiContainer * 
ProfileWidget::createContainer ()
{
    DuiLayout *layout = new DuiLayout();

    //% "Current profile"
    DuiLabel* currentHeader = new DuiLabel (qtTrId ("qtn_prof_currprof"));
    //% "Profile Settings"
    DuiLabel* settingsHeader = new DuiLabel (qtTrId ("qtn_prof_settings"));

    DuiLinearLayoutPolicy *portraitPolicy = 
        new DuiLinearLayoutPolicy(layout, Qt::Vertical);
    portraitPolicy->addItem (currentHeader, Qt::AlignLeft);
    portraitPolicy->addItem (m_ProfileButtons, Qt::AlignCenter);
    portraitPolicy->addItem (settingsHeader, Qt::AlignLeft);

    DuiGridLayoutPolicy *landscapePolicy = new DuiGridLayoutPolicy (layout);
    landscapePolicy->addItem(currentHeader, 0, 0, 1, 2);
    landscapePolicy->addItem(m_ProfileButtons, 1, 0, 1, 2, Qt::AlignCenter);
    landscapePolicy->addItem(settingsHeader, 2, 0, 1, 2);

    int row = 3;
    int col = 0;
    for (int i = 0; i < m_Containers.count(); ++i) {
        qDebug() << Q_FUNC_INFO << "row:" << row << "col:" << col;
        ProfileContainer* cont = m_Containers.value(i);
        portraitPolicy->addItem(cont);
        landscapePolicy->addItem(cont, row, col);
        ++col;
        if (1 < col) {
            ++row;
            col = 0;
        }
    }

    //layout->setLandscapePolicy (landscapePolicy);
    //layout->setPortraitPolicy (portraitPolicy);
    layout->setPolicy (portraitPolicy);

    DuiContainer *container = new DuiContainer ();
    container->centralWidget()->setLayout (layout);

    return container;
}

void 
ProfileWidget::sliderValueChanged (
        int index)
{
    ProfileContainer* profile = static_cast<ProfileContainer*>(this->sender());
    qDebug() << Q_FUNC_INFO << "for" << profile->title() << ":" << index;

    m_ProfileIf->setVolumeLevel (profile->id(), index);
}

void 
ProfileWidget::vibrationChanged (
        bool enabled)
{
    //NOTE: DuiButton->isChecked() method returns the state before the 
    // press at this point
    ProfileContainer* profile = static_cast<ProfileContainer*>(this->sender());
    qDebug() << Q_FUNC_INFO << "for" << profile->title() << ":" << enabled;

    m_ProfileIf->setVibration (profile->id(), enabled);
}

QString 
ProfileWidget::currentProfile ()
{
    /*
     * FIXME: So it is the current profile or the string that the UI thinks is
     * the current profile.
     */
    if (m_ProfileButtons) {
        return m_ProfileButtons->selectedProfileName ();
    }

    return "";
}

void 
ProfileWidget::profileSelected (
        int id)
{
    qDebug() << Q_FUNC_INFO << ":" << id;

    m_ProfileIf->setProfile (id);
}

void 
ProfileWidget::setProfile (
        int profileId)
{
    qDebug() << Q_FUNC_INFO << ":" << profileId;

    m_ProfileButtons->selectProfile (profileId);
}

void 
ProfileWidget::setVolume (
        int profileId, 
        int level)
{
    qDebug() << Q_FUNC_INFO << "for profile" << profileId << ":" << level;

    ProfileContainer *cont = m_Containers.value (profileId);
    if (cont) {
        cont->setLevel(level);
    }
}

void 
ProfileWidget::setVibration (
        int profileId, 
        bool enabled)
{
    qDebug() << Q_FUNC_INFO << "for profile" << profileId << ":" << enabled;

    ProfileContainer *cont = m_Containers.value(profileId);
    if (cont) {
        cont->setVibration (enabled);
    }
}

