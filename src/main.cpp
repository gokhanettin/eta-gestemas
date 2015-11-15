/* Copyright (C) 2015 Gökhan Karabulut <gokhan.karabulut@tubitak.gov.tr>
 *
 * This file is part of eta-gestemas.
 *
 * eta-gestemas is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName(GESTEMAS_APPLICATION_NAME);
    QCoreApplication::setApplicationVersion(GESTEMAS_VERSION);

    QCommandLineParser parser;
    parser.setApplicationDescription(GESTEMAS_APPLICATION_DESCRIPTION);
    parser.addHelpOption();
    parser.addVersionOption();
    parser.process(app);

    return app.exec();
}
