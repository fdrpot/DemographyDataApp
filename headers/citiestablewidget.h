#ifndef CITIESTABLEWIDGET_H
#define CITIESTABLEWIDGET_H

#include <QTableWidget>
#include <QSignalMapper>
#include "inttableitem.h"

class CensusReader;

class CensusData {

    friend class CensusReader;

    QString cityName;
    unsigned int population;
    unsigned int year;
public:

    static QVector<QString> nameOfData;
    static unsigned int getDataCount();

    unsigned int getYear();
    unsigned int getPopulation();
    QString getCityName();

    bool isEqOrMoreMillion();

    CensusData(QString, unsigned int, unsigned short);
    QVector<QTableWidgetItem*> getItemsForTable();
};

class CitiesTableWidget : public QTableWidget {
    Q_OBJECT
protected:
    Qt::SortOrder currentOrder = Qt::AscendingOrder;
    int currentSection = 0;
public:
    CitiesTableWidget(QVector<CensusData>, QWidget *parent = nullptr);
public slots:
    void updateData(QVector<CensusData>);
    void updateCurrentSort();
};

#endif // CITIESTABLEWIDGET_H
