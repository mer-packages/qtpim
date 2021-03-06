/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef tst_QVERSITCONTACTEXPORTER_H
#define tst_QVERSITCONTACTEXPORTER_H

#include <qcontactdetail.h>
#include <QObject>
#include <qcontact.h>
#include <qversitproperty.h>

QT_BEGIN_NAMESPACE_VERSIT
class QVersitContactExporter;
class QVersitContactExporterPrivate;
class QVersitDocument;
class QVersitProperty;
QT_END_NAMESPACE_VERSIT

QTCONTACTS_USE_NAMESPACE
QTVERSIT_USE_NAMESPACE

class MyQVersitResourceHandler;
class MyQVersitContactExporterDetailHandler;

class tst_QVersitContactExporter : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void testConvertContact();
    void testEmptyContact();
    void testContactDetailHandler();
    void testContactDetailHandlerV2();
    void testEncodeName();
    void testEncodePhoneNumber();
    void testEncodeEmailAddress();
    void testEncodeStreetAddress();
    void testEncodeUrl();
    void testEncodeParameters();
    void testEncodeUid();
    void testEncodeRev();
    void testEncodeVersion();
    void testEncodeBirthDay();
    void testEncodeNote();
    void testEncodeGeoLocation();
    void testEncodeOrganization();
    void testEncodeEmbeddedContent();
    void testEncodeRingtone();
    void testEncodeGender();
    void testEncodeNickName();
    void testEncodeTag();
    void testEncodeAnniversary();
    void testEncodeOnlineAccount();
    void testEncodeFamily();
    void testEncodeFavorite();
    void testEncodeExtendedDetail();
    void testEncodeExtendedDetail_data();
    void testEncodeMultipleExtendedDetails();
    void testEncodeAvatar();
    void testDefaultResourceHandler();
    void testEncodeEmailWithContextOther();

private:
    // Test Utility Functions
    QContact createContactWithName(QString name);
    int countProperties(const QVersitDocument& document);
    QContactDetail findDetailByType(QList<QContactDetail> details, QContactDetail::DetailType search);
    QVersitProperty findPropertyByName(const QVersitDocument& document,const QString& propertyName);

private: // Data
    QVersitContactExporter* mExporter;
    MyQVersitResourceHandler* mResourceHandler;
};

#endif // tst_QVERSITCONTACTEXPORTER_H
