//
// Created by Admin on 2024-05-25.
//

#include "IncercareGUI.h"
#include "CosCRUDGUI.h"
#include "CosReadOnlyGUI.h"

void IncercareGUI::initializeGUIComponents() {

    //main layout
    QHBoxLayout* lyMain = new QHBoxLayout;
    this->setLayout(lyMain);

    //left part of the window, pentu care setam layout vertical
    QWidget* lefet = new QWidget;
    QVBoxLayout* lyLeft = new QVBoxLayout;
    lefet->setLayout(lyLeft);

    QWidget* form = new QWidget;
    QFormLayout* lyForm = new QFormLayout;
    form->setLayout(lyForm);
    editDenumire = new QLineEdit;
    editDestinatie = new QLineEdit;
    editTip = new QLineEdit;
    editPret = new QLineEdit;

    //denumire
    lyForm->addRow(lblDenumire, editDenumire);

    //destinatie
    lyForm->addRow(lblDestinatie, editDestinatie);

    //tip
    lyForm->addRow(lblTip, editTip);

    //pret
    lyForm->addRow(lblPret, editPret);

    btnAddOferta = new QPushButton("Adauga oferta");
    lyForm->addWidget(btnAddOferta);
    btnModifyOferta = new QPushButton("Modifica oferta");
    lyForm->addWidget(btnModifyOferta);

    btnDeleteOferta = new QPushButton("Sterge oferta");
    lyForm->addWidget(btnDeleteOferta);

    //adaugam toate componentele legate de adaugare oferta in layout-ul care coresepunde partii in stanga a ferestrei
    lyLeft->addWidget(form);

    //creem un GroupBo petru radio buttons care corespund criteriilor de sortare pe care le putem avem
    QVBoxLayout* lyRadioBox = new QVBoxLayout;
    this->groupBox->setLayout(lyRadioBox);
    lyRadioBox->addWidget(radioSrtDenumire);
    lyRadioBox->addWidget(radioSrtDestinatie);
    lyRadioBox->addWidget(radioSrtTipPret);

    btnSortOferte = new QPushButton("Sorteaza ofertele");
    lyRadioBox->addWidget(btnSortOferte);

    //adaugam acest grup in partea stanga, dupa componentele pt adaugare in layout-ul vertical
    lyLeft->addWidget(groupBox);

    //creem un form pentru filtrari
    QWidget* formFilter = new QWidget;
    QFormLayout* lyFormFilter = new QFormLayout;
    formFilter->setLayout(lyFormFilter);

    editFilterPret = new QLineEdit();
    lyFormFilter->addRow(lblFilterCriteria, editFilterPret);
    btnFilterOferte1 = new QPushButton("Filtreaza dupa pret");
    lyFormFilter->addWidget(btnFilterOferte1);

    editFilterDestinatie = new QLineEdit();
    lyFormFilter->addRow(lblFilterCriteria2, editFilterDestinatie);
    btnFilterOferte2 = new QPushButton("Filtreaza dupa destinatie");
    lyFormFilter->addWidget(btnFilterOferte2);

    //adaugam form-ul de filtrare in layout-ul vertical
    lyLeft->addWidget(formFilter);

    //adaugam butonul de reload data
    btnReloadData = new QPushButton("Reincarca datele");
    lyLeft->addWidget(btnReloadData);

    btnUndo = new QPushButton("Undo");
    lyLeft->addWidget(btnUndo);

    btnClear = new QPushButton("Clear");
    lyLeft->addWidget(btnClear);

    btnCos = new QPushButton("Cos cumparaturi");
    adaugaCos = new QPushButton("Adauga in cos");
    denumireOfertaCos = new QLineEdit;
    destinatieOfertaCos = new QLineEdit;
    adaugaRandomCos = new QPushButton("Adauga random in cos");
    numarRandom = new QLineEdit;
    golesteCos = new QPushButton("Goleste cos");
    editExport = new QLineEdit;
    this->listaCos = new QListWidget;
    exportCos = new QPushButton("Export cos");

    //componenta right - contine doar tabelul cu oferte
    QWidget* right = new QWidget;
    QVBoxLayout* lyRight = new QVBoxLayout;
    right->setLayout(lyRight);

    btnAllInclusive = new QPushButton("AllInclusive");
    btnCityBreak = new QPushButton("CityBreak");
    btnHiking = new QPushButton("Hiking");
    btnMultiCountry = new QPushButton("MultiCountry");
    btnAltele = new QPushButton("Altele");
    lyRight->addWidget(btnAllInclusive);
    lyRight->addWidget(btnCityBreak);
    lyRight->addWidget(btnHiking);
    lyRight->addWidget(btnMultiCountry);
    lyRight->addWidget(btnAltele);

    lyRight->addWidget(tblV);

    lyRight->addWidget(adaugaCos);
    //lyRight->addWidget(adaugaRandomCos);
    lyRight->addWidget(golesteCos);
    //lyRight->addWidget(btnCos);
    lyRight->addWidget(btnCosCrud);
    lyRight->addWidget(btnCosReadOnly);

    lyMain->addWidget(lefet);
    lyMain->addWidget(right);
}

