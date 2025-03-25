#include "domain.h"
#include "repo.h"

class ActiuneUndo
{
    public:
    virtual void doUndo() = 0;
    virtual ~ActiuneUndo() {};
};

class UndoAdauga : public ActiuneUndo
{
    Oferta of;
    RepoAbstract &repo;
    public:
    UndoAdauga(RepoAbstract &repo, const Oferta& of) : repo{repo}, of{of}{};
    /*
     * Functie de undo pentru adaugare
     */
    void doUndo() override
    {
        repo.del(of);
    }
};

class UndoModifica : public ActiuneUndo
{
    Oferta of_veche, of_noua;
    RepoAbstract &repo;
    public:
    /*
     * Functie de undo pentru modificare
     * repo: repo ul in care se face undo
     * of_veche: oferta veche
     * of_noua: oferta noua
     * return: -
     */
    UndoModifica(RepoAbstract &repo, const Oferta& of_veche, const Oferta& of_noua) : repo{repo}, of_veche{of_veche},of_noua{of_noua}{};

    /*
     * Functie de undo pentru modificare
     */
    void doUndo() override
    {
        repo.modify(of_veche);
    }
};

class UndoSterge : public ActiuneUndo
{
    Oferta of;
    RepoAbstract &repo;
    public:
    UndoSterge(RepoAbstract &repo, const Oferta& of) : repo{repo}, of{of} {};
    /*
     * Functie de undo pentru stergere
     */
    void doUndo() override
    {
        repo.store(of);
    }
};