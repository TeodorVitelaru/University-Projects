

#include <sstream>
#include "GUI.h"
#include "Errors.h"


void GUI::initGUI()
{
    //main layout
    QHBoxLayout* lyMain = new QHBoxLayout;
    this->setLayout(lyMain);

    //left side - table
    QWidget* left = new QWidget;
    QVBoxLayout* lyLeft = new QVBoxLayout;
    left->setLayout(lyLeft);
    this->tblTasks = new QTableWidget;
    this->tblTasks->setColumnCount(4);
    QStringList labels;
    labels << "ID" << "Descriere" << "Stare" << "nr prog";
    this->tblTasks->setHorizontalHeaderLabels(labels);
    lyLeft->addWidget(this->tblTasks);
    lyMain->addWidget(left);

    //right side
    QWidget* right = new QWidget;
    QVBoxLayout* lyRight = new QVBoxLayout;
    right->setLayout(lyRight);

    //form layout
    QWidget* form = new QWidget;
    QFormLayout* formLy = new QFormLayout;
    form->setLayout(formLy);
    lyRight->addWidget(form);
    formLy->addRow(lblId, txtId);
    formLy->addRow(lblDescriere, txtDescriere);
    formLy->addRow(lblStare, txtStare);
    formLy->addRow(lblProgramatori, txtProgramatori);
    formLy->addWidget(btnAdauga);

    //new form layout
    QWidget* form2 = new QWidget;
    QFormLayout* formLy2 = new QFormLayout;
    form2->setLayout(formLy2);
    lyRight->addWidget(form2);
    formLy2->addRow(lblNume, txtNume);



    lyMain->addWidget(right);

}

void GUI::connectSignalsSlots() {
    QObject::connect(btnAdauga, &QPushButton::clicked, this, [&]()
    {
        try {
            int id = txtId->text().toInt();
            string descriere = txtDescriere->text().toStdString();
            string stare = txtStare->text().toStdString();
            string line = txtProgramatori->text().toStdString();
            vector<string> programatori;
            stringstream ss(line);
            string programmer;
            while (ss >> programmer) {
                programatori.push_back(programmer);
            }
            this->srv.adaugare(id, descriere, programatori, stare);
            this->reloadData();
            txtDescriere->clear();
            txtId->clear();
            txtStare->clear();
            txtProgramatori->clear();
            QMessageBox::information(this, "Succes", "Task adaugat cu succes!");
        }
        catch (Exception& ex) {
            QMessageBox::critical(this, "Error", QString::fromStdString(ex.get_msg()));
        }
    });

    QObject::connect(txtNume, &QLineEdit::textChanged, this, [&]() {
        string nume = txtNume->text().toStdString();
        this->tblTasks->setRowCount(0);
        vector<Task> tasks = this->srv.filrare(nume);
        for (const auto& task : tasks) {
            int row = this->tblTasks->rowCount();
            this->tblTasks->setRowCount(row + 1);
            this->tblTasks->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(to_string(task.get_id()))));
            this->tblTasks->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(task.get_descriere())));
            this->tblTasks->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(task.get_stare())));
            this->tblTasks->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(to_string(task.nr_prog()))));
        }
    });


}

void GUI::reloadData() {
    this->tblTasks->setRowCount(0);
    vector<Task> tasks = this->srv.sortare();
    for (const auto& task : tasks) {
        int row = this->tblTasks->rowCount();
        this->tblTasks->setRowCount(row + 1);
        this->tblTasks->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(to_string(task.get_id()))));
        this->tblTasks->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(task.get_descriere())));
        this->tblTasks->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(task.get_stare())));
        this->tblTasks->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(to_string(task.nr_prog()))));
    }
}

void GUI::openWindow() {
    auto f1 = new FereastraMea{ this->srv, "Open" };
    f1->run();
    auto f2 = new FereastraMea{ this->srv, "In progress" };
    f2->run();
    auto f3 = new FereastraMea{ this->srv, "Closed" };
    f3->run();

}