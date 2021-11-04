/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            aboutbox.cpp
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

#include "aboutbox.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QFile>
#include <QTimer>
#include <QScrollBar>
#include <QScrollArea>
#include <QPushButton>
#include <QApplication>

AboutBox::AboutBox(QWidget *parent): QDialog(parent)
{
  setWindowTitle(tr("About Lithomaker"));
  setFixedSize(750,500);

  // Read AUTHORS data from file
  QFile file(":AUTHORS");
  QByteArray authorsText;
  if(file.open(QIODevice::ReadOnly)) {
    authorsText = file.readAll();
    file.close();
  } else {
    printf("ERROR: Couldn't find AUTHORS file at the expected location.\n");
    authorsText = "ERROR: File not found...";
  }

  // Read COPYING data from file
  file.setFileName(":LICENSE");
  QByteArray gplText;
  if(file.open(QIODevice::ReadOnly)) {
    gplText = file.readAll();
    file.close();
  } else {
    printf("ERROR: Couldn't find LICENSE file at the expected location.\n");
    gplText = "ERROR: File not found...";
  }

  QVBoxLayout *layout = new QVBoxLayout();

  QHBoxLayout *topLayout = new QHBoxLayout();
  QLabel *logo = new QLabel();
  logo->setPixmap(QPixmap(":icon.png"));
  QLabel *title = new QLabel;
  title->setWordWrap(true);
  title->setText(tr("<h1>LithoMaker</h1>"
                    "<h2>Version " VERSION "</h2>"
                    "Copyright (C) 2021 Lars Bisballe Jensen<br />"
                    "LithoMaker is free software released under the terms "
                    "of the GNU General Public License. Click the license "
                    "tab for full license information."));
  topLayout->addWidget(logo);
  topLayout->addWidget(title);
  topLayout->setStretch(1, 1);
  topLayout->setStretch(2, 2);

  QLabel *releaseInfo = new QLabel(authorsText);
  releaseInfo->setStyleSheet("QLabel { background-color : white; }");

  QScrollArea *releaseInfoScroll = new QScrollArea();
  releaseInfoScroll->setWidget(releaseInfo);
  releaseInfoScroll->setStyleSheet("QScrollArea { background-color : white; }");

  QLabel *license = new QLabel(gplText);
  license->setStyleSheet("QLabel { font-family : monospace; "
                     "background-color : white; }");
  licenseScroll = new QScrollArea();
  licenseScroll->setStyleSheet("QScrollArea { background-color : white; }");
  licenseScroll->setWidget(license);

  connect(licenseScroll->verticalScrollBar(), &QScrollBar::sliderPressed, this, &AboutBox::noMoreScroll);
  tabs = new QTabWidget();
  connect(tabs, &QTabWidget::currentChanged, this, &AboutBox::checkTab);
  tabs->addTab(releaseInfoScroll, "Release Info");
  tabs->addTab(licenseScroll, "License");

  layout->addLayout(topLayout);
  layout->addWidget(tabs);
  setLayout(layout);

  scrollState = 0; // 0 = initial state
                   // 1 = initiate scroll
                   // 2 = scroll 1 step
                   // 3 = no more scroll

  scrollTimer = new QTimer();
  scrollTimer->setSingleShot(true);
  connect(scrollTimer, &QTimer::timeout, this, &AboutBox::scroll);
}

void AboutBox::mousePressEvent(QMouseEvent * event)
{
  if(event) { // TODO: Perhaps do a mouse click check on the event
    accept();
  }
}

void AboutBox::scroll()
{
  switch(scrollState) {
  case 0 : // 0 = initial state
    scrollState = 1;
    scrollTimer->setInterval(5000);
    scrollTimer->start();
    break;
  case 1 : // 1 = initiate scroll
    scrollState = 2;
    licenseScroll->verticalScrollBar()->setValue(0);
    scrollTimer->setInterval(200);
    scrollTimer->start();
    break;
  case 2 : // 2 = scroll 1 step
    licenseScroll->verticalScrollBar()->
      setValue(licenseScroll->verticalScrollBar()->value() + 2);
    if(licenseScroll->verticalScrollBar()->value() >=
       licenseScroll->verticalScrollBar()->maximum()) {
      scrollState = 0;
    }
    scrollTimer->start();
    break;
  case 3 : // 3 = no more scroll
    scrollTimer->stop();
    break;
  }
}

void AboutBox::noMoreScroll()
{
  scrollState = 3;
}

void AboutBox::checkTab(int tabIndex)
{
  if(tabIndex == 1) {
    scrollState = 0;
    scrollTimer->stop();
    scroll();
  }
}
