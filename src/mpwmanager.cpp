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
#include <QSettings>
#include <QThread>

#include "asyncmasterkey.h"
#include "dbmanager.h"

MPWManager::MPWManager(QObject *parent) :
    QObject(parent)
  , m_db(new DBManager(this))
  , m_model(new SitesSqlModel(this))
  , m_key(0)
{
    m_settings = new QSettings(QCoreApplication::applicationName(), QCoreApplication::applicationName(), this);

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
    qDebug() << "Using algorithm version:" << version;

    m_algVersion = version;
    m_settings->setValue("Algorithm", version);
}

void MPWManager::setName(const QString &name)
{
    m_name = name;
    m_settings->setValue("Name", name);
}

void MPWManager::generateMasterKey(const QString &name, const QString &password, AlgorithmVersion version)
{
    setName(name);
    setAlgorithmVersion(version);

    QThread* thread = new QThread;
    AsyncMasterKey* worker = new AsyncMasterKey(name, password, version);
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
    const char* p = mpw_siteResult((const unsigned char*) m_key->data(), site.toUtf8().data(),
                                   counter, MPKeyPurposeAuthentication, NULL, toMPSiteType(type),
                                   NULL, toMPAlgorithmVersion(m_algVersion));

    if (p) {
        m_db->insert(site, type, counter);
        m_model->refresh();
        return QString::fromUtf8(p);
    } else {
        qCritical() << "Error during password generation";
        return QString();
    }
}

MPAlgorithmVersion MPWManager::toMPAlgorithmVersion(AlgorithmVersion version)
{
    MPAlgorithmVersion v = MPAlgorithmVersionCurrent;
    switch (version) {
        case V0: v = MPAlgorithmVersion0; break;
        case V1: v = MPAlgorithmVersion1; break;
        case V2: v = MPAlgorithmVersion2; break;
        case V3: v = MPAlgorithmVersion3; break;
        default: qCritical() << "Unrecognized algorithm version:" << version;
    }

    return v;
}

MPResultType MPWManager::toMPSiteType(PasswordType type)
{
    MPResultType t = MPResultTypeTemplateLong;
    switch (type) {
        case Maximum: t = MPResultTypeTemplateMaximum; break;
        case Long: t = MPResultTypeTemplateLong; break;
        case Medium: t = MPResultTypeTemplateMedium; break;
        case Basic: t = MPResultTypeTemplateBasic; break;
        case Short: t = MPResultTypeTemplateShort; break;
        case PIN: t = MPResultTypeTemplatePIN; break;
        case Name: t = MPResultTypeTemplateName; break;
        case Phrase: t = MPResultTypeTemplatePhrase; break;
        default: qCritical() << "Unrecognized password type:" << type;
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
    //m_model = new SitesSqlModel(this);

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
