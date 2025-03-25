#include <iostream>
#include <string>
#include "ui.h"

using namespace std;

void Consola::print_menu() {
    cout<<"\nMENIU\n";
    cout<<"1.Aduagare oferta\n";
    cout<<"2.Stergere oferta\n";
    cout<<"3.Modificare oferta\n";
    cout<<"4.Printare oferte\n";
    cout<<"5.Cautare oferta\n";
    cout<<"6.Filtrare oferte\n";
    cout<<"7.Sortare oferte\n";
    cout<<"8.MENIU COS\n";
    cout<<"9.Exit\n";
    cout<<"10.UNDO\n";
}

void Consola::adaugare_ui() {
    string denumire, destinatie, tip;
    char pr[100];
    int pret;
    cout<<"Denumirea: ";
    cin>>denumire;
    cout<<"Destinatia: ";
    cin>>destinatie;
    cout<<"Tipul: ";
    cin>>tip;
    cout<<"Pretul: ";
    cin>>pr;
    pret = atoi(pr);
    try{
        srv.adaugare_oferta(denumire, destinatie, tip, pret);
        cout<<"Oferta a fost adaugata cu succes!"<<endl;
    }
    catch (RepoException& re)
    {
        cout<< re.getErrorMessage();
    }
    catch (Exception exception)
    {
        cout<<exception.getMessage();
    }
}

void Consola::modificare_ui()
{
    string denumire, destinatie, tip_nou;
    char pr[100];
    int pret_nou;
    cout<<"Denumirea ofertei pe care vreti sa o schimbati: ";
    cin>>denumire;
    cout<<"Destinatia ofertei pe care vreti sa o schimbati: ";
    cin>>destinatie;
    cout<<"Tipul nou: ";
    cin>>tip_nou;
    cout<<"Pretul nou: ";
    cin>>pr;
    pret_nou = atoi(pr);
    try {
        srv.modificare_oferta(denumire, destinatie, tip_nou,pret_nou);
    }
    catch (Exception exception)
    {
        cout<<exception.getMessage();
    }
    catch (RepoException& re)
    {
        cout<< re.getErrorMessage();
    }
}

void Consola::stergere_ui() {
    int poz;
    cout<<"Introduceti pozitia: ";
    cin>>poz;
    if(poz<0 || poz>=srv.get_all_oferte().size())
        cout<<"Introduceti o pozitie valida!"<<endl;
    Oferta of_to_remove = srv.get_all_oferte().at(poz);
    try{
        srv.stergere_oferta(of_to_remove);
    }
    catch (Exception exception)
    {
        cout<<exception.getMessage();
    }
    catch (RepoException& re)
    {
        cout<< re.getErrorMessage();
    }
}

void Consola::cautare_ui() {
    string denumire, destinatie;
    cout<<"Denumirea: ";
    cin>>denumire;
    cout<<"Destinatia: ";
    cin>>destinatie;
    try{
       cout<<srv.cautare_oferte(denumire,destinatie);
    }
    catch (Exception exception)
    {
        cout<<exception.getMessage();
    }
    catch (RepoException& re)
    {
        cout<< re.getErrorMessage();
    }
}

void Consola::filtrare_ui() {
    int nr;
    cout<<"1.destinatie   2.pret"<<endl;
    cout<<"alegeti o optiune: ";
    cin>>nr;
    if(nr == 1)
    {
        string destinatia;
        cout<<"Dati destinatia pentru care vreti sa vedeti ofertele: ";
        cin>>destinatia;
        print_all_oferte(srv.filtrare_destinatie(destinatia));
    }
    else if(nr == 2)
    {
        int pret;
        cout<<"Dati pretul pentru care vreti sa vedeti ofertele: ";
        cin>>pret;
        print_all_oferte(srv.filtrare_pret(pret));
    }
}

void Consola::sortare_ui() {
    if(!srv.get_all_oferte().empty())
    {
        string crit;
        cout << "Introduceti criteriul dupa care doriti sa faceti sortarea(1-denumire,2-destinatie,3-tip+pret): ";
        cin >> crit;
        if(crit == "1")
        {
            srv.sortare_denumire();
        } else if(crit == "2")
        {
            srv.sortare_destinatie();
        } else if(crit == "3")
        {
            srv.sortare_tipPret();
        }
        else{cout<<"Criteriu invalid!"<<endl;}
    }
    else{cout<<"Lista este goala.NU se pot face sortari!!"<<endl;}
}

void Consola::cos_adaugare_ui() {
    string denumire, destinatie;
    cout<<"Denumirea ofertei pe care vreti sa o adaugati in cos: ";
    cin>>denumire;
    cout<<"Destinatia ofertei pe care vreti sa o adaugati in cos: ";
    cin>>destinatie;
    try{
        srv.cos_adauga(denumire, destinatie);
    }
    catch (Exception exception)
    {
        cout<<exception.getMessage();
    }
    catch (RepoException& re)
    {
        cout<< re.getErrorMessage();
    }
}

