#include "../headers/citiesviewwidget.h"
#include "../headers/citiestablewidget.h"
#include "../headers/cityviewform.h"


#define RUS(str) codec->toUnicode(str)

CitiesViewWidget::CitiesViewWidget(QWidget *parent)
    : QWidget(parent) {
    setWindowTitle("Демографические данные");
    reader = new CensusReader("census_records.txt");

    mainTable = new CitiesTableWidget(reader->readDataFromFile(), this);
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(mainTable);

    QVBoxLayout *groupsLayout = new QVBoxLayout();

    QGroupBox *actionsBox = new QGroupBox(this);
    actionsBox->setTitle("Действия");
    QVBoxLayout *actionsLayout = new QVBoxLayout(actionsBox);

    QPushButton *addBtn = new QPushButton("Добавить запись", actionsBox);
    moreInfoBtn = new QPushButton("Подробнее о выделенном...", actionsBox);
    moreInfoBtn->setEnabled(false);
    delBtn = new QPushButton("Удалить выделенное", actionsBox);
    delBtn->setEnabled(false);

    actionsLayout->addWidget(addBtn);
    actionsLayout->addWidget(moreInfoBtn);
    actionsLayout->addWidget(delBtn);
    actionsBox->setLayout(actionsLayout);

    QGroupBox *settingsBox = new QGroupBox(this);
    settingsBox->setTitle("Настройки");
    QVBoxLayout *settingsLayout = new QVBoxLayout(settingsBox);

    onlyMillionersCheckBox = new QCheckBox("Только города-миллионеры", settingsBox);
    onlyLastDataCheckBox = new QCheckBox("Только последние переписи", settingsBox);
    QHBoxLayout *yearSettings = new QHBoxLayout();
    onlyYearCheckBox = new QCheckBox("Только в год:", settingsBox);
    onlyYearCheckBox->setEnabled(false);
    yearLineEdit = new QLineEdit(settingsBox);
    yearLineEdit->setMaximumWidth(60);
    yearSettings->addWidget(onlyYearCheckBox);
    yearSettings->addWidget(yearLineEdit);
    yearSettings->addStretch();

    settingsLayout->addWidget(onlyMillionersCheckBox);
    settingsLayout->addWidget(onlyLastDataCheckBox);
    settingsLayout->addLayout(yearSettings);
    settingsBox->setLayout(settingsLayout);

    groupsLayout->addWidget(actionsBox);
    groupsLayout->addWidget(settingsBox);
    groupsLayout->addStretch();

    mainLayout->addLayout(groupsLayout);

    connect(yearLineEdit, SIGNAL(textChanged(QString)), this, SLOT(checkYearSelection()));

    connect(addBtn, SIGNAL(clicked()), this, SLOT(openAddForm()));
    connect(moreInfoBtn, SIGNAL(clicked()), this, SLOT(openInfoForm()));
    connect(delBtn, SIGNAL(clicked()), this, SLOT(delSelectedRecords()));

    connect(onlyMillionersCheckBox, SIGNAL(clicked()), this, SLOT(checkFilters()));
    connect(onlyYearCheckBox, SIGNAL(clicked()), this, SLOT(checkFilters()));
    connect(onlyLastDataCheckBox, SIGNAL(clicked()), this, SLOT(checkFilters()));

    connect(mainTable->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(checkDelAndEditBtn()));

    setLayout(mainLayout);
}

void CitiesViewWidget::delSelectedRecords() {
    QList rows = mainTable->selectionModel()->selectedRows();
    QMessageBox *mbox = new QMessageBox(QMessageBox::Question, "Подтвердите действие", "Вы действительно хотите удалить выделенные записи (" + QString::number(rows.size()) + ")?", QMessageBox::NoButton, this);
    mbox->addButton("Да", QMessageBox::AcceptRole);
    mbox->addButton("Нет", QMessageBox::RejectRole);
    if (mbox->exec() == QMessageBox::RejectRole) return;
    QVector<int> rl;
    for (int i = 0; i < rows.size(); ++i) {
        rl.append(rows[i].row());
    }
    for (int i = 0; i < rl.size(); ++i) {
        reader->delCensusRecord(mainTable->item(rl[i], 0)->text(), mainTable->item(rl[i], 2)->text().toUInt());
    }
    checkFilters();
}

