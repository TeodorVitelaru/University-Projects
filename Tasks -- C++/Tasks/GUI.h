#include "Service.h"
#include <QtWidgets/QApplication>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QTableWidget>
#include <QMessageBox>
#include <QHeaderView>
#include <QGroupBox>
#include <QRadioButton>
#include <qlistwidget.h>
#include <QStringList>
#include <QPainter>
#pragma once
#include "Fereastra.h"

class GUI : public QWidget{
private:
    Service& srv;
    QTableWidget* tblTasks = new QTableWidget;

    //pentru adaugare
    QLineEdit* txtId = new QLineEdit;
    QLineEdit* txtDescriere = new QLineEdit;
    QLineEdit* txtStare = new QLineEdit;
    QLineEdit* txtProgramatori = new QLineEdit;
    QPushButton* btnAdauga = new QPushButton{ "&Adauga" };
    QLabel* lblId = new QLabel{ "Id" };
    QLabel* lblDescriere = new QLabel{ "Descriere" };
    QLabel* lblStare = new QLabel{ "Stare" };
    QLabel* lblProgramatori = new QLabel{ "Programatori" };

    QLineEdit* txtNume = new QLineEdit;
    QLabel* lblNume = new QLabel{ "Nume programator:" };

    void initGUI();
    void connectSignalsSlots();
    void reloadData();
    void openWindow();
public:
    GUI(Service& srv) : srv{ srv } {
        this->openWindow();
        this->initGUI();
        this->connectSignalsSlots();
        this->reloadData();
    }


};