void Consola::cos_adaugare_random_ui() {
    int nr;
    cout<<"Dati numarul de oferte random pe care vreti sa le adaugati in cos: ";
    cin>>nr;
    try{
        cout<<"S-au adaugat "<<srv.cos_adauga_random(nr)<<" oferte in cos"<<endl;
    }
    catch (Exception exception)
    {
        cout<<exception.getMessage();
    }
    catch (RepoException& re)
    {
        cout<< re.getErrorMessage();
    }
}

void Consola::cos_stergere_ui() {
    try{
        srv.cos_stergere();
        cout<<"Cosul a fost golit!"<<endl;
    }
    catch (Exception exception)
    {
        cout<<exception.getMessage();
    }
    catch (RepoException& re)
    {
        cout<< re.getErrorMessage();
    }
}

void Consola::cos_export_ui() {
    string filename;
    cout<<"Dati numele fisierului in care doriti sa exportati cosul: ";
    cin>>filename;
    try{
        string referinta = "../";
        srv.cos_export(referinta.append(filename));
        cout<<"Cosul a fost exportat cu succes!"<<endl;
    }
    catch (Exception exception)
    {
        cout<<exception.getMessage();
    }
    catch (RepoException& re)
    {
        cout<< re.getErrorMessage();
    }

}


void Consola::cos_ui() {
    while (true)
    {
        int cmd;
        cout<<"\nCOSUL DUMNEAVOASTARA:\n";
        cout<<"Exista "<<srv.get_all_cos().size()<<" oferte in cos\n";
        cout<<"MENIU COS\n";
        cout<<"1.Adaugare oferta in cos\n";
        cout<<"2.Stergere cos\n";
        cout<<"3.Adaugare random in cos\n";
        cout<<"4.Printare cos\n";
        cout<<"5.Export\n";
        cout<<"6.Exit\n";
        cout<<"Comanda este:";
        cin>>cmd;
        try
        {
            switch (cmd) {
                case 1:
                    cos_adaugare_ui();
                    break;
                case 2:
                    cos_stergere_ui();
                    break;
                case 3:
                    cos_adaugare_random_ui();
                    break;
                case 4:
                    print_all_oferte(srv.get_all_cos());
                    break;
                case 5:
                    cos_export_ui();
                    break;
                case 6:
                    cout<<"Iesire din cos"<<endl;
                    return;
                default:
                    cout<<"comanda invalida!";
                    break;
            }
        }
        catch (Exception exception)
        {
            cout<<exception.getMessage();
        }
        catch (RepoException& re)
        {
            cout<< re.getErrorMessage();
        }
    }
}

void Consola::add_default_oferte() {
    for (int i = 1; i <= 2; i++) {
        try{
            std::string denumire = "asd" + to_string(65+i);
            std::string destinatie = "asd" + to_string(65+i);
            std::string tip = "asd" + to_string(65+i);
            int pret = i * 10;
            srv.adaugare_oferta(denumire, destinatie, tip, pret);
        }
        catch (Exception exception)
        {
            cout<<exception.getMessage();
        }
        catch (RepoException& re)
        {
            cout<< re.getErrorMessage();
        }

    }
}

void Consola::undo_ui() {
    try{
        srv.undo();
        cout<<"Undo efectuat cu succes!"<<endl;
    }
    catch (Exception exception)
    {
        cout<<exception.getMessage();
    }
    catch (RepoException& re)
    {
        cout<< re.getErrorMessage();
    }
}

void Consola::print_all_oferte(vector<Oferta> allOferte) {
    if(allOferte.size() == 0)
        cout<< "Nu exista oferte"<<endl;
    else{
        int cnt=0;
        for(const auto& oferta : allOferte)
        {
            cout<<cnt<<"."<<oferta;
            cnt++;
        }
    }
}

void Consola::run() {
    int running = 1;
    int cmd;
    //this->add_default_oferte();
    while (running)
    {
        print_menu();
        cout<<"Comanda este:";
        cin>>cmd;
        switch (cmd)
        {   case 1:
                adaugare_ui();
                break;
            case 9:
                running = 0;
                break;
            case 4:
                print_all_oferte(srv.get_all_oferte());
                break;
            case 3:
                modificare_ui();
                break;
            case 2:
                stergere_ui();
                break;
            case 5:
                cautare_ui();
                break;
            case 6:
                filtrare_ui();
                break;
            case 7:
                sortare_ui();
                break;
            case 8:
                cos_ui();
                break;
            case 10:
                undo_ui();
                break;
            default:
                break;

        }
    }
}