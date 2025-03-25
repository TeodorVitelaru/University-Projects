#include <iostream>
#include <assert.h>
#include "teste.h"
#include <map>
#include <unordered_map>


void test_domain()
{
     Oferta o1{ "vacanta", "Suceava", "munte", 1400 };
     assert(o1.get_destinatie() == "Suceava");
     assert(o1.get_denumire() == "vacanta");
     assert(o1.get_tip() == "munte");
     assert(o1.get_pret() == 1400);
     o1.set_pret(100);
     o1.set_tip("mare");
     o1.set_denumire("aici");
     o1.set_destinatie("nuaici");
    assert(o1.get_destinatie() == "nuaici");
    assert(o1.get_denumire() == "aici");
    assert(o1.get_tip() == "mare");
    assert(o1.get_pret() == 100);
    cout<<o1;
}

void test_store()
{
    Repo repo;
    repo.store(Oferta{"La mare", "Mamaia", "all-inclusive", 2345});
    assert(repo.get_all_oferte().size()==1);
    repo.store(Oferta("Munte", "Sinaia", "all-inclusive", 1280));
    assert(repo.get_all_oferte().size()==2);
    try {
        repo.store(Oferta("La mare", "Mamaia", "all-inclusive", 2345));assert(false);
    }
    catch (RepoException& re)
    {
        assert(re.getErrorMessage() == "Oferta cu aceaste denumire si acest titlu exista deja!");
    }
}

void test_find()
{
    Oferta of1{"La mare", "Mamaia", "all-inclusive", 2345};
    Repo repo;
    try{
        repo.find("aici", "aici");
        //assert(false);
    } catch (RepoException& ex)
    {
        assert(true);
    }
    repo.store(of1);
    repo.find("La mare", "Mamaia");
}

void test_del()
{
    Repo repo;
    repo.store(Oferta("La mare", "Mamaia", "all-inclusive", 2345));
    assert(repo.get_all_oferte().size()==1);
    Oferta o1 = Oferta("La mare", "Mamaia", "all-inclusive", 2345);
    repo.del(o1);
    assert(repo.get_all_oferte().size()==0);
}

void test_modify()
{
    Repo repo;
    repo.store(Oferta("La mare", "Mamaia", "all-inclusive", 2345));
    assert(repo.get_all_oferte()[0].get_pret() == 2345);
    Oferta of1{"La mare", "Mamaia", "munte", 1400};
    repo.modify(of1);
    assert(repo.get_all_oferte()[0].get_pret() == 1400);
}

void test_adaugare_service()
{
    RepoFile repo{"../test.txt"};
    Validator valid;
    cosOferte cos{repo};
    Service service{repo, valid, cos};
    assert(service.get_all_oferte().size()==0);
    service.adaugare_oferta("La Mare", "Mamaia", "allinclusive", 2345);
    assert(service.get_all_oferte().size()==1);
    service.adaugare_oferta("100", "nuj", "fgfg", 123);
    assert(true);
    service.stergere_oferta(service.cautare_oferte("La Mare", "Mamaia"));
    service.stergere_oferta(service.cautare_oferte("100", "nuj"));
}

void test_stergere_service()
{
    RepoFile repo{"../test.txt"};
    Validator valid;
    cosOferte cos{repo};
    Service service{repo, valid, cos};
    repo.store(Oferta("La mare", "Mamaia", "all-inclusive", 2345));
    assert(repo.get_all_oferte().size()==1);
    //service.stergere_oferta(1);
    assert(repo.get_all_oferte().size()==1);
    Oferta of=repo.get_all_oferte()[0];
    service.stergere_oferta(of);
    assert(0 == service.get_all_oferte().size());
}

void test_modificare_service()
{
    RepoFile repo{"../test.txt"};
    Validator valid;
    cosOferte cos{repo};
    Service service{repo, valid, cos};
    service.adaugare_oferta("La mare", "Mamaia", "allinclusive", 2345);
    assert(repo.get_all_oferte().size()==1);
    service.modificare_oferta("La mare", "Mamaia", "trei zile", 1000);
    Oferta o = service.cautare_oferte("La mare", "Mamaia");
    assert(o.get_tip()=="trei zile");
    assert(o.get_pret()==1000);
    service.stergere_oferta(o);
    assert(service.get_all_oferte().size()==0);
}

