#ifndef CITYVIEWFORM_H
#define CITYVIEWFORM_H

#include <QWidget>
#include "citiestablewidget.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>

class CityViewForm : public QWidget
{
    Q_OBJECT
public:
    CityViewForm(QVector<CensusData>, QWidget *parent = nullptr);

signals:

};

#endif // CITYVIEWFORM_H
