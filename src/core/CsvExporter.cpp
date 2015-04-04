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

#include "CsvExporter.h"

#include <QFile>

#include "core/Database.h"
#include "core/FileDeviceWrapper.h"
#include "core/Group.h"


bool CsvExporter::exportDatabase(const QString& filename, const Database* db)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Truncate)) {
        m_error = file.errorString();
        return false;
    }
    FileDeviceWrapper fileDeviceWrapper(&file);
    return exportDatabase(&fileDeviceWrapper, db);
}

bool CsvExporter::exportDatabase(DeviceWrapper* device, const Database* db)
{
    QString header;
    addColumn(header, "Group");
    addColumn(header, "Title");
    addColumn(header, "Username");
    addColumn(header, "Password");
    addColumn(header, "URL");
    header.append("\n");

    if (device->write(header) == -1) {
        m_error = device->errorString();
        return false;
    }

    return writeGroup(device, db->rootGroup());
}

QString CsvExporter::errorString() const
{
    return m_error;
}

bool CsvExporter::writeGroup(DeviceWrapper* device, const Group* group, QString groupPath)
{
    if (!groupPath.isEmpty()) {
        groupPath.append("/");
    }
    groupPath.append(group->name());

    Q_FOREACH (const Entry* entry, group->entries()) {
        QString line;

        addColumn(line, groupPath);
        addColumn(line, entry->title());
        addColumn(line, entry->username());
        addColumn(line, entry->password());
        addColumn(line, entry->url());

        line.append("\n");

        if (device->write(line) == -1) {
            m_error = device->errorString();
            return false;
        }
    }

    Q_FOREACH (const Group* child, group->children()) {
        if (!writeGroup(device, child, groupPath)) {
            return false;
        }
    }

    return true;
}

void CsvExporter::addColumn(QString& str, const QString& column)
{
    if (!str.isEmpty()) {
        str.append(",");
    }

    str.append("\"");
    str.append(QString(column).replace("\"", "\"\""));
    str.append("\"");
}
