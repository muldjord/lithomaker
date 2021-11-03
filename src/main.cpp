/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            main.cpp
 *
 *  Wed Nov 3 09:03:00 CEST 2021
 *  Copyright 2021 Lars Muldjord
 *  muldjordlars@gmail.com
 ****************************************************************************/
/*
 *  This file is part of LithoMaker.
 *
 *  LithoMaker is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  LithoMaker is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with LithoMaker; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */

#include <QApplication>
#include <QDir>
#include <QDateTime>
#include <QTranslator>
#include <QSettings>
#include <QTextStream>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "mainwindow.h"

QSettings *settings;

void customMessageHandler(QtMsgType type, const QMessageLogContext &, const QString &msg)
{
  // Only add to debug file for certain severities
  bool addToFile = false;

  // Add timestamp to debug message
  QString txt = QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss");
  // Decide which type of debug message it is, and add string to signify it
  // Then append the debug message itself to the same string.
  switch (type) {
  case QtDebugMsg:
    txt += QString(": Debug: %1").arg(msg);
    break;
  case QtInfoMsg:
    txt += QString(": Info: %1").arg(msg);
    break;
  case QtWarningMsg:
    addToFile = true;
    txt += QString(": Warning: %1").arg(msg);
    break;
  case QtCriticalMsg:
    addToFile = true;
    txt += QString(": Critical: %1").arg(msg);
    break;
  case QtFatalMsg:
    addToFile = true;
    txt += QString(": Fatal: %1").arg(msg);
    abort();
  }
  printf("%s", txt.toStdString().c_str());
  
  if(addToFile) {
    QFile outFile(QApplication::applicationDirPath() + "/debug.log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    if (txt.right(1) == "\n")
      ts << txt;
    else
      ts << txt << Qt::endl;
    outFile.close();
  }
}

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  // Install the custom debug message handler used by qDebug()
  qInstallMessageHandler(customMessageHandler);

  QTranslator translator;
  translator.load("lithomaker_" + QLocale::system().name());
  app.installTranslator(&translator);

  QDir::setCurrent(QApplication::applicationDirPath());

  QCommandLineParser parser;
  parser.addHelpOption();
  QCommandLineOption cOption("c", "Sets the config file to use", "FILENAME", "config.ini");
  parser.addOption(cOption);

  parser.process(app);

  QSettings s(parser.value("c"), QSettings::IniFormat);
  settings = &s;

  MainWindow window;
  window.show();
  return app.exec();
}

