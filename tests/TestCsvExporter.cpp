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

#include "core/Database.h"
#include "core/Entry.h"
#include "core/Group.h"
#include "core/CsvExporter.h"

#include "crypto/Crypto.h"

QTEST_GUILESS_MAIN(TestCsvExporter)

const QString TestCsvExporter::ExpectedHeaderLine = QString("\"Group\",\"Title\",\"Username\",\"Password\",\"URL\",\"Notes\"\n");

void TestCsvExporter::init()
{
    m_db = new Database();
    m_deviceWrapperMock = new DeviceWrapperMock();
    m_csvExporter = new CsvExporter();
}

void TestCsvExporter::initTestCase()
{
    Crypto::init();
}

void TestCsvExporter::cleanUp()
{
    delete m_db;
    delete m_csvExporter;
    delete m_deviceWrapperMock;
}

void TestCsvExporter::testExport()
{
    Group* groupRoot = m_db->rootGroup();
    Group* group= new Group();
    group->setName("Test Group Name");
    group->setParent(groupRoot);
    Entry* entry = new Entry();
    entry->setGroup(group);
    entry->setTitle("Test Entry Title");
    entry->setUsername("Test Username");
    entry->setPassword("Test Password");
    entry->setUrl("http://test.url");
    entry->setNotes("Test Notes");

    m_csvExporter->exportDatabase(m_deviceWrapperMock, m_db);

    QString expectedResult = QString().append(ExpectedHeaderLine).append("\"Test Group Name\",\"Test Entry Title\",\"Test Username\",\"Test Password\",\"http://test.url\",\"Test Notes\"\n");

    QCOMPARE(m_deviceWrapperMock->getResult(), expectedResult);
}

void TestCsvExporter::testEmptyDatabase()
{
    m_csvExporter->exportDatabase(m_deviceWrapperMock, m_db);
    QCOMPARE(m_deviceWrapperMock->getResult(), ExpectedHeaderLine);
}

void TestCsvExporter::testNestedGroups()
{
    Group* groupRoot = m_db->rootGroup();
    Group* group= new Group();
    group->setName("Test Group Name");
    group->setParent(groupRoot);
    Group* childGroup= new Group();
    childGroup->setName("Test Sub Group Name");
    childGroup->setParent(group);
    Entry* entry = new Entry();
    entry->setGroup(childGroup);
    entry->setTitle("Test Entry Title");

    m_csvExporter->exportDatabase(m_deviceWrapperMock, m_db);

    QCOMPARE(m_deviceWrapperMock->getResult(), QString().append(ExpectedHeaderLine).append("\"Test Group Name/Test Sub Group Name\",\"Test Entry Title\",\"\",\"\",\"\",\"\"\n"));
}
