#include "cityviewform.h"

bool comp(CensusData d1, CensusData d2) {
    return d1.getYear() < d2.getYear();
}

CityViewForm::CityViewForm(QVector<CensusData> data, QWidget *parent)
    : QWidget{parent}
{
    setWindowTitle(data[0].getCityName() + " - Информация о демографических данных города");
    unsigned int minYear = -1, maxYear = 0;
    for (int i = 0; i < data.size(); ++i) {
        minYear = std::min(minYear, data[i].getYear());
        maxYear = std::max(maxYear, data[i].getYear());
    }
    QLabel *cityName = new QLabel(data[0].getCityName(), this);
    cityName->setAlignment(Qt::AlignCenter);
    QFont f = cityName->font();
    f.setBold(true);
    f.setPointSize(22);
    cityName->setFont(f);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(cityName);

    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    CitiesTableWidget *table = new CitiesTableWidget(data, this);
    table->hideColumn(0);
    table->sortByColumn(2, Qt::AscendingOrder);
    horizontalLayout->addWidget(table);

    if (data.size() < 2) {
        QLabel *notEnough = new QLabel("Недостаточно записей для построения графика", this);
        horizontalLayout->addWidget(notEnough);
    } else {

        QChartView *cityGraphView = new QChartView(this);
        QLineSeries *series = new QLineSeries();
        std::sort(data.begin(), data.end(), comp);
        for (int i = 0; i < data.size(); ++i) {
            series->append(data[i].getYear(), data[i].getPopulation());
        }
        QFont titleF;
        titleF.setBold(true);
        titleF.setPointSize(11);
        cityGraphView->chart()->addSeries(series);
        cityGraphView->chart()->legend()->setVisible(false);
        cityGraphView->chart()->setTitle("График изменения численности населения");
        cityGraphView->chart()->setTitleFont(titleF);
        QValueAxis *x = new QValueAxis(), *y = new QValueAxis();
        x->setMin(minYear - 1);
        x->setMax(maxYear + 1);
        x->setLabelFormat("%i");
        x->setTickCount(maxYear - minYear + 3);
        x->setTickInterval(1);
        titleF.setPointSize(7);
        x->setLabelsFont(titleF);
        y->setLabelFormat("%i");
        y->setTickInterval(1);
        cityGraphView->chart()->setAxisX(x, series);
        cityGraphView->chart()->setAxisY(y, series);
        cityGraphView->setMinimumWidth(400);
        cityGraphView->setMinimumHeight(200);
        horizontalLayout->addWidget(cityGraphView);

    }

    mainLayout->addLayout(horizontalLayout);

    setLayout(mainLayout);
    setMinimumHeight(500);
    setMinimumWidth(1000);
}
