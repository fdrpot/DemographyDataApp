#include "../headers/censusreader.h"

CensusReader::CensusReader(QString fileName) {
    f = new QFile(fileName);
    if (!f->exists()) {
        QMessageBox msg(QMessageBox::Critical, "Файл с данными не найден!", "Файл " + fileName + " создан.", QMessageBox::Ok);
        msg.exec();
    }
    f->open(QFile::ReadWrite);
}

QVector<CensusData> CensusReader::readDataFromFile() {
    f->seek(0);
    QVector<CensusData> res;
    QDataStream in(f);
    while (!in.atEnd()) {
        QString name;
        unsigned int population;
        unsigned int year;
        in >> name >> population >> year;
        CensusData cd(name, population, year);
        res.append(cd);
    }
    return res;
}

QVector<CensusData> CensusReader::readDataFromFile(QString cityName) {
    f->seek(0);
    QVector<CensusData> res;
    QDataStream in(f);
    while (!in.atEnd()) {
        QString name;
        unsigned int population;
        unsigned int year;
        in >> name >> population >> year;
        if (name == cityName) {
            CensusData cd(name, population, year);
            res.append(cd);
        }
    }
    return res;
}


bool CensusReader::isAnalogRecord(QString name, unsigned int year) {
    QVector<CensusData> data = readDataFromFile();
    for (int i = 0; i < data.size(); ++i) {
        if (data[i].cityName == name && data[i].year == year) {
            return true;
        }
    }
    return false;
}

void CensusReader::addCensusRecord(CensusData &data, bool &wasDeleted) {
    if (isAnalogRecord(data.cityName, data.year)) {
        delCensusRecord(data.cityName, data.year);
        wasDeleted = true;
    } else {
        wasDeleted = false;
    }
    f->seek(f->size());
    QDataStream out(f);
    out << data.cityName << data.population << data.year;
}

void CensusReader::delCensusRecord(QString cityName, unsigned int year) {
    QVector<CensusData> cur = readDataFromFile();
    int n = cur.size();
    for (int i = 0; i < n; ++i) {
        if (cur[i].cityName == cityName && cur[i].year == year) {
            cur.remove(i);
            break;
        }
    }
    f->resize(0);
    for (int i = 0; i < n - 1; ++i) {
        bool ok;
        addCensusRecord(cur[i], ok);
    }
}

CensusReader::~CensusReader() {
    f->close();
    delete f;
}
