#ifndef CITIESVIEWWIDGET_H
#define CITIESVIEWWIDGET_H

#include <QWidget>
#include <QTextCodec>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QGroupBox>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <censusreader.h>
#include "addform.h"

class CitiesViewWidget : public QWidget
{
    Q_OBJECT
protected:
    CensusReader *reader;
    AddForm *curForm = nullptr;
    CitiesTableWidget *mainTable;
    QPushButton *moreInfoBtn, *delBtn;
    QCheckBox *onlyMillionersCheckBox, *onlyLastDataCheckBox, *onlyYearCheckBox;
    QLineEdit *yearLineEdit;
    QVector<CensusData> filterByMillioners(QVector<CensusData>);
    QVector<CensusData> filterByYear(QVector<CensusData>);
    QVector<CensusData> filterByLast(QVector<CensusData>);
public:
    CitiesViewWidget(QWidget *parent = nullptr);
    ~CitiesViewWidget();
private slots:
    void checkFilters();
    void openAddForm();
    void openInfoForm();
    void processNewRecords(QVector<CensusData>);
    void delSelectedRecords();
    void checkDelAndEditBtn();
    void checkYearSelection();
    void setEnabledTrue();
};
#endif // CITIESVIEWWIDGET_H
