#include <iostream>
#include <fstream>
#include "repo.h"
using namespace std;

bool Repo::exists(const Oferta &oferta) {
    try{
        find(oferta.get_denumire(), oferta.get_destinatie());
        return true;
    }
    catch (RepoException&)
    {
        return false;
    }
}

const Oferta& Repo::find(std::string denumire, std::string destinatie) {
    for(const Oferta& oferta : this->allOferte)
    {
        if(oferta.get_denumire() == denumire && oferta.get_destinatie() == destinatie)
            return oferta;
    }
    throw RepoException("Oferta cu denumirea "+denumire+" si destinatia "+destinatie+" nu exista in lista");
}

void Repo::store(const Oferta &oferta) {
    if(exists(oferta))
        throw RepoException("Oferta cu aceaste denumire si acest titlu exista deja!");
    this->allOferte.push_back(oferta);
}


void Repo::del(const Oferta& of) {
    int i = 0;
    while (i < this->allOferte.size()) {
        if (allOferte.at(i).get_denumire() == of.get_denumire() && allOferte.at(i).get_destinatie() == of.get_destinatie()) {
            this->allOferte.erase(this->allOferte.begin() + i);
            break;
        }
        i++;
    }
}

void Repo::modify(const Oferta &of) {
    for(int i=0; i<allOferte.size(); i++)
        if(allOferte[i].get_denumire() == of.get_denumire() && allOferte[i].get_destinatie() == of.get_destinatie())
        {
            allOferte[i].set_tip(of.get_tip());
            allOferte[i].set_pret(of.get_pret());
        }

}

const vector<Oferta>& Repo::get_all_oferte() const {
    return this->allOferte;
}

void RepoFile::loadFromFile() {
    ifstream in(filename);
    if (!in.is_open())
        throw RepoException("Eroare la deschiderea fisierului!!!");
    while(!in.eof())
    {
        string denumire;
        string destinatie;
        string tip;
        int pret;
        if (in.eof()) break;
        in>>denumire;
        if (in.eof()) break;
        in>>destinatie;
        if (in.eof()) break;
        in>>tip;
        if (in.eof()) break;
        in>>pret;
        Oferta of{denumire, destinatie, tip, pret};
        Repo::store(of);
    }
}

void RepoFile::writeToFile() {
    ofstream out(filename);
    if (!out.is_open())
        throw RepoException("Eroare la deschiderea fisierului!!!");
    for(const Oferta& of : this->get_all_oferte())
    {
        out<<of.get_denumire()<<endl;
        out<<of.get_destinatie()<<endl;
        out<<of.get_tip()<<endl;
        out<<of.get_pret()<<endl;
    }
}



