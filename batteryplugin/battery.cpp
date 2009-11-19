#include "battery.h"
#include "batterydbusinterface.h"
#include "batteryimage.h"

#include <DuiApplicationIfProxy>
#include <DuiButton>
#include <DuiContainer>
#include <DuiGridLayoutPolicy>
#include <DuiImage>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLocale>
#include <DuiStatusIndicatorMenuPluginInterface>
#include <DuiTheme>

#include <QGraphicsLinearLayout>
#include <QDebug>

const QString cssDir = "/usr/share/duistatusindicatormenu/themes/style/";
const QString CONTROL_PANEL_SERVICE_NAME = "com.nokia.DuiControlPanel";

Battery::Battery(DuiStatusIndicatorMenuInterface &statusIndicatorMenu, QGraphicsItem *parent) :
        DuiWidget(parent),
        dbusIf(NULL),
        statusIndicatorMenu(statusIndicatorMenu),
        modeLabel(NULL),
        timeLabel(NULL),
        batteryImage(NULL)
{
    qDebug() << Q_FUNC_INFO;

    DuiTheme::loadCSS(cssDir + "batteryplugin.css");

    // init widgets
    DuiLabel *headerLabel = new DuiLabel(trid("qtn_ener_battery", "Battery"));
    headerLabel->setObjectName("batteryHeaderLabel");
    modeLabel = new DuiLabel();
    modeLabel->setObjectName("batteryModeLabel");
    timeLabel = new DuiLabel();
    timeLabel->setObjectName("batteryTimeLabel");
    batteryImage = new BatteryImage();
    batteryImage->setObjectName("batteryImage");

    // insert widgets
    DuiLayout *layout = new DuiLayout();
    DuiGridLayoutPolicy *layoutPolicy = new DuiGridLayoutPolicy(layout);
    layoutPolicy->addItemAtPosition(headerLabel, 0, 0, 1, 1);
    layoutPolicy->addItemAtPosition(modeLabel, 0, 1, 1, 1);
    layoutPolicy->addItemAtPosition(timeLabel, 1, 0, 1, 2);
    layoutPolicy->addItemAtPosition(batteryImage, 2, 0, 1, 2);
    DuiContainer *container = new DuiContainer();
    container->centralWidget()->setLayout(layout);

    // get widget values
    dbusIf = new BatteryDBusInterface();
    connect(dbusIf, SIGNAL(PSMValueReceived(bool)), this, SLOT(updateModeLabel(bool)));
    connect(dbusIf, SIGNAL(remainingTimeValuesReceived(QStringList)), this, SLOT(updateTimeLabel(QStringList)));
    connect(dbusIf, SIGNAL(batteryBarValueReceived(int)), batteryImage, SLOT(updateBatteryLevel(int)));
    connect(dbusIf, SIGNAL(batteryCharging(int)), batteryImage, SLOT(startCharging(int)));
    connect(dbusIf, SIGNAL(batteryNotCharging()), batteryImage, SLOT(stopCharging()));

    dbusIf->PSMValueRequired();
    dbusIf->remainingTimeValuesRequired();
    dbusIf->batteryBarValueRequired();
    dbusIf->batteryChargingStateRequired();

    // mainLayout
    QGraphicsLinearLayout *mainLayout = new QGraphicsLinearLayout();
    mainLayout->addItem(container);
    this->setLayout(mainLayout);    
}

Battery::~Battery()
{
    qDebug() << Q_FUNC_INFO;
    delete dbusIf;
    dbusIf = NULL;
    delete batteryImage;
    batteryImage = NULL;
}

void Battery::updateModeLabel(bool toggle)
{
    qDebug() << Q_FUNC_INFO;
    modeLabel->setText((toggle ? trid("qtn_ener_psmode", "Power save mode") : trid("qtn_ener_normal", "Normal mode")));    
}

void Battery::updateTimeLabel(const QStringList &times)
{
    qDebug() << Q_FUNC_INFO;
    if(times.size() != 2)
        return;

    if(times.at(1).toInt() == 0) // a text, not minute value
        timeLabel->setText(times.at(1));
    else
        timeLabel->setText(timeValue(times.at(1).toInt()));
}

QString Battery::timeValue(int minutes)
{
    const QString TimeValueText = trid("qtn_ener_remtime", "%a hours %b minutes");
    const QString minutesPrefix = TimeValueText.section("%b", 1, 1).trimmed();
    const QString hoursPrefix = (TimeValueText.section("%b", 0, 0)).section("%a", 1, 1).trimmed();
    QString time;

    if(minutes < 60)
        time = QString("%1%2").arg(minutes).arg(minutesPrefix);
    else {
        QVariant minsVar = minutes%60;
        minsVar = (minsVar.toInt() == 0) ? "00" : minsVar;
        time = QString("%1:%2%3").arg(minutes/60).arg(minsVar.toString()).arg(hoursPrefix);
    }
    return time;
}
