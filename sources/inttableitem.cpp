#include "inttableitem.h"

IntTableItem::IntTableItem(): QTableWidgetItem() {}
IntTableItem::IntTableItem(const unsigned int &val, int type): QTableWidgetItem(QString::number(val), type) {}

bool IntTableItem::operator<(const QTableWidgetItem &other) const {
    return text().toUInt() < other.text().toUInt();
}
