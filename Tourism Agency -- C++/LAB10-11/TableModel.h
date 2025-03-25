#pragma once
#include <QAbstractTableModel>
#include "domain.h"
#include <vector>
#include <qdebug.h>
#include <iostream>

using namespace std;

class TableModel : public QAbstractTableModel
{
    vector<Oferta> oferte;
public:
    TableModel(const std::vector<Oferta>& oferte) : oferte{oferte } {
    }

    int rowCount(const QModelIndex& parent = QModelIndex{}) const override
    {
        return oferte.size();
    }

    int columnCount(const QModelIndex& parent = QModelIndex{}) const override
    {
        return 4;
    }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override
    {
        if (role == Qt::DisplayRole)
        {
            Oferta p = oferte[index.row()];
            if(index.column() == 0)
                return QString::fromStdString(p.get_denumire());
            else if(index.column() == 1)
            {
                return QString::fromStdString(p.get_destinatie());
            }
            else if(index.column() == 2)
            {
                return QString::fromStdString(p.get_tip());
            }
            else if(index.column() == 3)
            {
                return QString::fromStdString(to_string(p.get_pret()));

            }
        }
        return QVariant{};
    }

    void setOferte(const vector<Oferta>& oferte1)
    {
        emit layoutAboutToBeChanged();
        this->oferte = oferte1;
        auto topLeft = createIndex(0, 0);
        auto bottomRight = createIndex(rowCount(), columnCount());
        emit dataChanged(topLeft, bottomRight);
        emit layoutChanged();
    }

};