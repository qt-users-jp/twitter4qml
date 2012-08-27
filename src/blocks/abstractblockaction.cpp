/* Copyright (c) 2012 Twitter4QML Project.
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

#include "abstractblockaction.h"

class AbstractBlockAction::Private
{
public:
    Private();

    QString screenName;
    QString userId;
    bool includeEntities;
    bool skipStatus;
};

AbstractBlockAction::Private::Private()
    : includeEntities(true)
    , skipStatus(false)
{
}

AbstractBlockAction::AbstractBlockAction(QObject *parent)
    : AbstractTwitterAction(parent)
    , d(new Private)
{
}

AbstractBlockAction::~AbstractBlockAction()
{
    delete d;
}

const QString &AbstractBlockAction::screenName() const
{
    return d->screenName;
}

void AbstractBlockAction::setScreenName(const QString &screenName)
{
    if (d->screenName == screenName) return;
    d->screenName = screenName;
    emit screenNameChanged(screenName);
}

const QString &AbstractBlockAction::userId() const
{
    return d->userId;
}

void AbstractBlockAction::setUserId(const QString &userId)
{
    if (d->userId == userId) return;
    d->userId = userId;
    emit userIdChanged(userId);
}

bool AbstractBlockAction::includeEntities() const
{
    return d->includeEntities;
}

void AbstractBlockAction::setIncludeEntities(bool includeEntities)
{
    if (d->includeEntities == includeEntities) return;
    d->includeEntities = includeEntities;
    emit includeEntitiesChanged(includeEntities);
}

bool AbstractBlockAction::skipStatus() const
{
    return d->skipStatus;
}

void AbstractBlockAction::setSkipStatus(bool skipStatus)
{
    if (d->skipStatus == skipStatus) return;
    d->skipStatus = skipStatus;
    emit skipStatusChanged(skipStatus);
}