void test_filtrare_dest()
{
    RepoFile repo{"../test.txt"};
    Validator valid;
    cosOferte cos{repo};
    Service service{repo, valid, cos};
    repo.store(Oferta("La mare", "Mamaia", "all-inclusive", 2345));
    assert(repo.get_all_oferte().size()==1);
    repo.store(Oferta("Plaja", "Mamaia", "-", 400));
    repo.store(Oferta("Munte", "Sinaia", "all-inclusive", 1234));
    vector<Oferta> listaFiltrata1 = service.filtrare_destinatie("Mamaia");
    assert(listaFiltrata1.size()==2);
    assert(listaFiltrata1[0].get_pret()==2345);
    assert(listaFiltrata1[1].get_pret()==400);
    vector<Oferta> listaFiltrata2 = service.filtrare_destinatie("Exemplu");
    assert(listaFiltrata2.empty());
    service.stergere_oferta(service.cautare_oferte("La mare", "Mamaia"));
    service.stergere_oferta(service.cautare_oferte("Plaja", "Mamaia"));
    service.stergere_oferta(service.cautare_oferte("Munte", "Sinaia"));
}

void test_filtrare_pret()
{
    RepoFile repo{"../test.txt"};
    Validator valid;
    cosOferte cos{repo};
    Service service{repo, valid, cos};
    repo.store(Oferta("La mare", "Mamaia", "all-inclusive", 2345));
    assert(repo.get_all_oferte().size()==1);
    repo.store(Oferta("Plaja", "Mamaia", "-", 400));
    repo.store(Oferta("Munte", "Sinaia", "all-inclusive", 1234));
    vector<Oferta> listaFiltrata1 = service.filtrare_pret(2000);
    assert(listaFiltrata1.size()==2);
    assert(listaFiltrata1[0].get_pret()==400);
    assert(listaFiltrata1[1].get_pret()==1234);
    vector<Oferta> listaFiltrata2 = service.filtrare_pret(10);
    assert(typeid(service.filtrare_pret(100))== typeid(vector<Oferta>));
    assert(listaFiltrata2.size()==0);
    service.stergere_oferta(service.cautare_oferte("La mare", "Mamaia"));
    service.stergere_oferta(service.cautare_oferte("Plaja", "Mamaia"));
    service.stergere_oferta(service.cautare_oferte("Munte", "Sinaia"));
}

void test_sortare_service()
{
    RepoFile repo{"../test.txt"};
    Validator valid;
    cosOferte cos{repo};
    Service service{ repo, valid, cos };
    service.adaugare_oferta("a", "b", "csc", 100);
    service.adaugare_oferta("sa", "a", "c", 50);
    service.adaugare_oferta("t", "y", "c", 63);
    service.adaugare_oferta("o", "xc", "m", 100);

    service.sortare_denumire();
    assert(service.get_all_oferte().size() == 4);
    assert(service.get_all_oferte()[0].get_denumire() == "a");
    assert(service.get_all_oferte()[1].get_denumire() == "sa");

    service.sortare_destinatie();
    assert(service.get_all_oferte().size() == 4);
    assert(service.get_all_oferte()[0].get_destinatie() == "b");
    assert(service.get_all_oferte()[1].get_destinatie() == "a");


    service.sortare_tipPret();
    assert(service.get_all_oferte().size() == 4);
    assert(service.get_all_oferte()[0].get_tip() == "csc");
    assert(service.get_all_oferte()[0].get_pret() == 100);
    assert(service.get_all_oferte()[1].get_tip() == "c");
    assert(service.get_all_oferte()[1].get_pret() == 50);

    service.stergere_oferta(service.cautare_oferte("a", "b"));
    service.stergere_oferta(service.cautare_oferte("sa", "a"));
    service.stergere_oferta(service.cautare_oferte("t", "y"));
    service.stergere_oferta(service.cautare_oferte("o", "xc"));
}

void test_adaugare_cos_servie()
{
    RepoFile repo{"../test.txt"};
    Validator valid;
    cosOferte cos{repo};
    Service service{ repo, valid, cos };
    service.adaugare_oferta("a", "b", "csc", 100);
    service.adaugare_oferta("sa", "a", "c", 50);
    service.adaugare_oferta("t", "y", "c", 63);
    service.adaugare_oferta("o", "xc", "m", 100);
    service.cos_adauga("a", "b");
    assert(service.get_all_cos().size() == 1);
    service.cos_adauga("sa", "a");
    assert(service.get_all_cos().size() == 2);
    service.cos_adauga("t", "y");
    assert(service.get_all_cos().size() == 3);
    service.cos_adauga("o", "xc");
    assert(service.get_all_cos().size() == 4);
    service.stergere_oferta(service.cautare_oferte("a", "b"));
    service.stergere_oferta(service.cautare_oferte("sa", "a"));
    service.stergere_oferta(service.cautare_oferte("t", "y"));
    service.stergere_oferta(service.cautare_oferte("o", "xc"));
}

