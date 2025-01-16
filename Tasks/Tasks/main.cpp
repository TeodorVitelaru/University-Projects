#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include "GUI.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Repo repo{"../tasks.txt"};
    Validator val;
    Service srv{repo, val};
    GUI gui{srv};
    gui.show();
    return QApplication::exec();
}
