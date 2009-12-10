#include "networklist.h"

#include <QModelIndex>
#include <QDebug>
#include <QStringListModel>


// Class list cannot inherit DuiList because the signals are not inherited along

NetworkList::NetworkList(QGraphicsItem *parent) :
        DuiList(parent),        
        listModel(NULL)
{
    setObjectName("availableNetworksList");
    enableItemSelection(true);
    listModel = new QStringListModel;    
    connect(this, SIGNAL(itemClicked(const QModelIndex &)), this, SLOT(availableOperatorClicked(const QModelIndex &)));
}

NetworkList::~NetworkList()
{
}

void NetworkList::insertOperators(int selected, const QStringList &operators)
{    
    listModel->setStringList(operators);
    this->setItemModel(listModel);

    if(selected != -1)
        selectItem(listModel->index(selected));        
}

void NetworkList::removeOperators()
{
    listModel->setStringList(QStringList());
}

void NetworkList::availableOperatorClicked(const QModelIndex &index)
{       
    emit availableOperatorSelected(index.row());
}

