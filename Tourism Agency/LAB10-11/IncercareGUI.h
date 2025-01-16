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
#include "service.h"
#include "TableModel.h"

class IncercareGUI : public QWidget
{
private:
    Service& srv;
    TableModel* modelTabel;
    QTableView* tblV = new QTableView;
    QPushButton* btnCosCrud = new QPushButton("Cos CRUD");
    QPushButton* btnCosReadOnly = new QPushButton("Cos ReadOnly");


    unordered_map<string, int> oferteMap = this->srv.creare_dict();
    QLabel* lblDenumire = new QLabel{ "Denumire" };
    QLabel* lblDestinatie = new QLabel{ "Destinatie" };
    QLabel* lblTip = new QLabel{ "Tip" };
    QLabel* lblPret = new QLabel{ "Pret" };

    QLineEdit* editDenumire;
    QLineEdit* editDestinatie;
    QLineEdit* editTip;
    QLineEdit* editPret;

    QPushButton* btnAddOferta;
    QPushButton* btnModifyOferta;
    QPushButton* btnDeleteOferta;

    QGroupBox* groupBox = new QGroupBox(tr("Tipe sortare"));

    QRadioButton* radioSrtDenumire = new QRadioButton(QString::fromStdString("Denumire"));
    QRadioButton* radioSrtDestinatie = new QRadioButton(QString::fromStdString("Destinatie"));
    QRadioButton* radioSrtTipPret = new QRadioButton(QString::fromStdString("Tip si pret"));
    QPushButton* btnSortOferte;

    QLabel* lblFilterCriteria = new QLabel{ "Pretul dupa care se filtreaza:" };
    QLineEdit* editFilterPret;
    QPushButton* btnFilterOferte1;

    QLabel* lblFilterCriteria2 = new QLabel{ "Destinatia dupa care se filtreaza:" };
    QLineEdit* editFilterDestinatie;
    QPushButton* btnFilterOferte2;

    QPushButton* btnReloadData;

    QPushButton* btnAllInclusive;
    QPushButton* btnCityBreak;
    QPushButton* btnHiking;
    QPushButton* btnMultiCountry;
    QPushButton* btnAltele;

    QPushButton* btnUndo;
    QPushButton* btnClear;

    //pentru tabel
    QTableWidget* tableOferta;
    //pentru lista
    QListWidget* listaOferte;

    QPushButton* btnCos;
    QPushButton* adaugaCos;
    QLineEdit* denumireOfertaCos;
    QLineEdit* destinatieOfertaCos;
    QPushButton* adaugaRandomCos;
    QLineEdit* numarRandom;
    QPushButton* golesteCos;
    QListWidget* listaCos;
    QLineEdit* editExport;
    QPushButton* exportCos;
    QLabel* lblRandom = new QLabel{ "Numar de oferte random" };
    QLabel* lblexport = new QLabel{ "Numele fisierului" };

    void initializeGUIComponents();

    void connectSignalsSlots();
    void reloadOfertaList(vector<Oferta> oferte);
    void reloadOfertaCosList(vector<Oferta> oferte);
public:
    IncercareGUI(Service& srv) : srv{ srv }
    {
        modelTabel = new TableModel{ srv.get_all_oferte() };
        tblV->setModel(modelTabel);
        initializeGUIComponents();
        connectSignalsSlots();
        //reloadOfertaList(srv.get_all_oferte());
    }
    void fillForm(QTableWidgetItem* item);
    void guiAddOferta();
    void guiModifyOferta();
    void guiDeleteOferta();
    void parcurgereMapButoane();
};



