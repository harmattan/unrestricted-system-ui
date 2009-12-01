#include "lockscreenui.h"
#include "unlocksliderwidget/unlockslider.h"

#include <QDebug>

#include <DuiLabel>
#include <DuiImage>
#include <DuiButton>
#include <DuiHomeButtonPanel>
#include <DuiLayout>
#include <DuiGridLayoutPolicy>
#include <DuiSceneManager>
#include <DuiApplication>
#include <DuiApplicationWindow>
#include <DuiTheme>
#include <DuiLocale>

#include <QTime>
#include <QDateTime>

LockScreenUI::LockScreenUI() :
        timeLabel(NULL),
        dateLabel(NULL),
        unreadEmailsImage(NULL),
        unreadMessagesImage(NULL),
        missedCallsImage(NULL),
        unreadChatMessagesImage(NULL),
        unreadEmailsLabel(NULL),
        unreadMessagesLabel(NULL),
        missedCallsLabel(NULL),
        unreadChatMessagesLabel(NULL)
{
    qDebug() << Q_FUNC_INFO;
    setFullscreen(true);
    setDisplayMode(0);
    setPannableAreaInteractive(false);

    // let's hide home button
    DuiApplication::activeApplicationWindow()->homeButtonPanel()->disappearNow();
}

LockScreenUI::~LockScreenUI()
{
}

void LockScreenUI::createContent()
{
    qDebug() << Q_FUNC_INFO;

    DuiApplicationPage::createContent();

    DuiLayout* layout = new DuiLayout;
    DuiLayout* widgets = createWidgets();

    QSize size = DuiApplication::activeApplicationWindow()->
                 sceneManager()->visibleSceneSize(Dui::Landscape);

    DuiGridLayoutPolicy* l_policy = new DuiGridLayoutPolicy(layout);
    l_policy->setSpacing(10);
    l_policy->setRowFixedHeight(1, size.height());
    l_policy->setColumnFixedWidth(0, size.width());
    l_policy->addItemAtPosition(widgets, 1, 0, Qt::AlignCenter);

    size = DuiApplication::activeApplicationWindow()->
           sceneManager()->visibleSceneSize(Dui::Portrait);

    DuiGridLayoutPolicy* p_policy = new DuiGridLayoutPolicy(layout);
    p_policy->setSpacing(10);
    p_policy->setRowFixedHeight(1, size.height());
    p_policy->setColumnFixedWidth(0, size.width());
    p_policy->addItemAtPosition(widgets, 1, 0, Qt::AlignCenter);

    layout->setLandscapePolicy(l_policy);
    layout->setPortraitPolicy(p_policy);

    centralWidget()->setLayout(layout);

    connect(slider, SIGNAL(unlocked()), this, SLOT(sliderUnlocked()));
}

void LockScreenUI::sliderUnlocked()
{
    disappear();
    slider->reset();
    emit unlocked();
}

DuiLayout* LockScreenUI::createWidgets()
{
    DuiLayout* layout = new DuiLayout(this);
    DuiGridLayoutPolicy* policy = new DuiGridLayoutPolicy(layout);

    policy->setColumnAlignment(0, Qt::AlignCenter);

    QGraphicsWidget* spacert = new QGraphicsWidget;
    spacert->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding));

    timeLabel = new DuiLabel;
    timeLabel->setObjectName("lockscreenTimeLabel");
    timeLabel->setAlignment(Qt::AlignCenter);

    dateLabel = new DuiLabel;
    dateLabel->setObjectName("lockscreenDateLabel");
    dateLabel->setAlignment(Qt::AlignCenter);

    updateDateTime();
    /*
        DuiSlider* slider = new DuiSlider(NULL, "dot");
        slider->setOrientation(Qt::Horizontal);
    */
    slider = new UnlockSlider;
    slider->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    slider->setVisible(true);

    slider->setObjectName("unlockslider");
    slider->setMinimumWidth(450);
    slider->setMaximumWidth(450);
    slider->setMaximumHeight(80);

    QGraphicsWidget* spacerl = new QGraphicsWidget;
    spacerl->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));

    // TODO: if icon id used we have a problem with sizing. that's why use directly svg icons.
    unreadEmailsImage = new DuiImage("icon-m-notification-email");
    //unreadEmailsImage->setImage(QImage("/usr/share/sysuid/themes/svg/emails-missed.svg"));
    unreadMessagesImage = new DuiImage("icon-m-notification-sms");
    //unreadMessagesImage->setImage(QImage("/usr/share/sysuid/themes/svg/messages-missed.svg"));
    missedCallsImage = new DuiImage("icon-m-notification-call");
    //missedCallsImage->setImage(QImage("/usr/share/sysuid/themes/svg/call-missed.svg"));
    unreadChatMessagesImage = new DuiImage("icon-m-notification-im");
    //unreadChatMessagesImage->setImage(QImage("/usr/share/sysuid/themes/svg/chat-missed.svg"));

    // TODO: some suitable layout could be added to image and then the label to that layout

    unreadEmailsLabel = new DuiLabel("0", unreadEmailsImage);
    unreadMessagesLabel = new DuiLabel("2", unreadMessagesImage);
    missedCallsLabel = new DuiLabel("1", missedCallsImage);
    unreadChatMessagesLabel = new DuiLabel("3", unreadChatMessagesImage);

    QGraphicsWidget* spacerr = new QGraphicsWidget;
    spacerr->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));

    QGraphicsWidget* spacerb = new QGraphicsWidget;
    spacerb->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding));

    policy->addItemAtPosition(spacert,                 0, 0, 1, 6);
    policy->addItemAtPosition(timeLabel,               1, 0, 1, 6);
    policy->addItemAtPosition(dateLabel,               2, 0, 1, 6);
    policy->addItemAtPosition(slider,                  3, 0, 1, 6);
    policy->addItemAtPosition(spacerl,                 4, 0);
    policy->addItemAtPosition(unreadEmailsImage,       4, 1);
    policy->addItemAtPosition(unreadMessagesImage,     4, 2);
    policy->addItemAtPosition(missedCallsImage,        4, 3);
    policy->addItemAtPosition(unreadChatMessagesImage, 4, 4);
    policy->addItemAtPosition(spacerr,                 4, 5);
    policy->addItemAtPosition(spacerb,                 5, 0, 1, 6);

    return layout;
}

void LockScreenUI::updateDateTime()
{
    DuiLocale locale;

    QDateTime now(QDateTime::currentDateTime());

    timeLabel->setText(locale.formatDateTime(now, DuiLocale::DateNone, DuiLocale::TimeShort));
    dateLabel->setText(locale.formatDateTime(now, DuiLocale::DateFull, DuiLocale::TimeNone));
    update();
}

// Check dbus adaptor!
//    Q_NOREPLY void SetMissedEvents(int emails, int messages, int calls, int im);
void LockScreenUI::updateMissedEventAmounts(int emails, int messages, int calls, int chatMessages)
{
    qDebug() << "LockScreenUI::updateMissedEventAmounts(" << calls << ", " << messages << ", " <<  emails << ", " << chatMessages << ")";
    missedCallsLabel->setText(QString("%1").arg(calls));
    unreadMessagesLabel->setText(QString("%1").arg(messages));
    unreadEmailsLabel->setText(QString("%1").arg(emails));
    unreadChatMessagesLabel->setText(QString("%1").arg(chatMessages));
}
