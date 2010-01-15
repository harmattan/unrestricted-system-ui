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

ProfileWidget::ProfileWidget(QGraphicsWidget *parent) :
        DcpWidget(parent),
        profileButtons(NULL),
        profileIf(NULL)
{
    setReferer(DcpProfile::None);
    initWidget();
}

ProfileWidget::~ProfileWidget()
{
    delete profileIf;
    profileIf = NULL;
}

void ProfileWidget::initWidget()
{
    //create dbus if
    profileIf = new ProfileDataInterface();

    // catch profile If actions
    connect(profileIf, SIGNAL(currentProfile(int)), this, SLOT(setProfile(int)));
    connect(profileIf, SIGNAL(volumeLevel(int, int)), this, SLOT(setVolume(int, int)));
    connect(profileIf, SIGNAL(vibrationValue(int, bool)), this, SLOT(setVibration(int, bool)));

    profileButtons = new ProfileButtons();

    // get init values
    initProfiles();
}

void ProfileWidget::initProfiles()
{
    qDebug() << Q_FUNC_INFO << "------------------------";

    QMap<int, QString> map;
    QList<ProfileDataInterface::ProfileData> l = profileIf->getProfilesData();
    qDebug() << Q_FUNC_INFO << "received count:" << l.count();

    // create profile containers
    for (int i = 0; i < l.count(); ++i) {
        ProfileDataInterface::ProfileData d = l.at(i);
        ProfileContainer* cont = new ProfileContainer(
            d.profileId,
            d.profileName,
            d.volumeLevel,
            d.vibrationEnabled);
        connect(cont, SIGNAL(sliderValueChanged(int)), this, SLOT(sliderValueChanged(int)));
        connect(cont, SIGNAL(vibrationChanged(bool)), this, SLOT(vibrationChanged(bool)));
        containers.insert(d.profileId, cont);

        map.insert(d.profileId, d.profileName);
    }

    profileButtons->init(map, profileIf->getCurrentProfile());
    connect(profileButtons, SIGNAL(profileSelected(int)), this, SLOT(profileSelected(int)));

    DuiContainer *contentContainer = createContainer();

    /*
     * mainLayout
     */
    QGraphicsLinearLayout *mainLayout = new QGraphicsLinearLayout();
    mainLayout->addItem(contentContainer);
    this->setLayout(mainLayout);
}

DuiContainer* ProfileWidget::createContainer()
{
    DuiLayout *layout = new DuiLayout();

    //% "Current profile"
    DuiLabel* currentHeader = new DuiLabel (qtTrId ("qtn_prof_currprof"));
    //% "Profile Settings"
    DuiLabel* settingsHeader = new DuiLabel (qtTrId ("qtn_prof_settings"));

    DuiLinearLayoutPolicy *portraitPolicy = new DuiLinearLayoutPolicy(layout, Qt::Vertical);
    portraitPolicy->addItem(currentHeader, Qt::AlignLeft);
    portraitPolicy->addItem(profileButtons, Qt::AlignCenter);
    portraitPolicy->addItem(settingsHeader, Qt::AlignLeft);

    DuiGridLayoutPolicy *landscapePolicy = new DuiGridLayoutPolicy(layout);
    landscapePolicy->addItemAtPosition(currentHeader, 0, 0, 1, 2);
    landscapePolicy->addItemAtPosition(profileButtons, 1, 0, 1, 2, Qt::AlignCenter);
    landscapePolicy->addItemAtPosition(settingsHeader, 2, 0, 1, 2);

    int row = 3;
    int col = 0;
    for (int i = 0; i < containers.count(); ++i) {
        qDebug() << Q_FUNC_INFO << "row:" << row << "col:" << col;
        ProfileContainer* cont = containers.value(i);
        portraitPolicy->addItem(cont);
        landscapePolicy->addItemAtPosition(cont, row, col);
        ++col;
        if (1 < col) {
            ++row;
            col = 0;
        }
    }

    layout->setLandscapePolicy(landscapePolicy); // ownership transferred
    layout->setPortraitPolicy(portraitPolicy); // ownership transferred

    DuiContainer *container = new DuiContainer();
    container->centralWidget()->setLayout(layout);

    return container;
}

void ProfileWidget::sliderValueChanged(int index)
{
    ProfileContainer* profile = static_cast<ProfileContainer*>(this->sender());
    qDebug() << Q_FUNC_INFO << "for" << profile->title() << ":" << index;
    profileIf->setVolumeLevel(profile->id(), index);
}

void ProfileWidget::vibrationChanged(bool enabled)
{
    //NOTE: DuiButton->isChecked() method returns the state before the press at this point
    ProfileContainer* profile = static_cast<ProfileContainer*>(this->sender());
    qDebug() << Q_FUNC_INFO << "for" << profile->title() << ":" << enabled;
    profileIf->setVibration(profile->id(), enabled);
}

QString ProfileWidget::currentProfile()
{
    if (profileButtons) {
        return profileButtons->selectedProfileName();
    }
    return "";
}

void ProfileWidget::profileSelected(int id)
{
    qDebug() << Q_FUNC_INFO << ":" << id;
    profileIf->setProfile(id);
}

void ProfileWidget::setProfile(int profileId)
{
    qDebug() << Q_FUNC_INFO << ":" << profileId;
    profileButtons->selectProfile(profileId);
}

void ProfileWidget::setVolume(int profileId, int level)
{
    qDebug() << Q_FUNC_INFO << "for profile" << profileId << ":" << level;
    ProfileContainer *cont = containers.value(profileId);
    if (cont) {
        cont->setLevel(level);
    }
}

void ProfileWidget::setVibration(int profileId, bool enabled)
{
    qDebug() << Q_FUNC_INFO << "for profile" << profileId << ":" << enabled;
    ProfileContainer *cont = containers.value(profileId);
    if (cont) {
        cont->setVibration(enabled);
    }
}
