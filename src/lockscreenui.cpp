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

    slider = new UnlockSlider(this, "continuous");
    slider->setOrientation(Qt::Horizontal);
    slider->setMaximumHeight(70);
    slider->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    slider->setVisible(true);

    timeLabel = new DuiLabel(this);
    timeLabel->setObjectName("lockscreenTimeLabel");
    timeLabel->setAlignment(Qt::AlignLeft);

    dateLabel = new DuiLabel(this);
    dateLabel->setObjectName("lockscreenDateLabel");
    dateLabel->setAlignment(Qt::AlignLeft);
    updateDateTime();

    DuiLayout *layout = new DuiLayout(centralWidget());
    centralWidget()->setLayout(layout);

    QRect timeRect, dateRect, sliderRect;
    calculateRects(layout, timeRect, dateRect, sliderRect);

    DuiFreestyleLayoutPolicy* policy = new DuiFreestyleLayoutPolicy(layout);
    policy->setSpacing(2);
    policy->addItemAtGeometry(timeLabel, timeRect);
    policy->addItemAtGeometry(dateLabel, dateRect);
    policy->addItemAtGeometry(slider, sliderRect);

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

void LockScreenUI::calculateRects(DuiLayout* layout, QRect& timeRect, QRect& dateRect, QRect& sliderRect)
{
    qreal left,top,right,bottom;
    layout->getContentsMargins(&left,&top,&right,&bottom);
    qDebug() << "LockScreenUI::calculateRects() margins l:"<<left<<"r:"<<right<<"t:"<<top<<"b:"<<bottom;

    QSize size = DuiSceneManager::instance()->visibleSceneSize();
    int w = size.width() - (left + right);
    int h = size.height() - (top + bottom);
    qDebug() << "LockScreenUI::calculateRects() w:" << w << "h:" << h;

    timeRect = QRect(QPoint(w*0.05f, h*0.17f), QPoint(w*0.95f, h*0.28f));
    dateRect = QRect(QPoint(w*0.05f, h*0.3f), QPoint(w*0.95f, h*0.35f));
    sliderRect = QRect(QPoint(40, h*0.81f), QPoint(w-40, h*0.95f));
}

void LockScreenUI::orientationChanged(const Dui::Orientation &orientation)
{
    // This is temprary solution, see bug #130452
    // As far as I understand:
    //      Like DuiGridLayoutPolicy, DuiFreestyleLayoutPolicy doesn't support automatic
    //      portrait / landscape switching and it's needed to be done manually.
    //      Feature will be implemented later(?).
    // 16-Sep-2009 /tt
    //
    DuiLayout* layout = (DuiLayout*)(centralWidget()->layout());

    QRect timeRect, dateRect, sliderRect;
    calculateRects(layout, timeRect, dateRect, sliderRect);

    DuiFreestyleLayoutPolicy* policy = (DuiFreestyleLayoutPolicy*)layout->policy();
    policy->setItemGeometry(timeLabel, timeRect);
    policy->setItemGeometry(dateLabel, dateRect);
    policy->setItemGeometry(slider, sliderRect);
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
