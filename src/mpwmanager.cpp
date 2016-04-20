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

extern "C"
{
#include <mpw-algorithm.h>
}

MPWManager::MPWManager(QObject *parent) :
    QObject(parent)
{
}

MPWManager::~MPWManager()
{
}

QString MPWManager::getKey() const
{
    return m_key;
}

QString MPWManager::getName() const
{
    return m_name;
}

void MPWManager::setUserData(const QString &name, const QString &password)
{
    m_name = name;

    m_key = QString::fromLatin1((const char*) mpw_masterKeyForUser(name.toLatin1().data(),
                                   password.toLatin1().data(), MPAlgorithmVersionCurrent));
}

QString MPWManager::getPassword(const QString &site, PasswordType type, const int counter) const
{
    MPSiteType t = 0;
    switch (type) {
        case Maximum: t = MPSiteTypeGeneratedMaximum; break;
        case Long: t = MPSiteTypeGeneratedLong; break;
        case Medium: t = MPSiteTypeGeneratedMedium; break;
        case Basic: t = MPSiteTypeGeneratedBasic; break;
        case Short: t = MPSiteTypeGeneratedShort; break;
        case PIN: t = MPSiteTypeGeneratedPIN; break;
        case Name: t = MPSiteTypeGeneratedName; break;
        case Phrase: t = MPSiteTypeGeneratedPhrase; break;
        default: qCritical() << "Unrecognized password type" << type;
    }

    return QString::fromLatin1(mpw_passwordForSite((const unsigned char*) m_key.toLatin1().data(),
                             site.toLatin1().data(), t, counter, MPSiteVariantPassword, NULL,
                             MPAlgorithmVersionCurrent));
}
