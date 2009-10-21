#include "profilecontainer.h"
#include "profilebuttons.h"
#include "profiletranslation.h"

#include <DuiButton>
#include <DuiSlider>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiImage>
#include <DuiGridLayoutPolicy>
#include <DuiStylableWidget>
#include <QDebug>

namespace ProfileApplet{
    const int sliderMin = 0;
    const int sliderMax = 100;
}

ProfileContainer::ProfileContainer(int id, const QString &title, DuiWidget *parent) :
    DuiContainer(title, parent),
    slider(NULL),
    level(-2),
    profileId(id)
{
    qDebug() << "ProfileContainer::ProfileContainer()" << title;

    // TODO: hardcoded silent value!!
    if(profileId != 1) {
        slider = new DuiSlider(this, "continuous");
        slider->setOrientation(Qt::Horizontal);
        slider->setRange(ProfileApplet::sliderMin, ProfileApplet::sliderMax);
        slider->setThumbLabelVisible(false);
        connect(slider, SIGNAL(valueChanged(int)), this, SIGNAL(sliderValueChanged(int)));
    }

    button = new DuiButton(DcpProfile::VibrationText, this);
    button->setCheckable(true);
    button->setMaximumHeight(36);
    setVibration(true);
    connect(button, SIGNAL(toggled(bool)), this, SIGNAL(vibrationChanged(bool)));

    setLayout();
}

ProfileContainer::~ProfileContainer()
{
}

void ProfileContainer::setLayout()
{
    DuiLayout *layout = new DuiLayout();

    DuiGridLayoutPolicy *landscapePolicy = new DuiGridLayoutPolicy(layout);
    //landscapePolicy->setObjectName("profileContainerLayout.Landscape");
    layout->setLandscapePolicy(landscapePolicy); // ownership transferred

    DuiGridLayoutPolicy *portraitPolicy = new DuiGridLayoutPolicy(layout);
    //portraitPolicy->setObjectName("profileContainerLayout.Portrait");
    layout->setPortraitPolicy(portraitPolicy); // ownership transferred


    DuiImage* img = NULL;
    QGraphicsLayoutItem* item = NULL;

    if(slider)
    {
        img = new DuiImage("icon-m-volume", this);
        item = slider;
        qDebug() << "slider height" << slider->size().height();
        slider->setMaximumHeight(32);
    }
    else
    {
        img = new DuiImage("icon-m-volume-off", this);
        item = new DuiLabel(DcpProfile::NoRingingText, this);
    }
    img->setObjectName("speakerIcon");

    landscapePolicy->setColumnFixedWidth(0, img->imageSize().width());
    portraitPolicy->setColumnFixedWidth(0, img->imageSize().width());
    landscapePolicy->setRowFixedHeight(0, img->imageSize().height());
    portraitPolicy->setRowFixedHeight(0, img->imageSize().height());

    // set objects
    landscapePolicy->addItemAtPosition(img, 0, 0);
    portraitPolicy->addItemAtPosition(img, 0, 0);
    landscapePolicy->addItemAtPosition(item, 0, 1);
    portraitPolicy->addItemAtPosition(item, 0, 1);
    landscapePolicy->addItemAtPosition(button, 1, 0, 1, 2);
    portraitPolicy->addItemAtPosition(button, 1, 0, 1, 2);

    landscapePolicy->setSpacing(5);
    portraitPolicy->setSpacing(5);

    DuiStylableWidget *layoutWidget = new DuiStylableWidget();
    layoutWidget->setLayout(layout);
    layoutWidget->setObjectName("profileContainerLayout");

    setCentralWidget(layoutWidget);
}

void ProfileContainer::setLevel(int value)
{
    qDebug() << "ProfileContainer::setLevel for " << title() << ":" << value << "(old:" << level << ")";
    if(!slider || value == level)
        return;

    level = value;
    if(ProfileApplet::sliderMin < level && ProfileApplet::sliderMax >= level)
    {
        slider->setValue(level);
        qDebug() << "ProfileContainer::setLevel done:" << slider->value();
    }
    qDebug() << "ProfileContainer::setLevel done:" << slider->value();
}

void ProfileContainer::setVibration(bool enabled)
{
    qDebug() << "ProfileContainer::setVibration for " << title() << ":" << enabled;
    button->setChecked(enabled);
}

int ProfileContainer::id()
{
    return profileId;
}
