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


#include "filepropertyparser.h"

#include <QFile>
#include <QXmlStreamReader>


#include <QDebug>


FilePropertyParser::FilePropertyParser(QString path) {
    fileType = nullptr;
    protocolVersion = -1;
    title = nullptr;
    description = nullptr;


    QFile file(path);

    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while(!in.atEnd()) {
            QString line = in.readLine();
            std::string stdstrline = line.toStdString();

            if(line.contains("<FightclubExchangeFile>", Qt::CaseSensitive)
               && line.contains("</FightclubExchangeFile>", Qt::CaseSensitive)) {
                unsigned first = stdstrline.find("<FightclubExchangeFile>")
                                 + QString("<FightclubExchangeFile>").length();
                unsigned last = stdstrline.find("</FightclubExchangeFile>");
                fileType = QString::fromStdString(stdstrline.substr(first,last-first));
            } else if(line.contains("<ExchangeProtocolVersion>", Qt::CaseSensitive)
                      && line.contains("</ExchangeProtocolVersion>", Qt::CaseSensitive)) {
                unsigned first = stdstrline.find("<FightclubExchangeFile>")
                                 + QString("<FightclubExchangeFile>").length();
                unsigned last = stdstrline.find("</FightclubExchangeFile>");
                protocolVersion = QString::fromStdString(stdstrline.substr(first,last-first)).toInt();
            } else if(line.contains("<ExchangeFileTitle>", Qt::CaseSensitive)
                      && line.contains("</ExchangeFileTitle>", Qt::CaseSensitive)) {
                unsigned first = stdstrline.find("<ExchangeFileTitle>")
                                 + QString("<ExchangeFileTitle>").length();
                unsigned last = stdstrline.find("</ExchangeFileTitle>");
                title = QString::fromStdString(stdstrline.substr(first,last-first));
            } else if(line.contains("<ExchangeFileContentDescr>", Qt::CaseSensitive)
                      && line.contains("</ExchangeFileContentDescr>", Qt::CaseSensitive)) {
                unsigned first = stdstrline.find("<ExchangeFileContentDescr>")
                                 + QString("<ExchangeFileContentDescr>").length();
                unsigned last = stdstrline.find("</ExchangeFileContentDescr>");
                description = QString::fromStdString(stdstrline.substr(first,last-first));
            }
        }
    }


    if(protocolVersion == -1)  protocolVersion = -1;
    if(title == nullptr)       title = path;
    if(description == nullptr) description = "No description provided";
}


QString FilePropertyParser::getFileType()        { return fileType; }
int     FilePropertyParser::getProtocolVersion() { return protocolVersion; }
QString FilePropertyParser::getTitle()           { return title; }
QString FilePropertyParser::getDescription()     { return description; }