void test_adauga_cos()
{
    RepoFile repo{"../test.txt"};
    cosOferte cos{repo};
    vector<Oferta> of = {Oferta("a", "b", "csc", 100), Oferta("sa", "a", "c", 50), Oferta("t", "y", "c", 63), Oferta("o", "xc", "m", 100)};
    cos.adauga_oferta_cos(of[0]);
    assert(cos.get_all_cos().size() == 1);
    cos.adauga_oferta_cos(of[1]);
    assert(cos.get_all_cos().size() == 2);
    assert(cos.get_all_cos()[0].get_denumire() == "a");
    assert(cos.get_all_cos()[1].get_denumire() == "sa");
    assert(cos.get_all_cos()[0].get_destinatie() == "b");
}

void test_adauga_cos_random()
{
    RepoFile repo{"../test.txt"};
    cosOferte cos{repo};
    vector<Oferta> of = {Oferta("a", "b", "csc", 100), Oferta("sa", "a", "c", 50), Oferta("t", "y", "c", 63), Oferta("o", "xc", "m", 100)};
    cos.adauga_random_cos(of, 2);
    assert(cos.get_all_cos().size() == 2);
}

void test_adauga_cos_random_service()
{
    RepoFile repo{"../test.txt"};
    Validator valid;
    cosOferte cos{repo};
    Service service{ repo, valid, cos };
    service.adaugare_oferta("a", "b", "csc", 100);
    service.adaugare_oferta("sa", "a", "c", 50);
    service.adaugare_oferta("t", "y", "c", 63);
    service.adaugare_oferta("o", "xc", "m", 100);
    service.cos_adauga_random(2);
    assert(service.get_all_cos().size() == 2);

    service.stergere_oferta(service.cautare_oferte("a", "b"));
    service.stergere_oferta(service.cautare_oferte("sa", "a"));
    service.stergere_oferta(service.cautare_oferte("t", "y"));
    service.stergere_oferta(service.cautare_oferte("o", "xc"));
}

void test_sterge_cos()
{
    RepoFile repo{"../test.txt"};
    cosOferte cos{repo};
    vector<Oferta> of = {Oferta("a", "b", "csc", 100), Oferta("sa", "a", "c", 50), Oferta("t", "y", "c", 63), Oferta("o", "xc", "m", 100)};
    cos.adauga_oferta_cos(of[0]);
    cos.adauga_oferta_cos(of[1]);
    cos.adauga_oferta_cos(of[2]);
    cos.adauga_oferta_cos(of[3]);
    assert(cos.get_all_cos().size() == 4);
    cos.sterge_cos();
    assert(cos.get_all_cos().size() == 0);
}

void test_sterge_cos_service()
{
    RepoFile repo{"../test.txt"};
    Validator valid;
    cosOferte cos{repo};
    Service service{ repo, valid, cos };
    service.adaugare_oferta("a", "b", "csc", 100);
    service.adaugare_oferta("sa", "a", "c", 50);
    service.adaugare_oferta("t", "y", "c", 63);
    service.adaugare_oferta("o", "xc", "m", 100);
    service.cos_adauga_random(2);
    assert(service.get_all_cos().size() == 2);
    service.cos_stergere();
    assert(service.get_all_cos().size() == 0);

    service.stergere_oferta(service.cautare_oferte("a", "b"));
    service.stergere_oferta(service.cautare_oferte("sa", "a"));
    service.stergere_oferta(service.cautare_oferte("t", "y"));
    service.stergere_oferta(service.cautare_oferte("o", "xc"));
}

void test_export_cos()
{
    RepoFile repo{"../test.txt"};
    cosOferte cos{repo};
    Validator valid;
    Service service{ repo, valid, cos };
    service.adaugare_oferta("a", "b", "csc", 100);
    service.adaugare_oferta("sa", "a", "c", 50);
    service.adaugare_oferta("t", "y", "c", 63);
    service.adaugare_oferta("o", "xc", "m", 100);
    service.cos_adauga("a", "b");
    service.cos_adauga("sa", "a");
    service.cos_adauga("t", "y");
    service.cos_adauga("o", "xc");
    service.cos_export("../test.html");
    assert(service.get_all_cos().size() == 4);
    try {
        service.cos_export("../test.txt");
        assert(false);
    }
    catch (Exception& ex)
    {
        assert(true);
    }
    service.cos_stergere();
    service.stergere_oferta(service.cautare_oferte("a", "b"));
    service.stergere_oferta(service.cautare_oferte("sa", "a"));
    service.stergere_oferta(service.cautare_oferte("t", "y"));
    service.stergere_oferta(service.cautare_oferte("o", "xc"));
}

