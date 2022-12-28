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

#include "dbmanager.h"

#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>

const static QString DB_NAME = QStringLiteral("mpw");
const static int DB_VERSION = 1;

const static QString CLEAR_SITES = QStringLiteral("DELETE FROM sites;");
const static QString CREATE_SITES_TABLE = QStringLiteral("CREATE TABLE IF NOT EXISTS sites(site TEXT PRIMARY KEY, type TEXT, counter SMALLINT, timestamp DATETIME DEFAULT CURRENT_TIMESTAMP);");
const static QString DELETE_SITE = QStringLiteral("DELETE FROM sites WHERE site=\"%1\";");
const static QString INSERT_INTO_SITES = QStringLiteral("INSERT INTO sites(site, type, counter) VALUES(\"%1\", \"%2\", %3);");
const static QString READ_DB_VERSION = QStringLiteral("PRAGMA user_version;");
const static QString UPDATE_DB_VERSION = QStringLiteral("PRAGMA user_version=%1;");

DBManager::DBManager(QObject *parent) :
    QObject(parent)
{
    db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"));

    const QString dbPath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    db.setDatabaseName(dbPath + QDir::separator() + DB_NAME + QStringLiteral(".sql"));

    const QDir dir;
    if (!dir.exists(dbPath)) {
        if (!dir.mkpath(dbPath)) {
            qCritical("Cannot create data folder!");
        }
    }

    if (!db.open()) {
        qCritical("Unable to open database!");
    } else {
        init();
    }
}

DBManager::~DBManager()
{
    db.close();
}

void DBManager::clearSites()
{
    QSqlQuery clearSites(db);
    if (!clearSites.exec(CLEAR_SITES)) {
        qCritical("Cannot clear sites");
    }
}

void DBManager::deleteSite(const QString &site)
{
    QSqlQuery query(db);
    if (!query.exec(DELETE_SITE.arg(site))) {
        qCritical("Cannot delete data!");
    }
}

void DBManager::init()
{
    QSqlQuery query(db);

    if (!query.exec(CREATE_SITES_TABLE)) {
        qCritical("Cannot create table!");
        return;
    }
}

int DBManager::readDBVersion() const {
    QSqlQuery query(db);
    if (!query.exec(READ_DB_VERSION)) {
        qCritical("Cannot determine DB version!");
        return 0;
    }

    query.next();

    return query.record().value(0).toInt();
}

void DBManager::insert(const QString &site, MPWManager::PasswordType type, const uint counter)
{
    deleteSite(site);

    QSqlQuery query(db);
    if (!query.exec(INSERT_INTO_SITES.arg(site).arg(typeToString(type)).arg(counter))) {
        qCritical("Cannot save data!");
    }
}

QString DBManager::typeToString(MPWManager::PasswordType type) const
{
    QString t;
    switch (type) {
        case MPWManager::Maximum: t = QStringLiteral("Maximum"); break;
        case MPWManager::Medium: t = QStringLiteral("Medium"); break;
        case MPWManager::Basic: t = QStringLiteral("Basic"); break;
        case MPWManager::Short: t = QStringLiteral("Short"); break;
        case MPWManager::PIN: t = QStringLiteral("PIN"); break;
        case MPWManager::Name: t = QStringLiteral("Name"); break;
        case MPWManager::Phrase: t = QStringLiteral("Phrase"); break;
        case MPWManager::Long:
        default: t = QStringLiteral("Long");
            if (type != MPWManager::Long) {
                qCritical() << "Unrecognized password type:" << type;
            }
    }

    return t;
}
