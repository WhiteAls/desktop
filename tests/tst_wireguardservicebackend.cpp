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

    void findConfigErrorReturnsLatest()
    {
        const auto log = QStringLiteral(
            "2026-09-28 12:00:00.000000: [TUN] Unable to load configuration"
            " from path: Invalid Jc: \"abc\"\n"
            "2026-09-28 12:00:05.000000: [TUN] Starting pia-wgservice\n"
            "2026-09-28 12:00:05.100000: [TUN] Unable to set device"
            " configuration: IPC error -22: failed to parse I1: bad tag\n"
            "2026-09-28 12:00:05.200000: [TUN] Shutting down\n");

        QCOMPARE(WireguardServiceBackendDetail::findConfigError(log),
                 QStringLiteral("failed to parse I1: bad tag"));
    }

    void findConfigErrorLoadConfiguration()
    {
        QCOMPARE(WireguardServiceBackendDetail::findConfigError(QStringLiteral(
                     "2026-09-28 12:00:00.000000: [TUN] Unable to load"
                     " configuration from path: Invalid S1: \"70000\"\r\n")),
                 QStringLiteral("Invalid S1: \"70000\""));
    }

    void findConfigErrorWithoutError()
    {
        QVERIFY(WireguardServiceBackendDetail::findConfigError(QStringLiteral(
            "2026-09-28 12:00:00.000000: [TUN] Starting pia-wgservice\n")).isEmpty());
        QVERIFY(WireguardServiceBackendDetail::findConfigError({}).isEmpty());
    }
};

QTEST_GUILESS_MAIN(tst_wireguardservicebackend)
#include TEST_MOC