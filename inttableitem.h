#ifndef INTTABLEITEM_H
#define INTTABLEITEM_H

#include <QTableWidgetItem>
#include <QObject>

class IntTableItem : public QTableWidgetItem {
public:
    IntTableItem();
    IntTableItem(const unsigned int &val, int type = Type);
    bool operator<(const QTableWidgetItem &other) const;
    ~IntTableItem() {}
};

#endif // INTTABLEITEM_H
