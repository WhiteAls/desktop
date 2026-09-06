// Copyright (c) 2026 Private Internet Access, Inc.
//
// This file is part of the Private Internet Access Desktop Client.
//
// The Private Internet Access Desktop Client is free software: you can
// redistribute it and/or modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation, either version 3 of
// the License, or (at your option) any later version.
//
// The Private Internet Access Desktop Client is distributed in the hope that
// it will be useful, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with the Private Internet Access Desktop Client.  If not, see
// <https://www.gnu.org/licenses/>.

#include "daemon/src/win/wireguardservicebackend.h"
#include <QtTest>

class tst_wireguardservicebackend : public QObject
{
    Q_OBJECT

private slots:
    void splitAwgConfigLines()
    {
        const auto lines =
            WireguardServiceBackendDetail::splitAwgConfigLines(
                QStringLiteral(" Jc = 4 \r\n\nI1 = value\r\n\r\nH1 = 1"));

        QCOMPARE(lines, QStringList({QStringLiteral(" Jc = 4 "),
                                     QStringLiteral("I1 = value"),
                                     QStringLiteral("H1 = 1")}));
        for(const auto &line : lines)
        {
            QVERIFY(!line.contains(QLatin1Char('\n')));
            QVERIFY(!line.contains(QLatin1Char('\r')));
        }
    }
};

QTEST_GUILESS_MAIN(tst_wireguardservicebackend)
#include TEST_MOC