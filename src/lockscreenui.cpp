#include "lockscreenui.h"

#include "unlockslider.h"

#include <DuiLabel>
#include <DuiImage>
#include <DuiButton>
#include <DuiLayout>
#include <DuiGridLayoutPolicy>
#include <DuiSceneManager>
#include <DuiSlider>

LockScreenUI::LockScreenUI()
{
    setFullscreen(true);
    setNavigationBarVisible(false);
    createContent();
}

LockScreenUI::~LockScreenUI()
{
}

void LockScreenUI::createContent()
{
    DuiApplicationPage::createContent();

    DuiLayout *layout = new DuiLayout();
    centralWidget()->setLayout(layout);

    l_policy = new DuiGridLayoutPolicy(layout);
    l_policy->setContentsMargins(32, 32, 32, 32);
    p_policy = new DuiGridLayoutPolicy(layout);
    p_policy->setContentsMargins(32, 32, 32, 32);

    slider = new UnlockSlider(this, "continuous");
    slider->setOrientation(Qt::Horizontal);
    slider->setMaximumHeight(70);
    slider->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));
    slider->setVisible(true);

    l_policy->addItemAtPosition(slider, 3, 0);
    p_policy->addItemAtPosition(slider, 3, 0, 1, 2);

    orientationChanged(DuiSceneManager::instance()->orientation());

    connect(slider, SIGNAL(sliderPressed()), this, SLOT(buttonPressed()));
    connect(slider, SIGNAL(sliderReleased()), this, SLOT(buttonReleased()));

    connect(DuiSceneManager::instance(),
        SIGNAL(orientationChanged(const Dui::Orientation &)),
        this, SLOT(orientationChanged(const Dui::Orientation &)));
}

void LockScreenUI::orientationChanged(const Dui::Orientation &orientation)
{
    if (orientation == Dui::Portrait) {
        p_policy->activate();
    } else {
        l_policy->activate();
    }
}

void LockScreenUI::buttonReleased()
{
    qDebug() << "LockScreenUI::buttonReleased()";
}

void LockScreenUI::buttonPressed()
{
    qDebug() << "LockScreenUI::buttonPressed()";
}
