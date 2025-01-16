#include <QApplication>
#include <QPushButton>
//#include "ui.h"
#include "teste.h"
#include "gui.h"
#include "IncercareGUI.h"

using namespace std;


void start_app()
{
    string filename = "../oferte.txt";
    try {
        RepoFile repo{filename};
        Validator val;
        cosOferte cos{repo};
        Service srv{ repo, val, cos};
        Consola ui{srv};
        ui.run();
    }
    catch (RepoException& re)
    {
        cout<<re.getErrorMessage();
    }
}

int main(int argc, char *argv[])
{
    test_all();
    //start_app();
    QApplication a(argc, argv);
    string filename = "../oferte.txt";
    RepoFile repo{filename};
    Validator val;
    cosOferte cos{repo};
    Service srv{ repo, val, cos};
    IncercareGUI gui{srv};
    gui.show();
    return  a.exec();
}