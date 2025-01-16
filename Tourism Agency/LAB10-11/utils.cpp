#include "utils.h"



int cmp_denumire(const Oferta& of1, const Oferta& of2)
{
    if(of1.get_denumire() > of2.get_denumire())
        return 1;
    else
        return 0;
}

int cmp_destinatie(const Oferta& of1, const Oferta& of2)
{
    if(of1.get_destinatie() > of2.get_destinatie())
        return 1;
    else
        return 0;
}

int cmp_tip_pret(const Oferta& of1, const Oferta& of2) {
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