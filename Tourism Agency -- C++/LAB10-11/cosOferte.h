#pragma once
#include "domain.h"
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include "repo.h"
#include "observer.h"

class cosOferte : public Observable
{
private:
    vector<Oferta> cos;
    const RepoFile& repo;
public:
    /*
     * Constructorul clasei cosOferte
     */
    cosOferte() = default;

    cosOferte(const RepoFile& repo) :repo{ repo } {};

    /*
     * Functie de adaugare a unei oferte in cos
     * of: obiect de tip Oferta
     * return: -
     */
    void adauga_oferta_cos(const Oferta& of);

    /*
     * Functie de adaugare a unui numar random de oferte in cos
     * of: vector de oferte
     * numar: numarul de oferte care se adauga
     * return: -
     */
    void adauga_random_cos(vector<Oferta>, int numar);

    /*
     * Functie de stergere a tuturor ofertelor din cos
     * return: -
     */
    void sterge_cos();

    /*
     * Functie de returnare a tuturor ofertelor din cos
     * return: vector de oferte
     */
    const vector<Oferta>& get_all_cos();

    /*
     * Destructorul clasei cosOferte
     */
    ~cosOferte() = default;

    void umple(int count)
    {
        int seed = std::chrono::system_clock::now().time_since_epoch().count();
        vector<Oferta> all = repo.get_all_oferte();
        shuffle(all.begin(), all.end(), default_random_engine(seed));
        while(cos.size() < count && !all.empty())
        {
            cos.push_back(all.back());
            all.pop_back();
        }
        notify();
    }
};