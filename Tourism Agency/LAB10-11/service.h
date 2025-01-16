#pragma once
#include <iostream>
#include "repo.h"
#include "validator.h"
#include "cosOferte.h"
#include <map>
#include <unordered_map>
#include "undo.h"
#include <memory>
using namespace std;

typedef int(*Sort)(const Oferta&, const Oferta&);

class Service
{
private:
    RepoAbstract& repo;
    Validator& val;
    cosOferte& cos;
    vector<unique_ptr<ActiuneUndo>> undoActions;
public:
    /*
     * Constructorul serivce ului
     * oferta_repo: obiect de tip repo
     * val: validatorul
     */
    Service(RepoAbstract& oferta_repo, Validator& val, cosOferte& cos) :repo{ oferta_repo }, val{ val }, cos{ cos }{};

    Service(const Service& ot) = delete;

    /*
     * functie de adaugare a unei oferte
     * denumire: string, denumirea ofertei
     * destinatie: string, destinatia ofertei
     * tip: string, tipul ofertei
     * pret: int, pretul ofertei
     * return: -
     */
    void adaugare_oferta(string denumire, string destinatie, string tip, int pret);

    /*
     * functie de stergere a unei oferte
     * poz: pozitia ofertei care se sterge
     * return: -
     */
    void stergere_oferta(const Oferta& of);

    /*
     * Functie de modificare a ofertei
     * denumire: string, denumirea ofertei de modificat
     * destinatie: string, destinatia ofertei de modificat
     * tip: string, tipul noii oferte
     * pret: int, pretul noii oferte
     * return: -
     */
    void modificare_oferta(const string& denumire, const string& destinatie, const string& tip, int pret);

    /*
     * functie de cautare a unei oferte
     * denumire: string, denumirea dupa care se cauta
     * destinatie: string, destinatia dupa care se cauta
     * return: oferta gasita
     */
    Oferta cautare_oferte(const string& denumire, const string& destinatie);

    /*
     * functie de sortare generica
     * (*fct)(const Oferta&, const Oferta&): functie de comparare
     * ord: modul in care se sorteaza
     * return: -
     */
    vector<Oferta> sortare_destinatie();
    vector<Oferta> sortare_denumire();
    vector<Oferta> sortare_tipPret();

    /*
     * functie de filtrare dupa destinatie
     * destinatie: string, destinatia dupa care se filtreaza
     * return: o lista de oferta care au aceeasi destinatie cu cea data
     */
    vector<Oferta> filtrare_destinatie(const string& destinatie);

    /*
     * functie de filtrare dupa pret
     * pret: int, pretul dupa care se filtreaza
     * return: o lista de oferta care au aceeasi destinatie cu cea data
     */
    vector<Oferta> filtrare_pret(const int& pret);

    /*
     * functie de returnare a listei de oferte
     * return: lista de oferte
     */
    const vector<Oferta>& get_all_oferte()
    {
        return this->repo.get_all_oferte();
    }

    /*
     * functie de adaugare a unei oferte in cos
     * denumire: string, denumirea ofertei
     * destinatie: string, destinatia ofertei
     * return: -
     */
    void cos_adauga(const string& denumire, const string& destinatie);

    /*
     * functie de stergere a tuturor ofertelor din cos
     * return: -
     */
    void cos_stergere();

    /*
     * functie de adaugare a unui numar random de oferte in cos
     * numar: numarul de oferte care se adauga
     * return: numarul de oferte adaugate
     */
    int cos_adauga_random(int nr);

    /*
     * functie de export a cosului in fisier
     * filename: string, numele fisierului
     * return: -
     */
    void cos_export(const string& filename);

    void undo();

    /*
     * functie de returnare a listei de oferte din cos
     * return: lista de oferte
     */
    const vector<Oferta>& get_all_cos();
    unordered_map<string, int> creare_dict();

    cosOferte& get_cos()
    {
        return cos;
    }
};
