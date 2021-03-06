/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Mobility Components.
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

#ifndef QCONTACTTESTPHONENUMBERACTION_P_H
#define QCONTACTTESTPHONENUMBERACTION_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qtcontacts.h"
#include "qserviceplugininterface.h"
#include "qservicecontext.h"
#include "qserviceinterfacedescriptor.h"

#include <QSharedData>
#include <QString>
#include <QVariantMap>

QTM_USE_NAMESPACE

class DummyAction : public QContactAction
{
public:
    DummyAction(QObject* parent = 0) { Q_UNUSED(parent) }

    QVariantMap metaData() const {return QVariantMap();}

    bool invokeAction(const QContactActionTarget&, const QVariantMap&)
    {
        // Well, do something
        emit stateChanged(QContactAction::FinishedState);
        return true;
    }

    bool invokeAction(const QList<QContactActionTarget>&, const QVariantMap&)
    {
        // Well, do something
        emit stateChanged(QContactAction::FinishedState);
        return true;
    }

    QVariantMap results() const
    {
        return QVariantMap();
    }

    State state() const {return QContactAction::FinishedState;}

};

/* Static actions for testing matching */
class QPhoneNumberAction : public DummyAction
{
    Q_OBJECT

public:
    QPhoneNumberAction(QObject *parent = 0) : DummyAction(parent) {}
    ~QPhoneNumberAction() {}
};

class QPhoneNumberActionFactory : public QContactActionFactory
{
    Q_OBJECT

public:
    QPhoneNumberActionFactory(QObject* parent = 0) : QContactActionFactory(parent)
    {
        m_actionDescriptor = createDescriptor("PhoneNumberAction", "tst_qcontactmanagerfiltering:phonenumberaction", "phonenumberaction", 1);
    }

    ~QPhoneNumberActionFactory()
    {

    }

    QList<QContactActionDescriptor> actionDescriptors() const
    {
        QList<QContactActionDescriptor> retn;
        retn << m_actionDescriptor;
        return retn;
    }

    QContactAction* create(const QContactActionDescriptor& which) const
    {
        Q_UNUSED(which)
        return new QPhoneNumberAction;
    }

    QSet<QContactActionTarget> supportedTargets(const QContact& contact, const QContactActionDescriptor& which) const
    {
        Q_UNUSED(which)
        QSet<QContactActionTarget> retn;
        QList<QContactDetail> pnDets = contact.details(QContactPhoneNumber::DefinitionName);
        foreach (const QContactDetail& det, pnDets) {
            if (!det.value(QContactPhoneNumber::FieldNumber).isEmpty()) {
                QContactActionTarget curr(contact, det);
                retn << curr;
            }
        }

        return retn;
    }

    QContactFilter contactFilter(const QContactActionDescriptor& which) const
    {
        Q_UNUSED(which)
        QContactDetailFilter df;
        // XXX TODO: find some way to pass the defAndFieldNamesForTypeForActions value for Integer to this function...
        df.setDetailDefinitionName(QContactPhoneNumber::DefinitionName, QContactPhoneNumber::FieldNumber);
        return df;
    }

    QVariant metaData(const QString& key, const QList<QContactActionTarget>& targets, const QVariantMap& parameters, const QContactActionDescriptor& which) const
    {
        Q_UNUSED(key);
        Q_UNUSED(targets);
        Q_UNUSED(parameters);
        Q_UNUSED(which);
        return QVariant();
    }

    bool supportsContact(const QContact& contact, const QContactActionDescriptor& which) const
    {
        return supportedTargets(contact, which).isEmpty();
    }

private:
    QContactActionDescriptor m_actionDescriptor;
};

class QPhoneNumberActionPlugin : public QObject, public QServicePluginInterface
{
    Q_OBJECT
    Q_INTERFACES(QtMobility::QServicePluginInterface)

public:
    QObject* createInstance(const QServiceInterfaceDescriptor& descriptor,
                            QServiceContext* context,
                            QAbstractSecuritySession* session)
    {
        Q_UNUSED(context);
        Q_UNUSED(session);

        if (descriptor.interfaceName() == QContactActionFactory::InterfaceName)
            return new QPhoneNumberActionFactory(this);
        else
            return 0;
    }
};
Q_EXPORT_PLUGIN2(contacts_phonenumberaction, QPhoneNumberActionPlugin);

#endif
