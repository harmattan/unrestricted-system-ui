#include "profilecontainer.h"
#include "profiletranslation.h"

#include <DuiButton>
#include <DuiSlider>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiImage>
#include <DuiGridLayoutPolicy>
#include <DuiStylableWidget>
#include <QDebug>

ProfileContainer::ProfileContainer(const QString &title, const QStringList& volumeLevels, int levelIndex, bool vibrationEnabled, DuiWidget *parent) :
    DuiContainer(title, parent),
    volumeLevels(volumeLevels),
    slider(NULL),
    levelIndex(-2)
{
    qDebug() << "ProfileContainer::ProfileContainer()" << title;

    slider = new DuiSlider(this, "continuous");
    slider->setOrientation(Qt::Horizontal);
    setLevel(levelIndex);
    connect(slider, SIGNAL(valueChanged(int)), this, SIGNAL(sliderValueChanged(int)));

    button = new DuiButton(DcpProfile::VibrationText, this);
    button->setCheckable(true);
    setVibration(vibrationEnabled);
    connect(button, SIGNAL(toggled(bool)), this, SIGNAL(vibrationChanged(bool)));
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

    if(slider->isVisible())
    {
        img = new DuiImage("Icon-back", this);
        item = slider;
    }
    else
    {
        img = new DuiImage("Icon-Qwerty-Delete", this);
        item = new DuiLabel(DcpProfile::NoRingingText, this);
    }
    img->setObjectName("speakerIcon");

    landscapePolicy->setColumnFixedWidth(0, img->imageSize().width());
    portraitPolicy->setColumnFixedWidth(0, img->imageSize().width());
    landscapePolicy->setRowFixedHeight(0, img->imageSize().height());
    portraitPolicy->setRowFixedHeight(0, img->imageSize().height());

    qDebug() << "slider height" << slider->size().height();
    slider->setMaximumHeight(32);

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

void ProfileContainer::setLevel(int index)
{
    qDebug() << "ProfileContainer::setLevel" << index;
    if(index == levelIndex)
        return;

    levelIndex = index;
    if(levelIndex >= 0)
    {
        int level;
        int max = volumeLevels.length();
        if( max > 0)
        {
            slider->setRange(0, max-1);
            if(levelIndex < max)
                level = volumeLevels[levelIndex].toInt();
        }
        slider->setValue(level);
        slider->setThumbLabelVisible(false);
    }
    else
    {
        slider->hide();
    }

}

void ProfileContainer::setVibration(bool enabled)
{
    button->setChecked(enabled);
}
