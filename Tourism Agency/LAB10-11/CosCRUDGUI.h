#pragma once
#include <qwidget.h>
#include <qtimer.h>
#include <QtWidgets/QHBoxLayout>
#include <qpushbutton.h>
#include <qlistwidget.h>
#include <qtablewidget.h>
#include <qstring.h>
#include <qlabel.h>
#include <vector>
#include "cosOferte.h"
#include "observer.h"


class CosCRUDGUI : public QWidget, public Observer
{
private:
    cosOferte& cos;
    QListWidget* lst;
    QPushButton* btn;
    QPushButton* btnRandom;
    void loadList(const vector<Oferta>& oferte)
    {
        lst->clear();
        for (const auto& of : oferte)
        {
            auto item = new QListWidgetItem(QString::fromStdString(of.get_denumire() + " " + of.get_destinatie() + " " + of.get_tip() + " " + to_string(of.get_pret())));
            lst->addItem(item);
        }
    }
    void initGUI()
    {
        QVBoxLayout* ly = new QVBoxLayout;
        lst = new QListWidget;
        btn = new QPushButton("Goleste cos");
        btnRandom = new QPushButton("Genereaza 3 oferte");
        ly->addWidget(lst);
        ly->addWidget(btn);
        ly->addWidget(btnRandom);
        setLayout(ly);
    }
    void connectSignals()
    {
        QObject::connect(btn, &QPushButton::clicked, [&]() {
            cos.sterge_cos();
            loadList(cos.get_all_cos());
        });
        QObject::connect(btnRandom, &QPushButton::clicked, [&]() {
            cos.sterge_cos();
            cos.umple(3);
            loadList(cos.get_all_cos());
        });
    }
    public:
    CosCRUDGUI(cosOferte& cos) :cos{ cos }
    {
        cos.addObserver(this);
        initGUI();
        connectSignals();
        loadList(cos.get_all_cos());
    }
    void update() override
    {
        loadList(cos.get_all_cos());
    }
    ~CosCRUDGUI()
    {
        cos.removeObserver(this);
    }
};
