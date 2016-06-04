/*
  The MIT License (MIT)

  Copyright (c) 2016 Andrea Scarpino <me@andreascarpino.it>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include "sitessqlmodel.h"

const static char* COLUMN_NAMES[] = {
    "site",
    "type",
    "counter",
    "timestamp",
    NULL
};
const static QString SQL_SELECT = QStringLiteral("SELECT * FROM sites ORDER BY timestamp DESC;");

SitesSqlModel::SitesSqlModel(QObject *parent):
    QSqlQueryModel(parent)
{
    int idx = 0;
    while (COLUMN_NAMES[idx]) {
        m_roleNames[Qt::UserRole + idx + 1] = COLUMN_NAMES[idx];
        idx++;
    }

    refresh();
}

void SitesSqlModel::refresh()
{
    setQuery(SQL_SELECT);
}

QVariant SitesSqlModel::data(const QModelIndex &index, int role) const
{
    QVariant value;
    if (role < Qt::UserRole) {
        value = QSqlQueryModel::data(index, role);
    } else {
        const int columnIdx = role - Qt::UserRole - 1;
        const QModelIndex modelIndex = this->index(index.row(), columnIdx);
        value = QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
    }

    return value;
}

QHash<int, QByteArray> SitesSqlModel::roleNames() const
{
    return m_roleNames;
}
