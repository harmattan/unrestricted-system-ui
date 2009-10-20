#include "networklist.h"

#include <DuiLabel>
#include <DuiList>
#include <DuiWidgetFactory>
#include <DuiWidgetListModel>
#include <QGraphicsWidget>
#include <QModelIndex>
#include <QDebug>


// Class list cannot inherit DuiList because the signals are not inherited along

NetworkList::NetworkList(QGraphicsItem *parent) :
        DuiList(parent),
        parent(parent),
        listModel(new DuiWidgetListModel(this))
{            
    setObjectName("availableNetworksList");
    enableItemSelection(true);
    setItemModel(listModel);
    connect(this, SIGNAL(itemClicked(const QModelIndex &)), this, SLOT(availableNetworkClicked(const QModelIndex &)));
}

NetworkList::~NetworkList()
{
}

bool NetworkList::insertNetworks(int selected, const QStringList &networks)
{        
    if(networks.size() <= 0)
        return false;

    for(int i=0; i<networks.size(); ++i)
        listModel->appendWidget(new DuiLabel(networks.at(i), parent));

    if(selected != -1)
        selectItem(listModel->index(selected));    

    return true;
}

void NetworkList::availableNetworkClicked(const QModelIndex &index)
{
    qDebug() << "NetworkList::availableNetworkSelected(" << index << ")";
    DuiLabel *label = static_cast<DuiLabel*>(DuiWidgetFactory::instance()->create(listModel, index.row()));
    emit availableNetworkSelected(label->text());    
}
