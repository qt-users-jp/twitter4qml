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

#include "abstracttwitter4qmltest.h"

#include <updatestatuseswithmedia.h>
#include <verifycredentials.h>

class UpdateStatusesWithMediaTest : public AbstractTwitter4QMLTest
{
    Q_OBJECT

private Q_SLOTS:
    void status();
    void status_data();
};

void UpdateStatusesWithMediaTest::status()
{
    QFETCH(QString, data);
    QFETCH(QString, media);

    UpdateStatusesWithMedia updateStatus;
    QCOMPARE(updateStatus.status(), QString());

    QString tweet = QString("%1 - %2").arg(data).arg(QDateTime::currentDateTime().toString());
    updateStatus.status(tweet);
    QCOMPARE(updateStatus.status(), tweet);

    QVariantList mediaList;
    mediaList << media;
    updateStatus.media(mediaList);
    QCOMPARE(updateStatus.media(), mediaList);

    QVERIFY2(exec(&updateStatus), "UpdateStatus::exec()");

    QVariantMap response = updateStatus.data().toMap();

    QCOMPARE(response.value("id").isValid(), true);
}

void UpdateStatusesWithMediaTest::status_data()
{
    QTest::addColumn<QString>("data");
    QTest::addColumn<QString>("media");
    QTest::newRow("Test") << "Upload" << ":/QtLogo.png";
}

QTEST_MAIN(UpdateStatusesWithMediaTest)

#include "tst_updatestatuseswithmedia.moc"
