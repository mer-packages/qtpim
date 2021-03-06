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

import QtQuick 2.0
import QtTest 1.0
import QtOrganizer 5.0

Rectangle {
    id: test;

    QOrganizerTestUtility {
        id: utility
    }

    Collection {
        id: testCollection
        name: 'My collection filter test'
        description: 'collection filter test'
    }

    TestCase {
        name: "Organizer collection filter test"

        function test_collectionFilter_data() {
            return [
                        { tag:  "properties",
                            code: "import QtOrganizer 5.0;CollectionFilter{}"
                        },
                    ]
        }

        function test_collectionFilter(data) {

            var list = utility.getManagerList();
            if (list.length < 0) {
                console.log("No manager to test");
                return;
            }
            //Test all manager backends
            for (var i = 0; i < list.length; i ++) {
                var managerName = list[i];
                var debugFlag = 0;
                console.log("test_collectionFilter test start! :" + managerName);
                var model = Qt.createQmlObject(
                        "import QtOrganizer 5.0;"
                        + "OrganizerModel {"
                        + "   manager: \"qtorganizer:" + managerName + ":id=qml\";"
                        + "   startPeriod:'2009-01-01';"
                        + "   endPeriod:'2012-12-31';"
                        + "   autoUpdate:true; }"
                        , test);
                utility.init(model);
                utility.waitModelChange();
                utility.empty_calendar();

                var collectionFilter = Qt.createQmlObject(data.code, test);

                var event = Qt.createQmlObject(
                        "import QtOrganizer 5.0;"
                        + "Event { "
                        + "   displayLabel: \"organizer collection filter test event\"; "
                        + "   description: \"organizer collection filter test event\"; "
                        + "   startDateTime: '2010-12-12'; "
                        + "   endDateTime: '2010-12-13'; }"
                        , test);

                //------Create and save the detail test------//
                var collectionLength = model.collections.length;
                model.saveCollection(testCollection)

                //Let's wait for the model to be up-to-date
                utility.waitModelChange(collectionLength + 1, utility.collectionChange);

                //we should have more than default collection now
                utility.debug("New collection length :" + model.collections.length, debugFlag);
                var savedCollection = model.collections[model.collections.length - 1];
                utility.debug("New collection id :" + savedCollection.collectionId, debugFlag);
                event.collectionId = savedCollection.collectionId;
                model.saveItem(event);
                utility.waitModelChange(1, utility.itemChange);

                var fetchlist = model.items;
                compare(model.itemCount, 1)
                var savedEvent = fetchlist[0];
                utility.debug("New event collection id :" + savedEvent.collectionId, debugFlag);
                compare(savedEvent.collectionId, event.collectionId);

                //Change collection filter id
                collectionFilter.ids = [event.collectionId];
                model.filter = collectionFilter;
                utility.waitModelChange(1);
                compare(model.itemCount, 1)

                //default collection
                utility.debug("default collection id :" + model.defaultCollection().collectionId, debugFlag);
                var modelCollectionFilter = model.filter;
                modelCollectionFilter.ids = [model.defaultCollection().collectionId];
                utility.waitModelChange(0);

                compare(model.itemCount, 0)
                //save event to default collection
                event.collectionId = model.defaultCollection().collectionId;
                model.saveItem(event);
                utility.waitModelChange(1);

                //empty ides:
                modelCollectionFilter.ids = [];
                utility.waitModelChange(0);
                compare(model.itemCount, 0)

                //remove collection filter or set empty filter
                model.filter = null;
                utility.waitModelChange(2);
                compare(model.itemCount, 2);

                //reset back filter
                collectionFilter = Qt.createQmlObject(data.code, test);
                collectionFilter.ids = [event.collectionId];
                model.filter = collectionFilter;
                utility.waitModelChange(1);
                compare(model.itemCount, 1);

                //set more collection ids
                modelCollectionFilter = model.filter
                modelCollectionFilter.ids = [model.defaultCollection().collectionId, savedCollection.collectionId];
                utility.waitModelChange(2);
                compare(model.itemCount, 2);

                //One invalid collection id
                modelCollectionFilter.ids = [model.defaultCollection().collectionId, "12345666666",savedCollection.collectionId];
                utility.waitModelChange(2);
                compare(model.itemCount, 2);

                utility.empty_calendar();
            }
        }
    }
}
