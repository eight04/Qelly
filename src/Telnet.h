/*****************************************************************************
 * Telnet.h
 *
 * Created: 05/10 2011 by uranusjr
 *
 * Copyright 2013 uranusjr. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/

#ifndef TELNET_H
#define TELNET_H

#include "AbstractConnection.h"
#include <QQueue>
class QHostAddress;
class QHostInfo;
class QTcpSocket;

namespace UJ
{

namespace Connection
{

class TelnetPrivate;

class Telnet : public AbstractConnection
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Telnet)
    TelnetPrivate * const d_ptr;

public:
    explicit Telnet(QObject *parent = 0);
    virtual ~Telnet();
    static const qint16 DefaultPort = 23;

public slots:
    virtual void close();
    virtual void reconnect();
    virtual void sendBytes(const QByteArray &bytes);

protected:
    virtual bool connectTo(const QString &address, qint16 port);

private slots:
    void onSocketHostFound();
    void onSocketConnected();
    void onSocketReadyRead();
    void onSocketError();
    void onSocketDisconnected();
};

}   // namespace Connection

}   // namespace UJ

#endif // TELNET_H
