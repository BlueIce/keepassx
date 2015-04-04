/*
 *  Copyright (C) 2015 Felix Geyer <debfx@fobos.de>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 or (at your option)
 *  version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "TestCsvExporter.h"

#include <QTest>

#include "tests.h"

#include "core/CsvExporter.h"
#include "core/Database.h"
#include "core/Entry.h"
#include "core/Group.h"
#include "core/test/DeviceWrapperMock.h"
#include "crypto/Crypto.h"

QTEST_GUILESS_MAIN(TestCsvExporter)

void TestCsvExporter::test()
{
    Crypto::init();
    Database* db = new Database();


    Group* groupRoot = db->rootGroup();
    Group* group= new Group();
    group->setName("Test Group Name");
    group->setParent(groupRoot);
    Entry* entry = new Entry();
    entry->setGroup(group);
    entry->setTitle("Test Entry Title");

    CsvExporter csvExporter;
    DeviceWrapperMock* mock = new DeviceWrapperMock();
    csvExporter.exportDatabase(mock, db);

    QString expectedResult = QString().append("\"Group\",\"Title\",\"Username\",\"Password\",\"URL\"\n")
            .append("\"Test Group Name\",\"Test Entry Title\",\"\",\"\",\"\"\n");

    QCOMPARE(mock->getResult(), expectedResult);


}
