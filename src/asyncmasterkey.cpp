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

#include "spectre_wrapper.h"

AsyncMasterKey::AsyncMasterKey(const QString &name,
                               const QString &password,
                               MPWManager::AlgorithmVersion version)
    : m_name(name)
    , m_password(password)
    , m_algVersion(version)
{}

AsyncMasterKey::~AsyncMasterKey() {}

void AsyncMasterKey::generate()
{
    /* Allocate a buffer large enough to hold the full SpectreUserKey struct */
    const size_t keySize = sw_user_key_size();
    QByteArray *key = new QByteArray((int) keySize, '\0');

    if (!sw_user_key_fill(m_name.toUtf8().data(),
                          m_password.toUtf8().data(),
                          MPWManager::toMPAlgorithmVersion(m_algVersion),
                          (uint8_t *) key->data(),
                          keySize)) {
        qCritical() << "Error during master key generation.";
        delete key;
        key = nullptr;
    }

    SW_Identicon fingerprint = sw_identicon(m_name.toUtf8().data(), m_password.toUtf8().data());

    Q_EMIT finished(key,
                    QString::fromUtf8(fingerprint.leftArm) + QString::fromUtf8(fingerprint.body)
                        + QString::fromUtf8(fingerprint.rightArm)
                        + QString::fromUtf8(fingerprint.accessory));
}
