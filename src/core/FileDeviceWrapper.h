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

#ifndef KEEPASSX_FILEDEVICEWRAPPER_H
#define KEEPASSX_FILEDEVICEWRAPPER_H

#include <QFile>

#include "core/DeviceWrapper.h"

class FileDeviceWrapper : public DeviceWrapper
{
public:
    FileDeviceWrapper(QFile* file);

    qint64 write(const QString& data);
    QString errorString() const;


private:
    QFile* m_file;
};

#endif // KEEPASSX_FILEDEVICEWRAPPER_H
