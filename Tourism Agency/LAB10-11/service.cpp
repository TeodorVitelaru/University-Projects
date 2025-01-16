#include <iostream>
#include <algorithm>
#include <random>
#include <fstream>
#include <unordered_map>
#include "service.h"
#include "errors.h"

using namespace std;

void Service::adaugare_oferta(std::string denumire, std::string destinatie, std::string tip, int pret) {
    try {
        Oferta of{denumire, destinatie, tip, pret};
        val.validateOferta(destinatie, destinatie, tip, pret);
        repo.store(of);
        undoActions.push_back(make_unique<UndoAdauga>(repo, of));
    }
    catch (Exception& e) {
        cout<<"aici";
        throw e;
    }
}

void Service::modificare_oferta(const std::string &denumire, const std::string &destinatie, const std::string &tip,
                                int pret) {
    val.validateOferta(destinatie, destinatie, tip, pret);
    Oferta new_of{denumire, destinatie, tip, pret};
    Oferta old_of = repo.find(denumire, destinatie);
    repo.modify(new_of);
    undoActions.push_back(make_unique<UndoModifica>(repo, old_of, new_of));
}

void Service::stergere_oferta(const Oferta& of) {
    repo.del(of);
    undoActions.push_back(make_unique<UndoSterge>(repo, of));
}

Oferta Service::cautare_oferte(const std::string &denumire, const std::string &destinatie) {
    return repo.find(denumire, destinatie);
}

vector<Oferta> Service::sortare_denumire() {
    vector<Oferta> sortedCopy{repo.get_all_oferte()};
    sort(sortedCopy.begin(), sortedCopy.end(), cmp_denumire);
    return sortedCopy;
}

vector<Oferta> Service::sortare_destinatie() {
    vector<Oferta> sortedCopy{repo.get_all_oferte()};
    sort(sortedCopy.begin(), sortedCopy.end(), cmp_destinatie);
    return sortedCopy;
}

vector<Oferta> Service::sortare_tipPret() {
    vector<Oferta> sortedCopy{repo.get_all_oferte()};
    sort(sortedCopy.begin(), sortedCopy.end(), cmp_tip_pret);
    return sortedCopy;
}

vector<Oferta> Service::filtrare_destinatie(const std::string &destinatie) {
    vector<Oferta> new_vec;
    const vector<Oferta>& v = get_all_oferte();
    copy_if(v.begin(), v.end(), back_inserter(new_vec), [destinatie](const Oferta& of){return of.get_destinatie() == destinatie;});
    return new_vec;
}

vector<Oferta> Service::filtrare_pret(const int &pret) {
    vector<Oferta> new_vec;
    const vector<Oferta>& v = get_all_oferte();
    copy_if(v.begin(), v.end(), back_inserter(new_vec), [pret](const Oferta& of){return of.get_pret() <= pret;});
    return new_vec;
}


void Service::cos_adauga(const std::string &denumire, const std::string &destinatie) {
    Oferta of = repo.find(denumire, destinatie);
    cos.adauga_oferta_cos(of);
}

void Service::cos_stergere() {
    cos.sterge_cos();
}

int Service::cos_adauga_random(int nr) {
    cos.adauga_random_cos(repo.get_all_oferte(), nr);
    return cos.get_all_cos().size();
}

void Service::cos_export(const std::string &filename) {
    if(filename.find(".csv") == string::npos && filename.find(".html") == string::npos)
        throw Exception("Fisierul nu este valid!");
    else
    {
        ofstream fout(filename);
        if(filename.find(".html") != string::npos)
        {
            fout<<"<html>";
            fout<<"<style> table, th, td {border: 1px solid black;} body{background-color: #E6E6FA;}</style>";
            fout<<"<body>";
            fout<<"<h1>COS OFERTE</h1>";
            fout << "<table><tr><th>Denumire</th> <th>Destinatie</th> <th>Tip</th> <th>Pret</th></tr>";
            for (auto &o: get_all_cos()) {
                fout << "<tr><td>" << o.get_denumire() << "<td>" << o.get_destinatie() << "</td><td>" << o.get_tip()
                     << "</td><td>" << o.get_pret() << "</td></tr>";
            }
            fout << "</table></body>";
            fout << "<html>";
        } else
        {
            fout<<"Denumire, Destinatie, Tip, Pret"<<endl;
            for (auto &o: get_all_cos()) {
                fout << o.get_denumire() << "," << o.get_destinatie() << "," << o.get_tip() << "," << o.get_pret() << endl;
            }

        }
        fout.close();
    }
}

void Service::undo() {
    if (undoActions.empty())
        throw Exception("Nu se mai poate face undo!");
    undoActions.back()->doUndo();
    undoActions.pop_back();
}

const vector<Oferta>& Service::get_all_cos() {
    return cos.get_all_cos();
}

unordered_map<string, int> Service::creare_dict() {
    unordered_map<string, int> asd;
    vector<Oferta> v = this->get_all_oferte();
    for (const auto &i: v) {
        if (asd.find(i.get_tip()) != asd.end())
            asd[i.get_tip()]++;
        else
            asd[i.get_tip()] = 1;
    }
    return asd;
}
