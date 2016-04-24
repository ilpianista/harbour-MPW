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

#include <QDebug>

MPWManager::MPWManager(QObject *parent) :
    QObject(parent)
{
}

MPWManager::~MPWManager()
{
}

MPWManager::AlgorithmVersion MPWManager::getAlgorithmVersion() const
{
    return m_algVersion;
}

QString MPWManager::getName() const
{
    return m_name;
}

void MPWManager::setAlgorithmVersion(AlgorithmVersion version)
{
    m_algVersion = version;
}

void MPWManager::setName(const QString &name)
{
    m_name = name;
}

void MPWManager::generateMasterKey(const QString &name, const QString &password, AlgorithmVersion version)
{
    setName(name);
    setAlgorithmVersion(version);
    qDebug() << "Using algorithm version:" << m_algVersion;

    const uint8_t* k = mpw_masterKeyForUser(name.toUtf8().data(), password.toUtf8().data(), toMPAlgorithmVersion(version));

    if (k) {
        m_key = QByteArray::fromRawData((const char*) k, MP_dkLen);
    } else {
        qCritical() << "Error during master key generation.";
    }
}

MPAlgorithmVersion MPWManager::toMPAlgorithmVersion(AlgorithmVersion version) const
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

QString MPWManager::getPassword(const QString &site, PasswordType type, const uint counter) const
{
    const char* p = mpw_passwordForSite((const unsigned char*) m_key.data(), site.toUtf8().data(),
                                        toMPSiteType(type), counter, MPSiteVariantPassword, NULL,
                                        toMPAlgorithmVersion(m_algVersion));

    if (p) {
        return QString::fromUtf8(p);
    } else {
        qCritical() << "Error during password generation.";
        return QString();
    }
}

MPSiteType MPWManager::toMPSiteType(PasswordType type) const
{
    MPSiteType t = MPSiteTypeGeneratedLong;
    switch (type) {
        case Maximum: t = MPSiteTypeGeneratedMaximum; break;
        case Long: t = MPSiteTypeGeneratedLong; break;
        case Medium: t = MPSiteTypeGeneratedMedium; break;
        case Basic: t = MPSiteTypeGeneratedBasic; break;
        case Short: t = MPSiteTypeGeneratedShort; break;
        case PIN: t = MPSiteTypeGeneratedPIN; break;
        case Name: t = MPSiteTypeGeneratedName; break;
        case Phrase: t = MPSiteTypeGeneratedPhrase; break;
        default: qCritical() << "Unrecognized password type:" << type;
    }

    return t;
}