void IncercareGUI::connectSignalsSlots() {
    QObject::connect(btnAddOferta, &QPushButton::clicked, this, &IncercareGUI::guiAddOferta);
    QObject::connect(btnModifyOferta, &QPushButton::clicked, this, &IncercareGUI::guiModifyOferta);
    QObject::connect(btnDeleteOferta, &QPushButton::clicked, this, &IncercareGUI::guiDeleteOferta);
    QObject::connect(btnUndo, &QPushButton::clicked, [&]() {
        try {
            srv.undo();
            this->reloadOfertaList(this->srv.get_all_oferte());
            QMessageBox::information(this, "Info", QString::fromStdString("Undo efectuat cu succes!"));
        }
        catch (Exception &exception) {
            QMessageBox::warning(this, "Warning", QString::fromStdString(exception.getMessage()));
        }
        catch (RepoException &re) {
            QMessageBox::warning(this, "Warning", QString::fromStdString(re.getErrorMessage()));
        }
    });
    QObject::connect(btnSortOferte, &QPushButton::clicked, this, [&]() {
        try {
            if (radioSrtDenumire->isChecked())
                this->reloadOfertaList(this->srv.sortare_denumire());
            else if (radioSrtDestinatie->isChecked())
                this->reloadOfertaList(this->srv.sortare_destinatie());
            else if (radioSrtTipPret->isChecked())
                this->reloadOfertaList(this->srv.sortare_tipPret());
        }
        catch (Exception &exception) {
            QMessageBox::warning(this, "Warning", QString::fromStdString(exception.getMessage()));
        }
        catch (RepoException &re) {
            QMessageBox::warning(this, "Warning", QString::fromStdString(re.getErrorMessage()));
        }
    });
    QObject::connect(golesteCos, &QPushButton::clicked, [&]() {
        try {
            this->srv.cos_stergere();
            this->reloadOfertaCosList(this->srv.get_all_cos());
            QMessageBox::information(this, "Info", QString::fromStdString("Cosul a fost golit!"));
        }
        catch (Exception &exception) {
            QMessageBox::warning(this, "Warning", QString::fromStdString(exception.getMessage()));
        }
        catch (RepoException &re) {
            QMessageBox::warning(this, "Warning", QString::fromStdString(re.getErrorMessage()));
        }
    });
    QObject::connect(adaugaCos, &QPushButton::clicked, [&]() {
        try {
            std::string denumire = editDenumire->text().toStdString();
            std::string destinatie = editDestinatie->text().toStdString();
            editDenumire->clear();
            editDestinatie->clear();
            editTip->clear();
            editPret->clear();
            this->srv.cos_adauga(denumire, destinatie);
            this->reloadOfertaCosList(this->srv.get_all_cos());
            QMessageBox::information(this, "Info", QString::fromStdString("Oferta adaugata in cos cu succes!"));
        }
        catch (Exception &exception) {
            QMessageBox::warning(this, "Warning", QString::fromStdString(exception.getMessage()));
        }
        catch (RepoException &re) {
            QMessageBox::warning(this, "Warning", QString::fromStdString(re.getErrorMessage()));
        }
    });
    QObject::connect(btnFilterOferte1, &QPushButton::clicked, this, [&]() {
        try {
            int pret = std::stoi(editFilterPret->text().toStdString());
            this->reloadOfertaList(this->srv.filtrare_pret(pret));
            editFilterPret->clear();
        }
        catch (Exception &exception) {
            QMessageBox::warning(this, "Warning", QString::fromStdString(exception.getMessage()));
        }
        catch (RepoException &re) {
            QMessageBox::warning(this, "Warning", QString::fromStdString(re.getErrorMessage()));
        }
    });
    QObject::connect(tblV->selectionModel(), &QItemSelectionModel::selectionChanged, [&]() {
        if (tblV->selectionModel()->selectedIndexes().isEmpty())
        {
            editDenumire->setText("");
            editDestinatie->setText("");
            editTip->setText("");
            editPret->setText("");
        }
        int selRow = tblV->selectionModel()->selectedIndexes().at(0).row();
        auto cel0Index = tblV->model()->index(selRow, 0);
        auto cel0 = tblV->model()->data(cel0Index, Qt::DisplayRole).toString();
        editDenumire->setText(cel0);
        auto cel1Index = tblV->model()->index(selRow, 1);
        auto cel1 = tblV->model()->data(cel1Index, Qt::DisplayRole).toString();
        editDestinatie->setText(cel1);
        auto cel2Index = tblV->model()->index(selRow, 2);
        auto cel2 = tblV->model()->data(cel2Index, Qt::DisplayRole).toString();
        editTip->setText(cel2);
        auto cel3Index = tblV->model()->index(selRow, 3);
        auto cel3 = tblV->model()->data(cel3Index, Qt::DisplayRole).toString();
        editPret->setText(cel3);
    });
    QObject::connect(btnFilterOferte2, &QPushButton::clicked, this, [&]() {
        try {
            std::string destinatie = editFilterDestinatie->text().toStdString();
            this->reloadOfertaList(this->srv.filtrare_destinatie(destinatie));
            editFilterDestinatie->clear();
        }
        catch (Exception &exception) {
            QMessageBox::warning(this, "Warning", QString::fromStdString(exception.getMessage()));
        }
        catch (RepoException &re) {
            QMessageBox::warning(this, "Warning", QString::fromStdString(re.getErrorMessage()));
        }
    });
    QObject::connect(btnReloadData, &QPushButton::clicked, [&]() {
        this->reloadOfertaList(this->srv.get_all_oferte());
    });
    QObject::connect(btnAllInclusive, &QPushButton::clicked, [&]() {
        int cnt = 0;
        for (int i = 0; i < srv.get_all_oferte().size(); i++)
            if (srv.get_all_oferte()[i].get_tip() == "AllInclusive")
                cnt++;
        QMessageBox::information(this, "Info", QString::fromStdString(
                "Numarul de oferte All Inclusive este: " + std::to_string(cnt)));
    });
    QObject::connect(btnCityBreak, &QPushButton::clicked, [&]() {
        int cnt = 0;
        for (int i = 0; i < srv.get_all_oferte().size(); i++)
            if (srv.get_all_oferte()[i].get_tip() == "CityBreak")
                cnt++;
        QMessageBox::information(this, "Info",
                                 QString::fromStdString("Numarul de oferte City Break este: " + std::to_string(cnt)));
    });
    QObject::connect(btnHiking, &QPushButton::clicked, [&]() {
        int cnt = 0;
        for (int i = 0; i < srv.get_all_oferte().size(); i++)
            if (srv.get_all_oferte()[i].get_tip() == "Hiking")
                cnt++;
        QMessageBox::information(this, "Info",
                                 QString::fromStdString("Numarul de oferte Hiking este: " + std::to_string(cnt)));
    });
    QObject::connect(btnMultiCountry, &QPushButton::clicked, [&]() {
        int cnt = 0;
        for (int i = 0; i < srv.get_all_oferte().size(); i++)
            if (srv.get_all_oferte()[i].get_tip() == "MultiCountry")
                cnt++;
        QMessageBox::information(this, "Info", QString::fromStdString(
                "Numarul de oferte Multi Country este: " + std::to_string(cnt)));
    });
    QObject::connect(btnAltele, &QPushButton::clicked, [&]() {
        int cnt = 0;
        for (int i = 0; i < srv.get_all_oferte().size(); i++)
            if (srv.get_all_oferte()[i].get_tip() == "Altele")
                cnt++;
        QMessageBox::information(this, "Info",
                                 QString::fromStdString("Numarul de oferte Altele este: " + std::to_string(cnt)));
    });
    QObject::connect(btnCos, &QPushButton::clicked, [&]() {
        QWidget *fereastraCos = new QWidget;
        QFormLayout *lyCos = new QFormLayout;
        fereastraCos->setLayout(lyCos);
        denumireOfertaCos = new QLineEdit;
        lyCos->addRow("Denumire oferta", denumireOfertaCos);
        destinatieOfertaCos = new QLineEdit;
        lyCos->addRow("Destinatie oferta", destinatieOfertaCos);
        lyCos->addWidget(adaugaCos);
        lyCos->addRow(lblRandom, numarRandom);
        lyCos->addWidget(adaugaRandomCos);
        lyCos->addWidget(golesteCos);
        editExport = new QLineEdit;
        lyCos->addRow(lblexport, editExport);
        lyCos->addWidget(exportCos);
        lyCos->addWidget(listaCos);
        fereastraCos->show();
    });
    QObject::connect(adaugaRandomCos, &QPushButton::clicked, [&]() {
        try {
            int nr = std::stoi(numarRandom->text().toStdString());
            numarRandom->clear();
            this->srv.cos_adauga_random(nr);
            this->reloadOfertaCosList(this->srv.get_all_cos());
        }
        catch (Exception &exception) {
            QMessageBox::warning(this, "Warning", QString::fromStdString(exception.getMessage()));
        }
        catch (RepoException &re) {
            QMessageBox::warning(this, "Warning", QString::fromStdString(re.getErrorMessage()));
        }
    });
    QObject::connect(tblV->selectionModel(), &QItemSelectionModel::selectionChanged, [&]() {
        if (tblV->selectionModel()->selectedIndexes().isEmpty()) {
            editDenumire->setText("");
            editDestinatie->setText("");
            editTip->setText("");
            editPret->setText("");
            return;
        }
        int selRow = tblV->selectionModel()->selectedIndexes().at(0).row();
        auto cel0Index = tblV->model()->index(selRow, 0);
        auto cel0Value = tblV->model()->data(cel0Index, Qt::DisplayRole).toString();
        editDenumire->setText(cel0Value);
        auto cel1Index = tblV->model()->index(selRow, 1);
        auto cel1Value = tblV->model()->data(cel1Index, Qt::DisplayRole).toString();
        editDestinatie->setText(cel1Value);
        auto cel2Index = tblV->model()->index(selRow, 2);
        auto cel2Value = tblV->model()->data(cel2Index, Qt::DisplayRole).toString();
        editTip->setText(cel2Value);
        auto cel3Index = tblV->model()->index(selRow, 3);
        auto cel3Value = tblV->model()->data(cel3Index, Qt::DisplayRole).toString();
        editPret->setText(cel3Value);
    });

    QObject::connect(golesteCos, &QPushButton::clicked, [&]() {
        try {
            this->srv.cos_stergere();
            this->reloadOfertaCosList(this->srv.get_all_cos());
        }
        catch (Exception &exception) {
            QMessageBox::warning(this, "Warning", QString::fromStdString(exception.getMessage()));
        }
        catch (RepoException &re) {
            QMessageBox::warning(this, "Warning", QString::fromStdString(re.getErrorMessage()));
        }
    });

    QObject::connect(exportCos, &QPushButton::clicked, [&]() {
        try {
            std::string filename = editExport->text().toStdString();
            editExport->clear();
            this->srv.cos_export(filename);
            QMessageBox::information(this, "Info", QString::fromStdString("Export realizat cu succes!"));
        }
        catch (Exception &exception) {
            QMessageBox::warning(this, "Warning", QString::fromStdString(exception.getMessage()));
        }
        catch (RepoException &re) {
            QMessageBox::warning(this, "Warning", QString::fromStdString(re.getErrorMessage()));
        }
    });
    QObject::connect(btnClear, &QPushButton::clicked, [&]() {
        editDenumire->clear();
        editDestinatie->clear();
        editTip->clear();
        editPret->clear();
    });
    QObject::connect(btnCosCrud, &QPushButton::clicked, this, [&]()
    {
        auto fereastraCos = new CosCRUDGUI{this->srv.get_cos()};
        fereastraCos->show();
    });
    QObject::connect(btnCosReadOnly, &QPushButton::clicked, this, [&]()
    {
        auto fereastraFiguri = new HistogramaGUI{this->srv.get_cos()};
        fereastraFiguri->show();
    });
}

