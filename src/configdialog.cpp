/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            configdialog.cpp
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

#include <QtWidgets>

#include "configdialog.h"

ConfigDialog::ConfigDialog(QWidget *parent) : QDialog(parent)
{
  setFixedSize(600, 400);
  contentsWidget = new QListWidget;
  contentsWidget->setViewMode(QListView::IconMode);
  contentsWidget->setIconSize(QSize(64, 64));
  contentsWidget->setMovement(QListView::Static);
  contentsWidget->setMaximumWidth(100);
  contentsWidget->setMinimumWidth(100);
  contentsWidget->setSpacing(12);

  mainPage = new MainPage();
  renderPage = new RenderPage();
  exportPage = new ExportPage();
  
  pagesWidget = new QStackedWidget;
  pagesWidget->addWidget(mainPage);
  pagesWidget->addWidget(renderPage);
  pagesWidget->addWidget(exportPage);

  QPushButton *okButton = new QPushButton(tr("Ok"));

  createIcons();
  contentsWidget->setCurrentRow(0);

  connect(okButton, &QPushButton::clicked, this, &ConfigDialog::accept);

  QHBoxLayout *horizontalLayout = new QHBoxLayout;
  horizontalLayout->addWidget(contentsWidget);
  horizontalLayout->addWidget(pagesWidget, 1);

  QHBoxLayout *buttonsLayout = new QHBoxLayout;
  buttonsLayout->addStretch(1);
  buttonsLayout->addWidget(okButton);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addLayout(horizontalLayout);
  mainLayout->addLayout(buttonsLayout);
  setLayout(mainLayout);

  setWindowTitle(tr("Preferences"));
}

void ConfigDialog::createIcons()
{
  QListWidgetItem *mainButton = new QListWidgetItem(contentsWidget);
  mainButton->setIcon(QIcon(":mainconfig.png"));
  mainButton->setText(tr("Main"));
  mainButton->setTextAlignment(Qt::AlignHCenter);
  mainButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

  QListWidgetItem *renderButton = new QListWidgetItem(contentsWidget);
  renderButton->setIcon(QIcon(":renderconfig.png"));
  renderButton->setText(tr("Render"));
  renderButton->setTextAlignment(Qt::AlignHCenter);
  renderButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

  QListWidgetItem *exportButton = new QListWidgetItem(contentsWidget);
  exportButton->setIcon(QIcon(":exportconfig.png"));
  exportButton->setText(tr("Export"));
  exportButton->setTextAlignment(Qt::AlignHCenter);
  exportButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

  connect(contentsWidget, &QListWidget::currentItemChanged, this, &ConfigDialog::changePage);
}

void ConfigDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
  if(!current) {
    current = previous;
  }

  pagesWidget->setCurrentIndex(contentsWidget->row(current));
}
