#include "addform.h"

#define EDIT_WIDTH 200

AddForm::AddForm(QWidget *parent)
    : QWidget{parent} {
    setWindowTitle("Добавление записи о переписи");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    savedCounter = new QLabel("Сохранено записей: 0", this);
    savedCounter->setStyleSheet("font-weight: bold");
    savedCounter->setAlignment(Qt::AlignCenter);
    savedCounter->hide();
    mainLayout->addWidget(savedCounter);

    QHBoxLayout *cityNameLayout = new QHBoxLayout();
    QLabel *cityNameLabel = new QLabel("Название города", this);
    cityNameEdit = new QLineEdit(this);
    cityNameEdit->setFixedWidth(EDIT_WIDTH);
    cityNameLayout->addWidget(cityNameLabel);
    cityNameLayout->addStretch();
    cityNameLayout->addWidget(cityNameEdit);
    mainLayout->addLayout(cityNameLayout);

    QHBoxLayout *cityPopulationLayout = new QHBoxLayout();
    QLabel *cityPopulationLabel = new QLabel("Население, чел.", this);
    cityPopulationEdit = new QLineEdit(this);
    cityPopulationEdit->setFixedWidth(EDIT_WIDTH);
    cityPopulationLayout->addWidget(cityPopulationLabel);
    cityPopulationLayout->addStretch();
    cityPopulationLayout->addWidget(cityPopulationEdit);
    mainLayout->addLayout(cityPopulationLayout);

    QHBoxLayout *yearLayout = new QHBoxLayout();
    QLabel *yearLabel = new QLabel("Год проведения переписи", this);
    yearEdit = new QLineEdit(this);
    yearEdit->setFixedWidth(EDIT_WIDTH);
    yearLayout->addWidget(yearLabel);
    yearLayout->addStretch();
    yearLayout->addWidget(yearEdit);
    mainLayout->addLayout(yearLayout);

    QVBoxLayout *btnsLayout = new QVBoxLayout();
    QPushButton *addBtn = new QPushButton("Сохранить запись", this);
    QPushButton *addAndCloseBtn = new QPushButton("Сохранить запись и выйти", this);
    QPushButton *exitBtn = new QPushButton("Выйти (с сохранением предыдущих записей)", this);
    btnsLayout->addWidget(addBtn);
    btnsLayout->addWidget(addAndCloseBtn);
    btnsLayout->addWidget(exitBtn);
    mainLayout->addLayout(btnsLayout);

    addAndCloseBtn->setDefault(true);
    connect(cityNameEdit, SIGNAL(returnPressed()), addAndCloseBtn, SLOT(click()));
    connect(cityPopulationEdit, SIGNAL(returnPressed()), addAndCloseBtn, SLOT(click()));
    connect(yearEdit, SIGNAL(returnPressed()), addAndCloseBtn, SLOT(click()));

    setFixedSize(400, 220);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    QSignalMapper *smp = new QSignalMapper(this);

    connect(addBtn, SIGNAL(clicked()), smp, SLOT(map()));
    connect(addAndCloseBtn, SIGNAL(clicked()), smp, SLOT(map()));

    smp->setMapping(addBtn, 0);
    smp->setMapping(addAndCloseBtn, 1);

    connect(smp, SIGNAL(mappedInt(int)), this, SLOT(addRecord(int)));
    connect(exitBtn, SIGNAL(clicked()), this, SLOT(finish()));

    setLayout(mainLayout);
}

void AddForm::addRecord(int needExit) {
    QMessageBox notCorrectInput(QMessageBox::Critical, "Некорректный ввод", "Все поля должны быть заполнены, численность населения и год переписи должны быть целыми числами");
    bool ok1 = false, ok2 = false;
    QString recordName = cityNameEdit->text();
    unsigned int recordPopulation = cityPopulationEdit->text().toUInt(&ok1);
    unsigned int recordYear = yearEdit->text().toUInt(&ok2);

    if (!ok1 || !ok2 || recordName.isEmpty()) {
        notCorrectInput.exec();
        return;
    }

    cityNameEdit->clear();
    cityPopulationEdit->clear();
    yearEdit->clear();

    CensusData newR(recordName, recordPopulation, recordYear);

    newRecords.append(newR);
    if (needExit) finish();
    else {
        savedCounter->setText("Сохранено записей: " + QString::number(newRecords.size()));
        savedCounter->show();
    }
}

void AddForm::closeEvent(QCloseEvent *event) {
    emit closing();
    event->accept();
}

void AddForm::finish() {
    emit addingFinished(newRecords);
    close();
}
