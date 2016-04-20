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

#ifndef MPWMANAGER_H
#define MPWMANAGER_H

#include <QObject>

class MPWManager : public QObject
{
    Q_OBJECT
public:
    enum PasswordType {
        Maximum, Long, Medium, Basic, Short, PIN, Name, Phrase
    };
    Q_ENUMS(PasswordType)

    explicit MPWManager(QObject *parent = 0);
    virtual ~MPWManager();

    Q_INVOKABLE QString getKey() const;
    Q_INVOKABLE QString getName() const;
    Q_INVOKABLE void setUserData(const QString &name, const QString &password);
    Q_INVOKABLE QString getPassword(const QString &site, PasswordType type, const int counter) const;

private:
    QString m_key;
    QString m_name;
};

#endif // MPWMANAGER_H