void IncercareGUI::reloadOfertaList(vector<Oferta> oferte) {
    modelTabel->setOferte(oferte);
    //modelTabel = new TableModel{ oferte };
    //tblV->setModel(modelTabel);

    /*
    int cnt = 0;
    for(int i=0; i<srv.get_all_oferte().size(); i++)
        if(srv.get_all_oferte()[i].get_tip()=="AllInclusive")
            cnt++;
    if (cnt==0)
        btnAllInclusive->setVisible(false);
    else
        btnAllInclusive->setVisible(true);

    cnt = 0;
    for(int i=0; i<srv.get_all_oferte().size(); i++)
        if(srv.get_all_oferte()[i].get_tip()=="CityBreak")
            cnt++;
    if (cnt==0)
        btnCityBreak->setVisible(false);
    else
        btnCityBreak->setVisible(true);

    cnt = 0;
    for(int i=0; i<srv.get_all_oferte().size(); i++)
        if(srv.get_all_oferte()[i].get_tip()=="Hiking")
            cnt++;
    if (cnt==0)
        btnHiking->setVisible(false);
    else
        btnHiking->setVisible(true);

    cnt = 0;
    for(int i=0; i<srv.get_all_oferte().size(); i++)
        if(srv.get_all_oferte()[i].get_tip()=="MultiCountry")
            cnt++;
    if (cnt==0)
        btnMultiCountry->setVisible(false);
    else
        btnMultiCountry->setVisible(true);

    cnt = 0;
    for(int i=0; i<srv.get_all_oferte().size(); i++)
        if(srv.get_all_oferte()[i].get_tip()=="Altele")
            cnt++;
    if (cnt==0)
        btnAltele->setVisible(false);
    else
        btnAltele->setVisible(true);
    */
}

