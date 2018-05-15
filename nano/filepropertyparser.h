/****************************************************************************
**
** Copyright (C) 2018 Sebastian Friedl
** Contact: sfr682k@t-online.de
**
** $QT_BEGIN_LICENSE:GPL$
** GNU General Public License Usage
** This file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.md included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** $QT_END_LICENSE$
**
****************************************************************************/


#ifndef FILEPROPERTYPARSER_H
#define FILEPROPERTYPARSER_H


#include <QString>

class FilePropertyParser {
private:
    QString fileType;
    int protocolVersion;
    QString title;
    QString description;

public:
    FilePropertyParser(QString);
    QString getFileType();
    int getProtocolVersion();
    QString getTitle();
    QString getDescription();
};

#endif // FILEPROPERTYPARSER_H
