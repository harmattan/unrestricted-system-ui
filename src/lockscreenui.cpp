#include "lockscreenui.h"
#include "unlockslider.h"

#include <QDebug>

#include <DuiLabel>
#include <DuiImage>
#include <DuiButton>
#include <DuiLayout>
#include <DuiFreestyleLayoutPolicy>
#include <DuiSceneManager>

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

    l_policy = new DuiFreestyleLayoutPolicy(layout);
    p_policy = new DuiFreestyleLayoutPolicy(layout);
    l_policy->setSpacing(2);
    p_policy->setSpacing(2);

    slider = new UnlockSlider(this, "continuous");
    slider->setOrientation(Qt::Horizontal);
    slider->setMaximumHeight(70);
    slider->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    slider->setVisible(true);

    timeLabel = new DuiLabel(this);
    timeLabel->setObjectName("lockscreenTimeLabel");
    dateLabel = new DuiLabel(this);
    dateLabel->setObjectName("lockscreenDateLabel");
    updateDateTime();

    int w = DuiSceneManager::instance()->visibleSceneSize().width();
    int h = DuiSceneManager::instance()->visibleSceneSize().height();

    // p_policy settings are just temporary!

    l_policy->addItemAtGeometry(timeLabel, QRect(QPoint(w*0.05f, h*0.17f), QPoint(w*0.95f, h*0.28f)));
    p_policy->addItemAtGeometry(timeLabel, QRect(QPoint(h*0.05f, w*0.17f), QPoint(h*0.95f, w*0.28f)));
    timeLabel->setAlignment(Qt::AlignLeft);
    l_policy->addItemAtGeometry(dateLabel, QRect(QPoint(w*0.05f, h*0.3f), QPoint(w*0.95f, h*0.35f)));
    p_policy->addItemAtGeometry(dateLabel, QRect(QPoint(h*0.05f, w*0.3f), QPoint(h*0.95f, w*0.35f)));
    dateLabel->setAlignment(Qt::AlignLeft);

    l_policy->addItemAtGeometry(slider, QRect(QPoint(40, h*0.81f), QPoint(760, h*0.95f)));
    p_policy->addItemAtGeometry(slider, QRect(QPoint(40, w*0.81f), QPoint(760, w*0.95f)));

    l_policy->activate();
    orientationChanged(DuiSceneManager::instance()->orientation());

    connect(slider, SIGNAL(unlocked()), this, SLOT(sliderUnlocked()));

    connect(DuiSceneManager::instance(),
        SIGNAL(orientationChanged(const Dui::Orientation &)),
        this, SLOT(orientationChanged(const Dui::Orientation &)));

    QTime t(QTime::currentTime());
    timerAdjusting = true;
    timer.start((60 - t.second()) + 1000, this);
}

void LockScreenUI::sliderUnlocked()
{
    disappear();
    slider->reset();
    emit unlocked();
}

void LockScreenUI::orientationChanged(const Dui::Orientation &orientation)
{
    if (orientation == Dui::Portrait) {
        p_policy->activate();
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
    timeLabel->setText(dt.time().toString("!! hh:mm A"));
    dateLabel->setText(dt.date().toString("!! dddd, d MMMM"));
}
