/* Copyright (c) 2012-2013 Twitter4QML Project.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Twitter4QML nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL TWITTER4QML BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LISTSDESTROY_H
#define LISTSDESTROY_H

#include "abstractlistsaction.h"

class ListsDestroy : public AbstractListsAction
{
    Q_OBJECT
    Q_PROPERTY(QString owner_screen_name READ owner_screen_name WRITE owner_screen_name NOTIFY owner_screen_nameChanged)
    Q_PROPERTY(QString owner_id READ owner_id WRITE owner_id NOTIFY owner_idChanged)
    Q_PROPERTY(QString list_id READ list_id WRITE list_id NOTIFY list_idChanged)
    Q_PROPERTY(QString slug READ slug WRITE slug NOTIFY slugChanged)
    Q_DISABLE_COPY(ListsDestroy)
public:
    explicit ListsDestroy(QObject *parent = 0);

public slots:
    void exec();

signals:
    void owner_screen_nameChanged(const QString &owner_screen_name);
    void owner_idChanged(const QString &owner_id);
    void list_idChanged(const QString &list_id);
    void slugChanged(const QString &slug);

protected:
    QUrl api() const { return QUrl(QStringLiteral("https://api.twitter.com/1.1/lists/destroy.json")); }
};

#endif // LISTSDESTROY_H
