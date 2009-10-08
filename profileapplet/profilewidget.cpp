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
    profileIf = new profileDBusInterface();

    // catch profile If actions

    ////// tmp!!!!
    QList<QString> l;
    l.append(QString("1"));
    l.append(QString("5"));
    l.append(QString("10"));
    QStringList volumeLevels(l);

    ProfileButtons::ProfileId defProfile = ProfileButtons::ringing;
    ////// end of tmp


    profileButtons = new ProfileButtons(defProfile, this);
    connect(profileButtons, SIGNAL(profileSelected(ProfileButtons::ProfileId)),
            this, SLOT(profileSelected(ProfileButtons::ProfileId)));

    // create profile containers
    ringingContainer = new ProfileContainer(DcpProfile::RingingText, volumeLevels, 0, true, this);
    connect(ringingContainer, SIGNAL(sliderValueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(ringingContainer, SIGNAL(vibrationChanged(bool)), this, SLOT(vibrationChanged(bool)));

    loudContainer = new ProfileContainer(DcpProfile::LoudText, volumeLevels, 1, true, this);
    connect(loudContainer, SIGNAL(sliderValueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(loudContainer, SIGNAL(vibrationChanged(bool)), this, SLOT(vibrationChanged(bool)));

    beepContainer = new ProfileContainer(DcpProfile::BeepText, volumeLevels, 2, true, this);
    connect(beepContainer, SIGNAL(sliderValueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(beepContainer, SIGNAL(vibrationChanged(bool)), this, SLOT(vibrationChanged(bool)));

    silentContainer = new ProfileContainer(DcpProfile::SilentText, volumeLevels, -1, false, this);
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
    layoutWidget->setObjectName("kissa");

    DuiContainer *container = new DuiContainer(DcpProfile::SettingsHeaderText, this);
    container->setCentralWidget(layoutWidget);

    return container;
}

void ProfileWidget::initSlider(DuiSlider* /*slider*/, int /*index*/, const QStringList& /*values*/)
{
/*    slider->setOrientation(Qt::Horizontal);
    slider->setRange(0,values.size()-1);
    slider->setValue(index);

    QString pattern = (slider == screenLightsSlider ? DcpProfile::ScreenLightsValueText : QString(""));
    updateSliderThumbLabel(slider, values.at(index), pattern);
*/}

void ProfileWidget::updateSliderThumbLabel(DuiSlider *slider, const QString &value, const QString &pattern)
{    
    slider->setThumbLabel((pattern.isEmpty() ? value : pattern.arg(value)));
}

void ProfileWidget::sliderValueChanged(int index)
{      
    ProfileContainer* profile = static_cast<ProfileContainer*>(this->sender());
    qDebug() << "ProfileWidget::sliderValueChanged() for" << profile->title() << ":" << index;
/*
    if(profile == silentContainer) {
        profileIf->setBrightnessValue(brightnessValues.at(index));
        updateSliderThumbLabel(slider, brightnessValues.at(index));
    }
    else {
        profileIf->setScreenLightsValue(screenLightsValues.at(index));
        updateSliderThumbLabel(slider, screenLightsValues.at(index), DcpProfile::ScreenLightsValueText);
    }
*/}

void ProfileWidget::vibrationChanged(bool on)
{
    //NOTE: DuiButton->isChecked() method returns the state before the press at this point
    ProfileContainer* profile = static_cast<ProfileContainer*>(this->sender());
    qDebug() << "ProfileWidget::vibrationChanged() for" << profile->title() << ":" << on;
}

QString ProfileWidget::currentProfile()
{
    return "[TODO: Current profile name]";
}

void ProfileWidget::profileSelected(int id)
{
    qDebug() << "ProfileWidget::profileSelected():" << id;
    switch (id)
    {
        case ProfileButtons::ringing:
            break;
        case ProfileButtons::silent:
            break;
        case ProfileButtons::beep:
            break;
        case ProfileButtons::loud:
            break;
    }
}

void ProfileWidget::orientationChanged(const Dui::Orientation &orientation)
{
    qDebug() << "ProfileWidget::orientationChanged() to" << orientation;
}
