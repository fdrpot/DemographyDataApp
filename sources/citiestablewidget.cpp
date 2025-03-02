#include "../headers/citiestablewidget.h"
#include "qheaderview.h"

CensusData::CensusData(QString name, unsigned int popultaion, unsigned short year) {
    this->cityName = name;
    this->population = popultaion;
    this->year = year;
}

unsigned int CensusData::getYear() {
    return year;
}

unsigned int CensusData::getPopulation() {
    return population;
}

QString CensusData::getCityName() {
    return cityName;
}

bool CensusData::isEqOrMoreMillion() {
    return population >= 1000000;
}

QVector<QString> CensusData::nameOfData = {"Название города", "Наcеление, чел.", "Год проведения переписи"};
unsigned int CensusData::getDataCount() {
    return nameOfData.size();
}

QVector<QTableWidgetItem*> CensusData::getItemsForTable() {
    QVector<QTableWidgetItem*> res;
    res.append(new QTableWidgetItem(cityName));
    res.append(new IntTableItem(population));
    res.append(new IntTableItem(year));
    return res;
}


CitiesTableWidget::CitiesTableWidget(QVector<CensusData> data, QWidget *parent): QTableWidget(parent) {
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::MultiSelection);
    setCursor(Qt::PointingHandCursor);
    setSortingEnabled(true);
    setFocusPolicy(Qt::NoFocus);
    verticalHeader()->hide();
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    updateData(data);

    connect(horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(updateCurrentSort()));
    setMinimumWidth(500);
}

void CitiesTableWidget::updateData(QVector<CensusData> newData) {
    clearContents();
    setRowCount(0);
    setColumnCount(0);
    setRowCount(newData.size());
    setColumnCount(CensusData::getDataCount());
    setHorizontalHeaderLabels(CensusData::nameOfData);
    for (int i = 0; i < newData.size(); ++i) {
        QVector<QTableWidgetItem*> items = newData[i].getItemsForTable();
        for (int j = 0; j < items.size(); ++j) {
            setItem(i, j, items[j]);
        }
    }
    sortItems(currentSection, currentOrder);
}

void CitiesTableWidget::updateCurrentSort() {
    currentSection = horizontalHeader()->sortIndicatorSection();
    currentOrder = horizontalHeader()->sortIndicatorOrder();
}