void test_undo()
{
    RepoFile repo{"../test.txt"};
    cosOferte cos{repo};
    Validator valid;
    Service service{ repo, valid, cos };
    service.adaugare_oferta("a", "b", "csc", 100);
    service.adaugare_oferta("sa", "a", "c", 50);
    service.adaugare_oferta("t", "y", "c", 63);
    service.adaugare_oferta("o", "xc", "m", 100);
    assert(service.get_all_oferte().size() == 4);
    service.undo();
    assert(service.get_all_oferte().size() == 3);
    Oferta of = service.cautare_oferte("t", "y");
    service.stergere_oferta(of);
    assert(service.get_all_oferte().size() == 2);
    service.undo();
    assert(service.get_all_oferte().size() == 3);
    service.modificare_oferta("a", "b", "haha", 300);
    assert(service.get_all_oferte()[0].get_pret() == 300);
    assert(service.get_all_oferte()[0].get_tip() == "haha");
    service.undo();
    assert(service.get_all_oferte()[0].get_pret() == 100);
    assert(service.get_all_oferte()[0].get_tip() == "csc");
    service.stergere_oferta(service.cautare_oferte("a", "b"));
    service.stergere_oferta(service.cautare_oferte("sa", "a"));
    service.stergere_oferta(service.cautare_oferte("t", "y"));
    //service.stergere_oferta(service.cautare_oferte("o", "xc"));
}

void test_create_dict()
{
    RepoFile repo{"../test.txt"};
    Validator valid;
    cosOferte cos{repo};
    Service service{ repo, valid, cos };
    service.adaugare_oferta("a", "b", "csc", 100);
    service.adaugare_oferta("sa", "a", "c", 50);
    service.adaugare_oferta("t", "y", "c", 63);
    service.adaugare_oferta("o", "xc", "m", 100);
    unordered_map<string, int> dict = service.creare_dict();
    assert(dict["csc"] == 1);
    assert(dict["c"] == 2);
    assert(dict["m"] == 1);
}

void test_fisier()
{
    string filename = "../test.txt";
    RepoFile repo{filename};
    Validator valid;
    cosOferte cos{repo};
    Service service{ repo, valid, cos };
    service.adaugare_oferta("a", "b", "csc", 100);
    service.adaugare_oferta("sa", "a", "c", 50);
    service.adaugare_oferta("t", "y", "c", 63);
    assert(service.get_all_oferte().size() == 3);
    service.stergere_oferta(service.cautare_oferte("t", "y"));
    assert(service.get_all_oferte().size() == 2);
    service.modificare_oferta("a", "b", "haha", 300);
    assert(service.get_all_oferte()[0].get_pret() == 300);
    assert(service.get_all_oferte()[0].get_tip() == "haha");
    service.stergere_oferta(service.cautare_oferte("a", "b"));
    service.stergere_oferta(service.cautare_oferte("sa", "a"));
    service.stergere_oferta(service.cautare_oferte("t", "y"));
    assert(service.get_all_oferte().size() == 0);
}

void test_repo_lab()
{
    /*
    RepoLab repo;
    repo.store(Oferta("La mare", "Mamaia", "all-inclusive", 2345));
    assert(repo.get_map().size()==1);
    repo.store(Oferta("Munte", "Sinaia", "all-inclusive", 1280));
    assert(repo.get_map().size()==2);
    try {
        repo.store(Oferta("La mare", "Mamaia", "all-inclusive", 2345));assert(false);
    }
    catch (Exception& re) {
        assert(re.getMessage() == "Oferta exista deja!");
    }
    assert(repo.get_map().size()==2);
    repo.del(Oferta("La mare", "Mamaia", "all-inclusive", 2345));
    assert(repo.get_map().size()==1);
    repo.modify(Oferta("Munte", "Sinaia", "all-inclusive", 1280));
    assert(repo.get_map().size()==1);
    assert(repo.get_map()["Munte"].get_tip() == "all-inclusive");
    assert(repo.get_map()["Munte"].get_pret() == 1280);
    assert(repo.get_map()["Munte"].get_destinatie() == "Sinaia");
*/
}

void test_all(){
    test_domain();
    test_store();
    test_find();
    test_del();
    test_modify();

    test_adaugare_service();
    test_stergere_service();
    test_modificare_service();
    test_filtrare_dest();
    test_filtrare_pret();
    test_sortare_service();

    test_adauga_cos();
    test_adaugare_cos_servie();
    test_adauga_cos_random();
    test_adauga_cos_random_service();
    test_sterge_cos();
    test_sterge_cos_service();
    //test_export_cos();
    test_undo();
    //test_fisier();
    //test_repo_lab();

    //test_create_dict();
}