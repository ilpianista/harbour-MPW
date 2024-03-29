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

#include "asyncmasterkey.h"

#include <QDebug>

extern "C"
{
#ifndef MPWALGORITHM_H
#define MPWALGORITHM_H
#include <mpw-algorithm.h>
#endif

#ifndef MPWUTIL_H
#define MPWUTIL_H
#include <mpw-util.h>
#endif
}

AsyncMasterKey::AsyncMasterKey(const QString &name, const QString &password,
                               MPWManager::AlgorithmVersion version)
    : m_name(name), m_password(password), m_algVersion(version)
{
}

AsyncMasterKey::~AsyncMasterKey()
{
}

void AsyncMasterKey::generate()
{
    const uint8_t* k = mpw_masterKey(m_name.toUtf8().data(), m_password.toUtf8().data(),
                                     MPWManager::toMPAlgorithmVersion(m_algVersion));

    QByteArray* key = 0;
    if (k) {
        key = new QByteArray((const char*) k, MPMasterKeySize);
    } else {
        qCritical() << "Error during master key generation.";
    }

    MPIdenticon fingerprint = mpw_identicon(m_name.toUtf8().data(), m_password.toUtf8().data());

    Q_EMIT finished(key, QString::fromUtf8(fingerprint.leftArm)
                    + QString::fromUtf8(fingerprint.body)
                    + QString::fromUtf8(fingerprint.rightArm)
                    + QString::fromUtf8(fingerprint.accessory));
}
