#include <string>
#include <vector>
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QFormLayout>
#include "Observer.h"
#include "GUI.h"
#pragma once


class FereastraMea : public Observer, public QWidget
{
private:
   Service& srv;
    string stare;
    QWidget* wnd = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout;
    QPushButton* btnOpen = new QPushButton{ "Open" };
    QPushButton* btnClose = new QPushButton{ "Close" };
    QPushButton* btnInProgress = new QPushButton{ "In progress" };
    QListWidget *listaTask = new QListWidget;
public:
    FereastraMea(Service& srv, string stare) :srv{ srv }, stare(stare) {
        reloadLista();
        connectSignals();
        this->srv.addObserver(this);
        this->wnd->setWindowTitle(QString::fromStdString(stare));
        this->wnd->setLayout(layout);
        layout->addWidget(listaTask);
        layout->addWidget(btnOpen);
        layout->addWidget(btnInProgress);
        layout->addWidget(btnClose);

    }
    void reloadLista()
    {
        this->listaTask->clear();
        for(auto& task : srv.get_all())
        {
            auto item = new QListWidgetItem(QString::fromStdString(to_string(task.get_id()) + "  " + task.get_descriere() + "  " + task.get_stare() + "  " + std::to_string(task.get_programatori().size())));
            this->listaTask->addItem(item);
        }
    }
    void run()
    {
        this->wnd->show();
    }
    void update() override
    {
        reloadLista();
    }
    void connectSignals()
    {
        QObject::connect(btnOpen, &QPushButton::clicked, this, [&]()
        {
            auto items = this->listaTask->selectedItems();
            if (items.isEmpty())
            {
                return;
            }
            auto item = items.at(0);
            auto text = item->text().toStdString();
            auto id = text.substr(0, text.find(" "));
            int id1= stoi(id);
            srv.modifica(id1, "open");
            reloadLista();
        });
        QObject::connect(btnInProgress, &QPushButton::clicked, this, [&]()
        {
            auto items = this->listaTask->selectedItems();
            if (items.isEmpty())
            {
                return;
            }
            auto item = items.at(0);
            auto text = item->text().toStdString();
            auto id = text.substr(0, text.find(" "));
            int id1 = stoi(id);
            srv.modifica(id1, "inprogress");
            reloadLista();
        });
        QObject::connect(btnClose, &QPushButton::clicked, this, [&]()
        {
            auto items = this->listaTask->selectedItems();
            if (items.isEmpty())
            {
                return;
            }
            auto item = items.at(0);
            auto text = item->text().toStdString();
            auto id = text.substr(0, text.find(" "));
            int id1 = stoi(id);
            srv.modifica(id1, "close");
            reloadLista();
        });
    }
};