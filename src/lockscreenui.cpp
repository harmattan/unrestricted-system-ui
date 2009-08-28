#include "lockscreenui.h"

#include "unlockslider.h"

#include <DuiLabel>
#include <DuiImage>
#include <DuiButton>
#include <DuiLayout>
#include <DuiGridLayoutPolicy>
#include <DuiSceneManager>
#include <DuiSlider>

#include <QDateTime>

LockScreenUI::LockScreenUI()
{
    setFullscreen(true);
    setNavigationBarVisible(false);
    setPannableAreaInteractive(false);
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

    slider = new UnlockSlider(this, "continuous");
    slider->setOrientation(Qt::Horizontal);
    slider->setMaximumHeight(70);
    slider->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));
    slider->setVisible(true);

    timeLabel = new DuiLabel(this);
    dateLabel = new DuiLabel(this);
    updateDateTime();

    l_policy->setContentsMargins(8, 8, 8, 8);
    l_policy->addItemAtPosition(timeLabel, 1, 1, 1, 2);
    l_policy->addItemAtPosition(dateLabel, 2, 1, 1, 2);
    l_policy->addItemAtPosition(slider,    5, 2, 1, 5);

    int w = DuiSceneManager::instance()->visibleSceneRect().width();
    int h = DuiSceneManager::instance()->visibleSceneRect().height();

    for (int i=0; i<9; i++) {
        l_policy->setColumnFixedWidth(i, w/9);
    }
    for (int i=0; i<7; i++) {
        l_policy->setRowFixedHeight(i, h/7);
    }

    orientationChanged(DuiSceneManager::instance()->orientation());

    connect(slider, SIGNAL(unlocked()), this, SLOT(unlocked()));

    connect(DuiSceneManager::instance(),
        SIGNAL(orientationChanged(const Dui::Orientation &)),
        this, SLOT(orientationChanged(const Dui::Orientation &)));

    QTime t(QTime::currentTime());
    timerAdjusting = true;
    timer.start((60 - t.second()) * 1000 + 1100, this);
}

void LockScreenUI::unlocked()
{
    disappear();
}

void LockScreenUI::orientationChanged(const Dui::Orientation &orientation)
{
    if (orientation == Dui::Portrait) {
//        p_policy->activate();
        l_policy->activate();
    } else {
        l_policy->activate();
    }
}

void LockScreenUI::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timer.timerId()) {
        if (timerAdjusting) {
            timerAdjusting = false;
            timer.start(60 * 1000, this);
        }
        updateDateTime();
        update();
    }
}

void LockScreenUI::updateDateTime()
{
    QDateTime dt(QDateTime::currentDateTime());
    // TODO: locale stuff
    timeLabel->setText(dt.time().toString("hh:mm A"));
    dateLabel->setText(dt.date().toString("dddd, d MMMM"));
}
