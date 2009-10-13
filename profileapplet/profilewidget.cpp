#include "profilewidget.h"
#include "profiletranslation.h"
#include "profiledbusinterface.h"
#include "profilecontainer.h"
#include "dcpprofile.h"
#include "profilebuttons.h"

#include <QDebug>
#include <QTimer>

#include <DuiButton>
#include <DuiContainer>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiGridLayoutPolicy>
#include <DuiLinearLayoutPolicy>
#include <DuiSceneManager>
#include <DuiSlider>
#include <DuiStylableWidget>
#include <DuiWidgetController>
#include <QDebug>

ProfileWidget::ProfileWidget(QGraphicsWidget *parent)
	    :DcpWidget(parent)
{    
    setReferer(DcpProfile::None);
    initWidget();

    connect(DuiSceneManager::instance(),
        SIGNAL(orientationChanged(const Dui::Orientation &)),
        this, SLOT(orientationChanged(const Dui::Orientation &)));
}

ProfileWidget::~ProfileWidget()
{    
}

void ProfileWidget::initWidget()
{
    //create dbus if
    profileIf = new ProfileDBusInterface();

    // catch profile If actions
    connect(profileIf, SIGNAL(currentProfile(int)), this, SLOT(setProfile(int)));
    connect(profileIf, SIGNAL(volumeLevel(int, int)), this, SLOT(setVolume(int, int)));
    connect(profileIf, SIGNAL(vibrationValue(int, bool)), this, SLOT(setVibration(int, bool)));

    // get init values
    profileIf->currentProfileRequired();
    profileIf->volumeLevelsRequired();
    profileIf->vibrationValuesRequired();

    profileButtons = new ProfileButtons(this);
    connect(profileButtons, SIGNAL(profileSelected(ProfileButtons::ProfileId)),
            this, SLOT(profileSelected(ProfileButtons::ProfileId)));

    // create profile containers
    ringingContainer = new ProfileContainer(ProfileButtons::ringing, DcpProfile::RingingText, this);
    connect(ringingContainer, SIGNAL(sliderValueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(ringingContainer, SIGNAL(vibrationChanged(bool)), this, SLOT(vibrationChanged(bool)));

    loudContainer = new ProfileContainer(ProfileButtons::loud, DcpProfile::LoudText, this);
    connect(loudContainer, SIGNAL(sliderValueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(loudContainer, SIGNAL(vibrationChanged(bool)), this, SLOT(vibrationChanged(bool)));

    beepContainer = new ProfileContainer(ProfileButtons::beep, DcpProfile::BeepText, this);
    connect(beepContainer, SIGNAL(sliderValueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(beepContainer, SIGNAL(vibrationChanged(bool)), this, SLOT(vibrationChanged(bool)));

    silentContainer = new ProfileContainer(ProfileButtons::silent, DcpProfile::SilentText, this);
    connect(silentContainer, SIGNAL(vibrationChanged(bool)), this, SLOT(vibrationChanged(bool)));

    DuiContainer *contentContainer = createContainer();

    /*
     * mainLayout
     */
    DuiLayout *mainLayout = new DuiLayout(this);
    DuiLinearLayoutPolicy *mainLayoutPolicy = new DuiLinearLayoutPolicy(mainLayout, Qt::Vertical);
    mainLayoutPolicy->addItem(contentContainer);   

    this->setLayout(mainLayout);    

}

DuiContainer* ProfileWidget::createContainer()
{
    DuiLayout *layout = new DuiLayout();

    DuiLabel* currentHeader = new DuiLabel(DcpProfile::CurrentText,this);
    DuiLabel* settingsHeader = new DuiLabel(DcpProfile::SettingsHeaderText,this);

    DuiLinearLayoutPolicy *portraitPolicy = new DuiLinearLayoutPolicy(layout, Qt::Vertical);
    portraitPolicy->addItem(currentHeader, Qt::AlignLeft);
    portraitPolicy->addItem(profileButtons, Qt::AlignCenter);
    portraitPolicy->addItem(settingsHeader, Qt::AlignLeft);
    portraitPolicy->addItem(ringingContainer, Qt::AlignLeft);
    portraitPolicy->addItem(silentContainer, Qt::AlignLeft);
    portraitPolicy->addItem(beepContainer, Qt::AlignLeft);
    portraitPolicy->addItem(loudContainer, Qt::AlignLeft);

    DuiGridLayoutPolicy *landscapePolicy = new DuiGridLayoutPolicy(layout);
    landscapePolicy->addItemAtPosition(currentHeader, 0, 0, 1, 2);
    landscapePolicy->addItemAtPosition(profileButtons, 1, 0, 1, 2, Qt::AlignCenter);
    landscapePolicy->addItemAtPosition(settingsHeader, 2, 0, 1, 2);
    landscapePolicy->addItemAtPosition(ringingContainer, 3,0);
    landscapePolicy->addItemAtPosition(silentContainer, 3,1);
    landscapePolicy->addItemAtPosition(beepContainer, 4,0);
    landscapePolicy->addItemAtPosition(loudContainer, 4,1);

    layout->setLandscapePolicy(landscapePolicy); // ownership transferred
    layout->setPortraitPolicy(portraitPolicy); // ownership transferred

    DuiStylableWidget *layoutWidget = new DuiStylableWidget();
    layoutWidget->setLayout(layout);

    DuiContainer *container = new DuiContainer(this);
    container->setCentralWidget(layoutWidget);

    return container;
}

void ProfileWidget::sliderValueChanged(int index)
{      
    ProfileContainer* profile = static_cast<ProfileContainer*>(this->sender());
    qDebug() << "ProfileWidget::sliderValueChanged() for" << profile->title() << ":" << index;
    profileIf->setVolumeLevel(profile->id(), index);
}

void ProfileWidget::vibrationChanged(bool enabled)
{
    //NOTE: DuiButton->isChecked() method returns the state before the press at this point
    ProfileContainer* profile = static_cast<ProfileContainer*>(this->sender());
    qDebug() << "ProfileWidget::vibrationChanged() for" << profile->title() << ":" << enabled;
    profileIf->setVibration(profile->id(), enabled);
}

QString ProfileWidget::currentProfile()
{
    if(profileButtons) {
        return profileButtons->selectedProfileName();
    }
    return "";
}

void ProfileWidget::profileSelected(ProfileButtons::ProfileId id)
{
    qDebug() << "ProfileWidget::profileSelected():" << id;
    profileIf->setProfile(id);
}

void ProfileWidget::orientationChanged(const Dui::Orientation &orientation)
{
    qDebug() << "ProfileWidget::orientationChanged() to" << orientation;
}

void ProfileWidget::setProfile(int profileId)
{
    qDebug() << "ProfileWidget::setProfile():" << profileId;
    profileButtons->selectProfile((ProfileButtons::ProfileId)profileId);
}

void ProfileWidget::setVolume(int profileId, int level)
{
     qDebug() << "ProfileWidget::setVolume() for profile" << profileId << ":" << level;
    switch (profileId)
    {
        case ProfileButtons::ringing:
            ringingContainer->setLevel(level);
            break;
        case ProfileButtons::beep:
            beepContainer->setLevel(level);
            break;
        case ProfileButtons::loud:
            loudContainer->setLevel(level);
            break;
        case ProfileButtons::silent:
        case ProfileButtons::none:
        default:
            break;
    }
}

void ProfileWidget::setVibration(int profileId, bool enabled)
{
   qDebug() << "ProfileWidget::setVibration() for profile" << profileId << ":" << enabled;
   switch (profileId)
    {
        case ProfileButtons::ringing:
            ringingContainer->setVibration(enabled);
            break;
        case ProfileButtons::silent:
            silentContainer->setVibration(enabled);
            break;
        case ProfileButtons::beep:
            beepContainer->setVibration(enabled);
            break;
        case ProfileButtons::loud:
            loudContainer->setVibration(enabled);
            break;
        case ProfileButtons::none:
        default:
            break;
    }

}