void IncercareGUI::reloadOfertaCosList(vector<Oferta> oferte) {
    this->listaCos->clear();
    for(auto& oferta : oferte)
    {
        auto item = new QListWidgetItem(QString::fromStdString(oferta.get_denumire() + "  " + oferta.get_destinatie() + "  " + oferta.get_tip() + "  " + std::to_string(oferta.get_pret())));
        this->listaCos->addItem(item);
    }
}

void IncercareGUI::guiAddOferta() {
    try{
        std::string denumire = editDenumire->text().toStdString();
        std::string destinatie = editDestinatie->text().toStdString();
        std::string tip = editTip->text().toStdString();
        int pret = std::stoi(editPret->text().toStdString());

        editDenumire->clear();
        editDestinatie->clear();
        editTip->clear();
        editPret->clear();

        this->srv.adaugare_oferta(denumire, destinatie, tip, pret);
        this->reloadOfertaList(this->srv.get_all_oferte());
        QMessageBox::information(this, "Info", QString::fromStdString("Oferta adaugata cu succes!"));
    }
    catch (Exception &exception) {
        QMessageBox::warning(this, "Warning", QString::fromStdString(exception.getMessage()));
    }
    catch (RepoException &re) {
        QMessageBox::warning(this, "Warning", QString::fromStdString(re.getErrorMessage()));
    }
    catch (std::invalid_argument& e) {
        QMessageBox::warning(this, "Warning", "Pretul trebuie sa fie un numar intreg!");
    } catch (std::out_of_range& e) {
        QMessageBox::warning(this, "Warning", "Pretul este prea mare!");
    }
}

