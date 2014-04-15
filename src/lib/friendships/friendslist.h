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

#ifndef FRIENDSLIST_H
#define FRIENDSLIST_H

#include "abstractusersmodel.h"

class TWITTER4QML_EXPORT FriendsList : public AbstractUsersModel
{
    Q_OBJECT
    Q_PROPERTY(QString user_id READ id WRITE id NOTIFY idChanged)
    Q_PROPERTY(QString screen_name READ screen_name WRITE screen_name NOTIFY screen_nameChanged)
    Q_PROPERTY(QString cursor READ cursor WRITE cursor NOTIFY cursorChanged)
    Q_PROPERTY(bool skip_status READ skip_status WRITE skip_status NOTIFY skip_statusChanged)
    Q_PROPERTY(bool include_user_entities READ include_entities WRITE include_entities NOTIFY include_entitiesChanged)

    Q_PROPERTY(int next_cursor READ next_cursor NOTIFY next_cursorChanged DESIGNABLE false)
    Q_PROPERTY(QString next_cursor_str READ next_cursor_str NOTIFY next_cursor_strChanged DESIGNABLE false)
    Q_PROPERTY(int previous_cursor READ previous_cursor NOTIFY previous_cursorChanged DESIGNABLE false)
    Q_PROPERTY(QString previous_cursor_str READ previous_cursor_str NOTIFY previous_cursor_strChanged DESIGNABLE false)
    Q_DISABLE_COPY(FriendsList)

public:
    explicit FriendsList(QObject *parent = 0);

public slots:
    void reload();

signals:
    void idChanged(const QString &id);
    void screen_nameChanged(const QString &screen_name);
    void cursorChanged(const QString &cursor);
    void skip_statusChanged(bool skip_status);
    void include_entitiesChanged(bool include_entities);

    void next_cursorChanged(int next_cursor) const;
    void next_cursor_strChanged(const QString &next_cursor_str) const;
    void previous_cursorChanged(int previous_cursor) const;
    void previous_cursor_strChanged(const QString &previous_cursor_str) const;

protected:
    QUrl api() const { return QUrl(QStringLiteral("https://api.twitter.com/1.1/friends/list.json")); }
    void parseDone(const QVariant &result);

private:
    ADD_PROPERTY(const QString &, id, QString)
    ADD_PROPERTY(bool, skip_status, bool)
};

#endif // FRIENDSLIST_H
