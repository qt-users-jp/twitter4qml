/* Copyright (c) 2012-2015 Twitter4QML Project.
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

#ifndef MEDIAUPLOAD_H
#define MEDIAUPLOAD_H

#include "abstractstatusaction.h"

class TWITTER4QML_EXPORT MediaUpload : public AbstractStatusAction
{
    Q_OBJECT
    Q_PROPERTY(QString media READ media WRITE media NOTIFY mediaChanged)


    Q_DISABLE_COPY(MediaUpload)
public:
    explicit MediaUpload(QObject *parent = 0);

signals:
    void mediaChanged(const QString &media);

protected:
    QUrl api() const { return QUrl(QStringLiteral("https://upload.twitter.com/1.1/media/upload.json")); }
    AuthorizeBy authenticationMethod() const { return AuthorizeByHeader; }
    bool isMultiPart() const { return true; }

    ADD_PROPERTY(const QString &, media, QString)
};

#endif // MEDIAUPLOAD_H
