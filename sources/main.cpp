#include "../headers/citiesviewwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CitiesViewWidget w;
    w.show();

    return a.exec();
}
