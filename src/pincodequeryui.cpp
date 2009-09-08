#include "pincodequeryui.h"

#include <DuiWidget>
#include <DuiLocale>
#include <DuiTextEdit>
#include <DuiTheme>
#include <DuiSceneManager>
#include <DuiGrid>
#include <DuiGridLayoutPolicy>
#include <DuiButton>
#include <DuiLabel>
#include <DuiTextEdit>

#include <QStringList>
#include <QSizePolicy>
#include <QTimer>
#include <QDebug>

PinCodeQueryUI::PinCodeQueryUI()
{
    qDebug() << "PinCodeQueryUI()";
    setFullscreen(true);
    setNavigationBarVisible(false);
    setTitle("PIN code query:");
    createContent();
}

PinCodeQueryUI::~PinCodeQueryUI()
{
    qDebug() << "~PinCodeQueryUI";

    if(backspaceTimer != NULL) {
        //we stop timing the press event
        backspaceTimer->stop();
        delete backspaceTimer;
        backspaceTimer = NULL;
    }

    delete emergencyCallButton;
    emergencyCallButton = NULL;
    delete enterButton;
    enterButton = NULL;
    delete cancelButton;
    cancelButton = NULL;
    delete backspaceButton;
    backspaceButton = NULL;
    delete headerLabel;
    headerLabel = NULL;
    delete entryTextEdit;
    entryTextEdit = NULL;
    delete numpadLayout;
    numpadLayout = NULL;
    delete portraitPolicy;
    portraitPolicy = NULL;
    delete landscapePolicy;
    landscapePolicy = NULL;
}

void PinCodeQueryUI::createContent()
{
    DuiApplicationPage::createContent();
    DuiWidget *panel = centralWidget();

    //create the widgets and numpad.
    createWidgetItems();

    //create mainLayout
    DuiLayout *mainLayout = new DuiLayout(panel);
    //mainLayout->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    //create landscapePolicy and portraitPolicy for mainLayout
    landscapePolicy = new DuiGridLayoutPolicy(mainLayout);
    portraitPolicy = new DuiGridLayoutPolicy(mainLayout);

    //attach widget items to landscape and portrait policies
    landscapePolicy->addItemAtPosition(headerLabel, 0, 0, 1, 5);
    landscapePolicy->addItemAtPosition(emergencyCallButton, 1, 0, 1, 2);
    landscapePolicy->addItemAtPosition(entryTextEdit, 2, 0, 1, 3);
    landscapePolicy->addItemAtPosition(backspaceButton, 2, 3, 1, 1);
    landscapePolicy->addItemAtPosition(enterButton, 3, 0, 1, 2);
    landscapePolicy->addItemAtPosition(cancelButton, 3, 2, 1, 2);
    landscapePolicy->addItemAtPosition(numpadLayout, 1, 4, 3, 1);
    portraitPolicy->addItemAtPosition(headerLabel, 0, 0, 1, 4);
    portraitPolicy->addItemAtPosition(emergencyCallButton, 1, 0, 1, 4);
    portraitPolicy->addItemAtPosition(entryTextEdit, 2, 0, 1, 3);
    portraitPolicy->addItemAtPosition(backspaceButton, 2, 3);
    portraitPolicy->addItemAtPosition(numpadLayout, 3, 0, 1, 4);
    portraitPolicy->addItemAtPosition(enterButton, 4, 0, 1, 2);
    portraitPolicy->addItemAtPosition(cancelButton, 4, 2, 1, 2);

    //set column and row sizes
    int fullWidth = DuiSceneManager::instance()->visibleSceneSize().width();
    int fullHeight = DuiSceneManager::instance()->visibleSceneSize().height();
    for(int lsCol=0; lsCol<5; ++lsCol) {
	if(lsCol < 4)
            landscapePolicy->setColumnFixedWidth(lsCol, fullWidth/8);
	else
	    landscapePolicy->setColumnFixedWidth(lsCol, fullWidth/2);
    }
    for(int lsRow=0; lsRow<4;++lsRow) {
	landscapePolicy->setRowFixedHeight(lsRow, fullHeight/4);
    }
    for(int ptCol=0; ptCol<4; ++ptCol) {
	portraitPolicy->setColumnFixedWidth(ptCol, fullHeight/4);
    }
    for(int ptRow=0; ptRow<5; ++ptRow) {
	portraitPolicy->setRowFixedHeight(ptRow, fullWidth/5);
    }
	    
    orientationChanged(DuiSceneManager::instance()->orientation());

//    DuiTheme::loadCSS("pinquery.css");

    // HACK: make all buttons visible :)
    /*
    requestPageMove(QPointF(0, -1), QPointF(0, 1), false);
    restorePageMove();
   */


    // Get informed about orientation changes
    connect(DuiSceneManager::instance(),
            SIGNAL(orientationChanged(const Dui::Orientation &)),
            this, SLOT(orientationChanged(const Dui::Orientation &)));

    backspaceTimer = NULL;
}

