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
    unreadMessagesImage = new DuiImage(/*"icon-m-unread-messages",*/ this);
    unreadMessagesImage->setImage(QImage("/usr/share/sysuid/themes/svg/messages-missed.svg"));
    missedCallsImage = new DuiImage(/*"icon-m-call-missed", */this);
    missedCallsImage->setImage(QImage("/usr/share/sysuid/themes/svg/call-missed.svg"));
    unreadChatMessagesImage = new DuiImage(/*"icon-m-unread-chat-messages",*/ this);
    unreadChatMessagesImage->setImage(QImage("/usr/share/sysuid/themes/svg/chat-missed.svg"));

    unreadMessagesLbl = new DuiLabel("2", this);
    missedCallsLbl = new DuiLabel("1", this);
    unreadChatMessagesLbl = new DuiLabel("3", this);

    DuiLayout *layout = new DuiLayout(centralWidget());
    centralWidget()->setLayout(layout);
    DuiFreestyleLayoutPolicy* policy = new DuiFreestyleLayoutPolicy(layout);
    policy->setSpacing(2);

    QRect timeRect, dateRect, a,b,c, a2,b2,c2, sliderRect;
    calculateRects(layout, timeRect, dateRect, a,b,c, a2,b2,c2, sliderRect);

    timeLabel->setGeometry(timeRect);
    dateLabel->setGeometry(dateRect);
    unreadMessagesImage->setGeometry(a);
    missedCallsImage->setGeometry(b);
    unreadChatMessagesImage->setGeometry(c);
    unreadMessagesLbl->setGeometry(a2);
    missedCallsLbl->setGeometry(b2);
    unreadChatMessagesLbl->setGeometry(c2);
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
                                  QRect& messagesRect, QRect& callsRect, QRect& chatsRect,
                                  QRect& a2, QRect& b2, QRect& c2,
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

    messagesRect = QRect(0.5f*imgW, imgY,   imgW, imgH);
    callsRect = QRect(2*imgW,       imgY,   imgW, imgH);
    chatsRect = QRect(3.5f*imgW,    imgY,   imgW, imgH);

    int orig = imgW;
    imgW *= 0.3;
    imgH *= 0.3;
    a2 = QRect(1.2f*orig,   imgY,   imgW, imgH);
    b2 = QRect(2.7*orig,    imgY,   imgW, imgH);
    c2 = QRect(4.2f*orig,   imgY,   imgW, imgH);

    sliderRect = QRect(QPoint(40, h*0.81f), QPoint(w-40, h*0.95f));
}

void LockScreenUI::orientationChanged(const Dui::Orientation &orientation)
{
    Q_UNUSED(orientation);

    // This is temprary solution, see bug #130452
    // As far as I understand:
    //      Like DuiGridLayoutPolicy, DuiFreestyleLayoutPolicy doesn't support automatic
    //      portrait / landscape switching and it's needed to be done manually.
    //      Feature will be implemented later(?).
    // 16-Sep-2009 /tt
    //
    DuiLayout* layout = (DuiLayout*)(centralWidget()->layout());

    QRect timeRect, dateRect, a,b,c, a2,b2,c2, sliderRect;
    calculateRects(layout, timeRect, dateRect, a,b,c, a2,b2,c2, sliderRect);
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
    unreadChatMessagesImage->setGeometry(c);

    unreadMessagesLbl->setGeometry(a2);
    missedCallsLbl->setGeometry(b2);
    unreadChatMessagesLbl->setGeometry(c2);

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

void LockScreenUI::updateMissedEventAmounts(int calls, int messages, int emails, int chatMessages)
{
    qDebug() << "LockScreenUI::updateMissedEventAmounts(" << calls << ", " << messages << ", " <<  emails << ", " << chatMessages << ")";
    //will change when localizable strings available
    QString word = calls > 1 ? "calls" : "call";
    calls > 0 ? missedCallsLbl->setVisible(false) : missedCallsLbl->setVisible(true);
    missedCallsLbl->setText(QString("%1 unanswered %2").arg(calls).arg(word));

    //will change when localizable strings available
    word = messages > 1 ? "messages" : "message";
    messages > 0 ? unreadMessagesLbl->setVisible(false) : unreadMessagesLbl->setVisible(true);
    unreadMessagesLbl->setText(QString("%1 unread %2").arg(messages).arg(word));

    //will change when localizable strings available
    /*
    word = emails > 1 ? "emails" : "email";
    emails > 0 ? unreadEmailsLbl->setVisible(false) : unreadEmailsLbl->setVisible(true);
    unreadEmailsLbl->setText(QString("%1 unread %2").arg(emails).arg(word));
    */

    //will change when localizable strings available
    word = chatMessages > 1 ? "messages" : "message";
    chatMessages > 0 ? unreadChatMessagesLbl->setVisible(false) : unreadChatMessagesLbl->setVisible(true);
    unreadChatMessagesLbl->setText(QString("%1 unread chat %2").arg(chatMessages).arg(word));

}
