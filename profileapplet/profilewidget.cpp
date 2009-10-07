#include "profilewidget.h"
#include "profiletranslation.h"
#include "profiledbusinterface.h"
#include "profilecontainer.h"
#include "dcpprofile.h"

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

    // tmp!!!!
    QList<QString> l;
    l.append(QString("1"));
    l.append(QString("5"));
    l.append(QString("10"));
    QStringList volumeLevels(l);

    // create profile containers
    ringingContainer = new ProfileContainer(DcpProfile::RingingText, volumeLevels, 0, true, this);
    connect(ringingContainer, SIGNAL(sliderValueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(ringingContainer, SIGNAL(setVibration(bool)), this, SLOT(vibrationChanged(bool)));

    loudContainer = new ProfileContainer(DcpProfile::LoudText, volumeLevels, 1, true, this);
    connect(loudContainer, SIGNAL(sliderValueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(loudContainer, SIGNAL(setVibration(bool)), this, SLOT(vibrationChanged(bool)));

    beepContainer = new ProfileContainer(DcpProfile::BeepText, volumeLevels, 2, true, this);
    connect(beepContainer, SIGNAL(sliderValueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(beepContainer, SIGNAL(setVibration(bool)), this, SLOT(vibrationChanged(bool)));

    silentContainer = new ProfileContainer(DcpProfile::SilentText, volumeLevels, -1, false, this);
    connect(silentContainer, SIGNAL(setVibration(bool)), this, SLOT(vibrationChanged(bool)));

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

    DuiLabel* header = new DuiLabel(DcpProfile::SettingsHeaderText,this);
    header->setObjectName("kissa");

    DuiLinearLayoutPolicy *portraitPolicy = new DuiLinearLayoutPolicy(layout, Qt::Vertical);
    portraitPolicy->addItem(header, Qt::AlignLeft);
    portraitPolicy->addItem(ringingContainer, Qt::AlignLeft);
    portraitPolicy->addItem(silentContainer, Qt::AlignLeft);
    portraitPolicy->addItem(beepContainer, Qt::AlignLeft);
    portraitPolicy->addItem(loudContainer, Qt::AlignLeft);

    DuiGridLayoutPolicy *landscapePolicy = new DuiGridLayoutPolicy(layout);
    landscapePolicy->addItemAtPosition(header, 0, 0, 1, 2);
    landscapePolicy->addItemAtPosition(ringingContainer, 1,0);
    landscapePolicy->addItemAtPosition(silentContainer, 1,1);
    landscapePolicy->addItemAtPosition(beepContainer, 2,0);
    landscapePolicy->addItemAtPosition(loudContainer, 2,1);

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

void ProfileWidget::orientationChanged(const Dui::Orientation &orientation)
{
    qDebug() << "ProfileWidget::orientationChanged() to" << orientation;
}