void IncercareGUI::guiModifyOferta() {
    try {
        std::string denumire = editDenumire->text().toStdString();
        std::string destinatie = editDestinatie->text().toStdString();
        std::string tip = editTip->text().toStdString();
        int pret = std::stoi(editPret->text().toStdString());

        editDenumire->clear();
        editDestinatie->clear();
        editTip->clear();
        editPret->clear();

        this->srv.modificare_oferta(denumire, destinatie, tip, pret);
        this->reloadOfertaList(this->srv.get_all_oferte());
        QMessageBox::information(this, "Info", QString::fromStdString("Oferta modificata cu succes!"));
    }
    catch (Exception &exception) {
        QMessageBox::warning(this, "Warning", QString::fromStdString(exception.getMessage()));
    }
    catch (RepoException &re) {
        QMessageBox::warning(this, "Warning", QString::fromStdString(re.getErrorMessage()));
    }
}

void IncercareGUI::guiDeleteOferta() {
    try {
        std::string denumire = editDenumire->text().toStdString();
        std::string destinatie = editDestinatie->text().toStdString();

        editDenumire->clear();
        editDestinatie->clear();
        editTip->clear();
        editPret->clear();

        Oferta oferta_de_sters = this->srv.cautare_oferte(denumire, destinatie);

        this->srv.stergere_oferta(oferta_de_sters);
        this->reloadOfertaList(this->srv.get_all_oferte());
        QMessageBox::information(this, "Info", QString::fromStdString("Oferta stearsa cu succes!"));
    }
    catch (Exception &exception) {
        QMessageBox::warning(this, "Warning", QString::fromStdString(exception.getMessage()));
    }
    catch (RepoException &re) {
        QMessageBox::warning(this, "Warning", QString::fromStdString(re.getErrorMessage()));
    }
}

void IncercareGUI::fillForm(QTableWidgetItem* item)
{
    int row = item->row();

    editDenumire->setText(tableOferta->item(row, 0)->text());
    editDestinatie->setText(tableOferta->item(row, 1)->text());
    editTip->setText(tableOferta->item(row, 2)->text());
    editPret->setText(tableOferta->item(row, 3)->text());
}

void IncercareGUI::parcurgereMapButoane() {
    for (const auto& element : oferteMap) {
        QPushButton* buttonType = new QPushButton(QString::fromStdString(element.first));
        connect(buttonType, &QPushButton::clicked, this, [=]() {
            int count = oferteMap.at(element.first);
            QString message = "There are " + QString::number(count) + " products of type " + buttonType->text();
            QMessageBox::information(this, "Info", message);
        });
        buttonType->setFixedWidth(223);
    }
}