void CitiesViewWidget::openAddForm() {
    if (curForm != nullptr) return;
    curForm = new AddForm();
    connect(curForm, SIGNAL(addingFinished(QVector<CensusData>)), this, SLOT(processNewRecords(QVector<CensusData>)));
    connect(curForm, SIGNAL(closing()), this, SLOT(setEnabledTrue()));
    setEnabled(false);
    curForm->show();
}

void CitiesViewWidget::processNewRecords(QVector<CensusData> newData) {
    curForm = nullptr;
    if (newData.size() == 0) return;

    int counter = 0;
    for (int i = 0; i < newData.size(); ++i) {
        bool wasDeleted = false;
        reader->addCensusRecord(newData[i], wasDeleted);
        if (wasDeleted) {
            counter++;
        }
    }
    checkFilters();
    setEnabled(true);
    QMessageBox mb(QMessageBox::Information, "Записи добавлены", "Добавлено новых записей: " + QString::number(newData.size()) + "\nПри этом было изменено записей: " + QString::number(counter), QMessageBox::Ok, this);
    mb.exec();
}

void CitiesViewWidget::checkDelAndEditBtn() {
    if (mainTable->selectionModel()->selectedRows().isEmpty()) { delBtn->setEnabled(false); moreInfoBtn->setEnabled(false); }
    else { delBtn->setEnabled(true); moreInfoBtn->setEnabled(true); }
}

QVector<CensusData> CitiesViewWidget::filterByMillioners(QVector<CensusData> data) {
    if (!onlyMillionersCheckBox->isChecked()) return data;
    QVector<CensusData> filteredData;
    for (int i = 0; i < data.size(); ++i) {
        if (data[i].isEqOrMoreMillion()) filteredData.append(data[i]);
    }
    return filteredData;
}

void CitiesViewWidget::checkFilters() {
    QVector<CensusData> data = reader->readDataFromFile();
    data = filterByLast(data);
    data = filterByMillioners(data);
    data = filterByYear(data);
    mainTable->updateData(data);
}

void CitiesViewWidget::checkYearSelection() {
    bool ok = false;
    int year = yearLineEdit->text().toInt(&ok);
    if (ok && year >= 0) onlyYearCheckBox->setEnabled(true);
    else  {
        onlyYearCheckBox->setEnabled(false);
        onlyYearCheckBox->setChecked(false);
    }
    checkFilters();
}

QVector<CensusData> CitiesViewWidget::filterByYear(QVector<CensusData> data) {
    if (!onlyYearCheckBox->isChecked()) return data;
    QVector<CensusData> filteredData;
    unsigned int year = yearLineEdit->text().toUInt();
    for (int i = 0; i < data.size(); ++i) {
        if (data[i].getYear() == year) filteredData.append(data[i]);
    }
    return filteredData;
}

QVector<CensusData> CitiesViewWidget::filterByLast(QVector<CensusData> data) {
    if (!onlyLastDataCheckBox->isChecked()) return data;
    QVector<CensusData> filteredData;
    QSet<QString> s;
    for (int i = 0; i < data.size(); ++i) s.insert(data[i].getCityName());
    QSetIterator<QString> si(s);
    while (si.hasNext()) {
        QString name = si.next();
        unsigned int lastYear = 0, ind = -1;
        for (int i = 0; i < data.size(); ++i) {
            if (data[i].getCityName() == name && data[i].getYear() >= lastYear) {
                lastYear = data[i].getYear();
                ind = i;
            }
        }
        filteredData.append(data[ind]);
    }
    return filteredData;
}

void CitiesViewWidget::openInfoForm() {
    QList rows = mainTable->selectionModel()->selectedRows();
    QVector<int> rl;
    for (int i = 0; i < rows.size(); ++i) rl.append(rows[i].row());
    QVector<QString> cityNames;
    for (int i = 0; i < rl.size(); ++i) {
        QString name = mainTable->item(rl[i], 0)->text();
        if (!cityNames.contains(name)) {
            QVector<CensusData> data = reader->readDataFromFile(name);
            CityViewForm *form = new CityViewForm(data);
            form->show();
            cityNames.append(name);
        }
    }
}


void CitiesViewWidget::setEnabledTrue() {
    curForm = nullptr;
    setEnabled(true);
}

CitiesViewWidget::~CitiesViewWidget() {}

