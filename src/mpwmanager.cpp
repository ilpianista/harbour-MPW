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

#include "mpwmanager.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QSettings>
#include <QStandardPaths>
#include <QThread>

#include "asyncmasterkey.h"
#include "dbmanager.h"

MPWManager::MPWManager(QObject *parent)
    : QObject(parent)
    , m_db(new DBManager(this))
    , m_model(new SitesSqlModel(this))
    , m_key(0)
{
    const QString settingsPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)
                                 + QDir::separator() + QCoreApplication::applicationName()
                                 + ".conf";
    m_settings = new QSettings(settingsPath, QSettings::NativeFormat, this);

    m_name = m_settings->value("Name").toString();
    m_algVersion = algVersionFromInt(m_settings->value("Algorithm", 3).toUInt());
}

MPWManager::~MPWManager()
{
    delete m_db;
    delete m_model;
    delete m_key;
    delete m_settings;
}

MPWManager::AlgorithmVersion MPWManager::getAlgorithmVersion() const
{
    return m_algVersion;
}

QString MPWManager::getName() const
{
    return m_name;
}

QString MPWManager::getFingerprint() const
{
    return m_fingerprint;
}

void MPWManager::setAlgorithmVersion(AlgorithmVersion version)
{
    m_algVersion = version;
    m_settings->setValue("Algorithm", version);
}

void MPWManager::setName(const QString &name)
{
    m_name = name;
    m_settings->setValue("Name", name);
}

void MPWManager::generateMasterKey(const QString &name,
                                   const QString &password,
                                   AlgorithmVersion version)
{
    setName(name);
    setAlgorithmVersion(version);

    QThread *thread = new QThread;
    AsyncMasterKey *worker = new AsyncMasterKey(name, password, version);
    worker->moveToThread(thread);
    connect(thread, &QThread::started, worker, &AsyncMasterKey::generate);
    connect(worker, &AsyncMasterKey::finished, this, &MPWManager::gotMasterKey);
    connect(worker, &AsyncMasterKey::finished, thread, &QThread::quit);
    connect(worker, &AsyncMasterKey::finished, worker, &AsyncMasterKey::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    thread->start();
}

void MPWManager::gotMasterKey(QByteArray *key, const QString &fingerprint)
{
    qDebug() << "Storing master key";
    m_key = key;

    m_fingerprint = fingerprint;
    Q_EMIT generatedMasterKey(fingerprint);
}

QString MPWManager::getPassword(const QString &site, PasswordType type, const uint counter) const
{
    const char *p = sw_site_result((const uint8_t *) m_key->data(),
                                   site.toUtf8().data(),
                                   toMPSiteType(type),
                                   NULL,
                                   counter,
                                   SW_PURPOSE_AUTHENTICATION,
                                   NULL);

    if (p) {
        m_db->insert(site, type, counter);
        m_model->refresh();
        return QString::fromUtf8(p);
    } else {
        qCritical() << "Error during password generation";
        return QString();
    }
}

unsigned int MPWManager::toMPAlgorithmVersion(AlgorithmVersion version)
{
    unsigned int v = SW_ALGORITHM_CURRENT;
    switch (version) {
    case V0:
        v = SW_ALGORITHM_V0;
        break;
    case V1:
        v = SW_ALGORITHM_V1;
        break;
    case V2:
        v = SW_ALGORITHM_V2;
        break;
    case V3:
        v = SW_ALGORITHM_V3;
        break;
    default:
        qCritical() << "Unrecognized algorithm version:" << version;
    }

    return v;
}

uint32_t MPWManager::toMPSiteType(PasswordType type)
{
    uint32_t t = SW_RESULT_LONG;
    switch (type) {
    case Maximum:
        t = SW_RESULT_MAXIMUM;
        break;
    case Long:
        t = SW_RESULT_LONG;
        break;
    case Medium:
        t = SW_RESULT_MEDIUM;
        break;
    case Basic:
        t = SW_RESULT_BASIC;
        break;
    case Short:
        t = SW_RESULT_SHORT;
        break;
    case PIN:
        t = SW_RESULT_PIN;
        break;
    case Name:
        t = SW_RESULT_NAME;
        break;
    case Phrase:
        t = SW_RESULT_PHRASE;
        break;
    default:
        qCritical() << "Unrecognized password type:" << type;
    }

    return t;
}

void MPWManager::clearSites()
{
    m_db->clearSites();
    m_model->refresh();
}

void MPWManager::deleteSite(const QString &site)
{
    m_db->deleteSite(site);
    m_model->refresh();
}

SitesSqlModel *MPWManager::recentSites()
{
    // m_model = new SitesSqlModel(this);

    return m_model;
}

MPWManager::AlgorithmVersion MPWManager::algVersionFromInt(const uint &version)
{
    if (version == 0) {
        return V0;
    } else if (version == 1) {
        return V1;
    } else if (version == 2) {
        return V2;
    } else {
        return V3;
    }
}