DuiButton *PinCodeQueryUI::getEmergencyBtn()
{
    return emergencyCallButton;
}
DuiButton *PinCodeQueryUI::getCancelBtn()
{
    return cancelButton;
}
DuiButton *PinCodeQueryUI::getEnterBtn()
{
    return enterButton;
}
DuiTextEdit *PinCodeQueryUI::getCodeEntry()
{
    return entryTextEdit;
}
void PinCodeQueryUI::setHeader(QString header)
{
    headerLabel->setText(header);
}


void PinCodeQueryUI::createWidgetItems()
{    
    emergencyCallButton = new DuiButton(QString(trid("qtn_cell_emergency_call", "Emergency call")), 0 );
    emergencyCallButton->setObjectName("emergencyCallButton");
    connect(emergencyCallButton, SIGNAL(released()), this, SLOT(buttonReleased()));

    entryTextEdit = new DuiTextEdit(DuiTextEditModel::SingleLine, "", 0);
    entryTextEdit->setObjectName("codeEntry");
    entryTextEdit->setMaskedInput(true);

    enterButton = new DuiButton(QString(trid("qtn_cell_enter", "Enter")), 0);
    enterButton->setObjectName("enterButton");
    connect(enterButton, SIGNAL(released()), this, SLOT(buttonReleased()));

    cancelButton = new DuiButton(QString(trid("qtn_comm_cancel", "Cancel")), 0);
    cancelButton->setObjectName("cancelButton");
    connect(cancelButton, SIGNAL(released()), this, SLOT(buttonReleased()));

    backspaceButton = new DuiButton(0);
    backspaceButton->setObjectName("backspaceButton");
    backspaceButton->setIconID("Icon-back");
    connect(backspaceButton, SIGNAL(released()), this, SLOT(buttonReleased()));
    connect(backspaceButton, SIGNAL(pressed()), this, SLOT(buttonPressed()));

    headerLabel = new DuiLabel(0);
    headerLabel->setAlignment(Qt::AlignCenter);    
    
    createNumpad();
}

void PinCodeQueryUI::createNumpad()
{
    //create numpadLayout and policy for that
    numpadLayout = new DuiLayout(0);
    DuiGridLayoutPolicy *numpadLayoutPolicy = new DuiGridLayoutPolicy(numpadLayout);

    int values[] = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9,
        0
    };

    int colSpan = 1;
    for (unsigned int i=0; i<sizeof(values)/sizeof(values[0]); ++i) {
	
	if(values[i] == 0)
	    colSpan = 3;

        QString str1 = QString("qtn_cell_dialer_").append(QString::number(values[i]));
        QString str2 = QString::number(values[i]);
        QString str3 = QString("numpadButton" + str2);
        DuiButton *num = new DuiButton(QString(trid(str1.toLatin1(), str2.toLatin1())), 0);
        num->setObjectName(str3);
        connect(num, SIGNAL(released()), this, SLOT(buttonReleased()));
        numpadLayoutPolicy->addItemAtPosition(num, (i/3), i%3, 1, colSpan); 
    }
}

void PinCodeQueryUI::buttonReleased()
{    
    DuiButton* button = static_cast<DuiButton*>(this->sender());

    //Check if the button was a numpad button       
    if(button->objectName().left(button->objectName().length()-1) == "numpadButton") {
        entryTextEdit->insert(button->objectName().right(1));
        qDebug() << "text now: " << entryTextEdit->text();
    }

    //Check if the button was backspace
    else if(button->objectName() == QString("backspaceButton")) {
        if(backspaceTimer != NULL) {
            //we stop timing the press event
            backspaceTimer->stop();
            delete backspaceTimer;
            backspaceTimer = NULL;
        }
        entryTextEdit->setText(entryTextEdit->text().left(entryTextEdit->text().length()-1));
    }
}

void PinCodeQueryUI::buttonPressed()
{    
    DuiButton* button = static_cast<DuiButton*>(this->sender());
    //Check if the button was backspace
    if(button->objectName() == QString("backspaceButton")) {
        //we check if the user holds the button down for 1 second or longer
        backspaceTimer = new QTimer(0);        
        connect(backspaceTimer, SIGNAL(timeout()), this, SLOT(removeText()));
        backspaceTimer->start(1000);
    }
}

void PinCodeQueryUI::removeText()
{
    if(backspaceTimer != NULL) {
        backspaceTimer->stop();
        delete backspaceTimer;
        backspaceTimer = NULL;
    }
    entryTextEdit->setText("");
}

void PinCodeQueryUI::orientationChanged(const Dui::Orientation &orientation)
{
    qDebug() << "orientationChanged";
    if (orientation == Dui::Portrait) {
        portraitPolicy->activate();
    } else {
        landscapePolicy->activate();
    }
}

