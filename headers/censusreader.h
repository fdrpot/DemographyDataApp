#ifndef CENSUSREADER_H
#define CENSUSREADER_H

#include "citiestablewidget.h"
#include <QFile>
#include <QMessageBox>

class CensusReader {
private:
    QFile *f;
    bool readOneCensus();
    bool isAnalogRecord(QString, unsigned int);
public:
    CensusReader(QString fileName);
    ~CensusReader();
    QVector<CensusData> readDataFromFile();
    QVector<CensusData> readDataFromFile(QString);
    void addCensusRecord(CensusData &, bool&);
    void delCensusRecord(QString, unsigned int);
};

#endif // CENSUSREADER_H
