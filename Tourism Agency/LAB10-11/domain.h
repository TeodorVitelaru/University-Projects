#pragma once
#include <string>
#include <iostream>
using namespace std;

class Oferta
{
private:
    string denumire{ "" };
    string destinatie{ "" };
    string tip{ "" };
    int pret{ 0 };
public:
    Oferta() = default;
    /*
     * Constructorul clasei produs
     * denumire, denumirea produsului
     * destinatia, destinatia ofertei
     * tip, tipul ofertei
     * pret, pretul ofertei
     */
    Oferta(string denumire, string destinatie, string tip, int pret) :denumire{ denumire }, destinatie{ destinatie }, tip{ tip }, pret{ pret }{};

    Oferta(const Oferta& ot)
    {
        denumire = ot.denumire;
        destinatie = ot.destinatie;
        tip = ot.tip;
        pret = ot.pret;
    }

    /*
     * Functiile de get pentru fiecare parametru
     * al produsului
     */
    string get_denumire() const;
    string get_destinatie() const;
    string get_tip() const;
    int get_pret() const;

    /*
     * Functiile de set pentru fiecare parametru
     * al produsului
     */
    void set_denumire(string _denumire);
    void set_destinatie(string _destinatie);
    void set_tip(string _tip);
    void set_pret(int _pret);

    /*
     * Functie pentru afisare a ofertei
     */
    friend ostream& operator<<(ostream& stream, const Oferta& of);

    bool cmp_denumire(const Oferta& of1, const Oferta& of2)
    {
        if(of1.get_denumire() > of2.get_denumire())
            return 1;
        else
            return 0;
    }

    bool cmp_destinatie(const Oferta& of1, const Oferta& of2)
    {
        if(of1.get_destinatie() > of2.get_destinatie())
            return 1;
        else
            return 0;
    }

    bool cmp_tip_pret(const Oferta& of1, const Oferta& of2) {
        if (of1.get_tip() > of2.get_tip()) {
            return 1;
        } else if(of1.get_tip() == of2.get_tip())
        {
            if(of1.get_pret() > of2.get_pret())
                return 1;
            return 0;
        }
        return 0;
    }
};


