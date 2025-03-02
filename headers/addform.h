#ifndef ADDFORM_H
#define ADDFORM_H

#include "citiestablewidget.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QSignalMapper>
#include <QCloseEvent>

class AddForm : public QWidget
{
    Q_OBJECT
    QVector<CensusData> newRecords;
    QLineEdit *cityNameEdit;
    QLineEdit *cityPopulationEdit;
    QLineEdit *yearEdit;
    QLabel *savedCounter;
    void closeEvent(QCloseEvent *event);
public:
    explicit AddForm(QWidget *parent = nullptr);

signals:
    void addingFinished(QVector<CensusData>);
    void closing();
private slots:
    void addRecord(int needExit = false);
    void finish();
};

#endif // ADDFORM_H
