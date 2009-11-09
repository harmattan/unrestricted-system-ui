#include "lockscreenui.h"
#include "unlocksliderwidget/unlockslider.h"

#include <QDebug>

#include <DuiLabel>
#include <DuiImage>
#include <DuiButton>
#include <DuiLayout>
#include <DuiFreestyleLayoutPolicy>
#include <DuiSceneManager>
#include <DuiTheme>
#include <QTime>

#include <QDateTime>

LockScreenUI::LockScreenUI()
{
    qDebug() << Q_FUNC_INFO;
    setFullscreen(true);
    setDisplayMode(0);
    setPannableAreaInteractive(false);
//    createContent();
}

LockScreenUI::~LockScreenUI()
{
}

void LockScreenUI::createContent()
{
    qDebug() << Q_FUNC_INFO;

    DuiApplicationPage::createContent();

    slider = new UnlockSlider(this);
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

    // TODO: if icon id used we have a problem with sizing. that's why use directly svg icons.
    unreadEmailsImage = new DuiImage("icon-m-notification-email", this);
    //unreadEmailsImage->setImage(QImage("/usr/share/sysuid/themes/svg/emails-missed.svg"));
    unreadMessagesImage = new DuiImage("icon-m-notification-sms", this);
    //unreadMessagesImage->setImage(QImage("/usr/share/sysuid/themes/svg/messages-missed.svg"));
    missedCallsImage = new DuiImage("icon-m-notification-call", this);
    //missedCallsImage->setImage(QImage("/usr/share/sysuid/themes/svg/call-missed.svg"));
    unreadChatMessagesImage = new DuiImage("icon-m-notification-im", this);
    //unreadChatMessagesImage->setImage(QImage("/usr/share/sysuid/themes/svg/chat-missed.svg"));

    unreadEmailsLbl = new DuiLabel("0", this);
    unreadMessagesLbl = new DuiLabel("2", this);
    missedCallsLbl = new DuiLabel("1", this);
    unreadChatMessagesLbl = new DuiLabel("3", this);

    DuiLayout *layout = new DuiLayout(centralWidget());
    centralWidget()->setLayout(layout);
    DuiFreestyleLayoutPolicy* policy = new DuiFreestyleLayoutPolicy(layout);
    policy->setSpacing(2);

    QRect timeRect, dateRect, a,b,c,d, a2,b2,c2,d2, sliderRect;
    calculateRects(layout, timeRect, dateRect, a,b,c,d, a2,b2,c2,d2, sliderRect);

    timeLabel->setGeometry(timeRect);
    dateLabel->setGeometry(dateRect);

    unreadEmailsImage->setGeometry(a);
    unreadMessagesImage->setGeometry(b);
    missedCallsImage->setGeometry(c);
    unreadChatMessagesImage->setGeometry(d);

    unreadEmailsLbl->setGeometry(a2);
    unreadMessagesLbl->setGeometry(b2);
    missedCallsLbl->setGeometry(c2);
    unreadChatMessagesLbl->setGeometry(d2);

    slider->setGeometry(sliderRect);

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

void LockScreenUI::calculateRects(DuiLayout* layout,
                                  QRect& timeRect, QRect& dateRect,
                                  QRect& a, QRect& b, QRect& c, QRect& d,
                                  QRect& a2, QRect& b2, QRect& c2, QRect& d2,
                                  QRect& sliderRect)
{
    qreal left,top,right,bottom;
    layout->getContentsMargins(&left,&top,&right,&bottom);
    qDebug() << "LockScreenUI::calculateRects() margins l:"<<left<<"r:"<<right<<"t:"<<top<<"b:"<<bottom;

    QSize size = DuiSceneManager::instance()->visibleSceneSize();
    int w = size.width() - (left + right);
    int h = size.height() - (top + bottom);
    qDebug() << "LockScreenUI::calculateRects() w:" << w << "h:" << h << ". "<< size;

    timeRect = QRect(QPoint(w*0.05f, h*0.17f), QPoint(w*0.95f, h*0.28f));
    dateRect = QRect(QPoint(w*0.05f, h*0.3f), QPoint(w*0.95f, h*0.35f));

    float x,y;
    unreadMessagesImage->setGeometry(QRect(0,0, w*0.15f, h*0.1f));
    unreadMessagesImage->zoomFactor(&x, &y);
    unreadMessagesImage->zoomFactor(&x, &y);
    QSizeF s = unreadMessagesImage->imageSize();
    s *= x;
    qDebug() << unreadMessagesImage->imageSize() << s;

    int imgW = s.width(),
        imgH = s.height(),
        imgY = h*0.5f;

    a = QRect(0.5f*imgW,    imgY,   imgW, imgH);
    b = QRect(2*imgW,       imgY,   imgW, imgH);
    c = QRect(3.5f*imgW,    imgY,   imgW, imgH);
    d = QRect(5*imgW,       imgY,   imgW, imgH);

    int orig = imgW;
    imgW *= 0.3;
    imgH *= 0.3;
    a2 = QRect(1.2f*orig,   imgY,   imgW, imgH);
    b2 = QRect(2.7*orig,    imgY,   imgW, imgH);
    c2 = QRect(4.2f*orig,   imgY,   imgW, imgH);
    d2 = QRect(5.7f*orig,   imgY,   imgW, imgH);

    sliderRect = QRect(QPoint(40, h*0.81f), QPoint(w-40, h*0.95f));
}

void LockScreenUI::orientationChanged(const Dui::Orientation &orientation)
{
    Q_UNUSED(orientation);
    qDebug() << Q_FUNC_INFO;

    // This is temprary solution, see bug #130452
    // As far as I understand:
    //      Like DuiGridLayoutPolicy, DuiFreestyleLayoutPolicy doesn't support automatic
    //      portrait / landscape switching and it's needed to be done manually.
    //      Feature will be implemented later(?).
    // 16-Sep-2009 /tt
    //
    DuiLayout* layout = (DuiLayout*)(centralWidget()->layout());

    QRect timeRect, dateRect, a,b,c,d, a2,b2,c2,d2, sliderRect;
    calculateRects(layout, timeRect, dateRect, a,b,c,d, a2,b2,c2,d2, sliderRect);
/*
    // Animation not supported because of headache with freestyle layout policy.

    DuiFreestyleLayoutPolicy* policy = (DuiFreestyleLayoutPolicy*)layout->policy();
    policy->setItemGeometry(timeLabel, timeRect);
    policy->setItemGeometry(dateLabel, dateRect);

    policy->setItemGeometry(unreadMessagesImage, a);
    policy->setItemGeometry(missedCallsImage, b);
    policy->setItemGeometry(unreadChatMessagesImage, c);

    policy->setItemGeometry(slider, sliderRect);
 */

    timeLabel->setGeometry(timeRect);
    dateLabel->setGeometry(dateRect);

    unreadMessagesImage->setGeometry(a);
    missedCallsImage->setGeometry(b);
    unreadEmailsImage->setGeometry(c);
    unreadChatMessagesImage->setGeometry(d);

    unreadMessagesLbl->setGeometry(a2);
    missedCallsLbl->setGeometry(b2);
    unreadEmailsLbl->setGeometry(c2);
    unreadChatMessagesLbl->setGeometry(d2);

    slider->setGeometry(sliderRect);

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

// Check dbus adaptor!
//    Q_NOREPLY void SetMissedEvents(int emails, int messages, int calls, int im);
void LockScreenUI::updateMissedEventAmounts(int emails, int messages, int calls, int chatMessages)
{
    qDebug() << "LockScreenUI::updateMissedEventAmounts(" << calls << ", " << messages << ", " <<  emails << ", " << chatMessages << ")";
    missedCallsLbl->setText(QString("%1").arg(calls));
    unreadMessagesLbl->setText(QString("%1").arg(messages));
    unreadEmailsLbl->setText(QString("%1").arg(emails));
    unreadChatMessagesLbl->setText(QString("%1").arg(chatMessages));
}
