/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            mainwindow.cpp
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

#include <stdio.h>
#include <QtWidgets>
#include <QSettings>

#include "mainwindow.h"
#include "aboutbox.h"
#include "configdialog.h"

extern QSettings *settings;

MainWindow::MainWindow()
{
  restoreGeometry(settings->value("windowState", "").toByteArray());

  if(!settings->contains("alwaysOnTop")) {
    showPreferences();
  }

  setWindowTitle("LithoMaker v" VERSION);

  if(settings->value("alwaysOnTop").toBool()) {
    setWindowFlags(Qt::WindowStaysOnTopHint);
  }

  createActions();
  createMenus();

  QLabel *inputLabel = new QLabel(tr("Input image filename:"));
  inputLineEdit = new QLineEdit(tr("example.png"));

  QLabel *exportLabel = new QLabel(tr("Export STL filename:"));
  exportLineEdit = new QLineEdit(tr("lithophane.stl"));

  QPushButton *renderButton = new QPushButton(tr("Render"));
  connect(renderButton, &QPushButton::clicked, this, &MainWindow::renderStl);

  QPushButton *exportButton = new QPushButton(tr("Export"));
  connect(exportButton, &QPushButton::clicked, this, &MainWindow::exportStl);

  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(inputLabel);
  layout->addWidget(inputLineEdit);
  layout->addWidget(exportLabel);
  layout->addWidget(exportLineEdit);
  layout->addWidget(renderButton);
  layout->addWidget(exportButton);

  setCentralWidget(new QWidget());
  centralWidget()->setLayout(layout);

  show();

}

MainWindow::~MainWindow()
{
  settings->setValue("windowState", saveGeometry());
}

void MainWindow::createActions()
{
  quitAct = new QAction("&Quit", this);
  quitAct->setIcon(QIcon(":quit.png"));
  connect(quitAct, &QAction::triggered, qApp, &QApplication::quit);

  preferencesAct = new QAction(tr("Edit &Preferences..."), this);
  preferencesAct->setIcon(QIcon(":preferences.png"));
  connect(preferencesAct, &QAction::triggered, this, &MainWindow::showPreferences);

  aboutAct = new QAction(tr("&About..."), this);
  aboutAct->setIcon(QIcon(":about.png"));
  connect(aboutAct, &QAction::triggered, this, &MainWindow::showAbout);
}

void MainWindow::createMenus()
{
  fileMenu = new QMenu(tr("&File"), this);
  fileMenu->addAction(quitAct);

  optionsMenu = new QMenu(tr("&Options"), this);
  optionsMenu->addAction(preferencesAct);

  helpMenu = new QMenu(tr("&Help"), this);
  helpMenu->addAction(aboutAct);

  menuBar = new QMenuBar();
  menuBar->addMenu(fileMenu);
  menuBar->addMenu(optionsMenu);
  menuBar->addMenu(helpMenu);
  
  setMenuBar(menuBar);
}

void MainWindow::showAbout()
{
  // Spawn about window
  AboutBox aboutBox(this);
  aboutBox.exec();
}

void MainWindow::showPreferences()
{
  // Spawn preferences dialog
  ConfigDialog preferences(this);
  preferences.exec();
}

QString MainWindow::getVertexString(const double &x, const double &y, const double &z)
{
  return QString("\t\t\tvertex " + QString::number((double)x, 'g') + " " + QString::number((double)y, 'g') + " " + QString::number((double)z, 'g') + "\n");
}

QString MainWindow::beginTriangle()
{
  return QString("\tfacet normal 0.0 0.0 0.0\n\t\touter loop\n");
}

QString MainWindow::endTriangle()
{
  return QString("\t\tendloop\n\tendfacet\n");
}

void MainWindow::renderStl()
{
  printf("Rendering STL...\n");
  QImage image(inputLineEdit->text());
  image.invertPixels();
  image = image.mirrored(false, true);
  stlString = "solid lithophane\n";
  for(int y = 0; y < image.height() - 1; ++y) {
    // Close left side
    stlString.append(beginTriangle());
    stlString.append(getVertexString(0, y, minThickness));
    stlString.append(getVertexString(0, y, image.pixelColor(0 , y).red() / factor));
    stlString.append(getVertexString(0, y + 1, image.pixelColor(0 , y + 1).red() / factor));
    stlString.append(endTriangle());
    
    stlString.append(beginTriangle());
    stlString.append(getVertexString(0, y + 1, image.pixelColor(0 , y + 1).red() / factor));
    stlString.append(getVertexString(0, y + 1, minThickness));
    stlString.append(getVertexString(0, y, minThickness));
    stlString.append(endTriangle());
    for(int x = 0; x < image.width() - 1; ++x) {
      if(y == 0) {
	// Close top
	stlString.append(beginTriangle());
	stlString.append(getVertexString(x + 1, 0, image.pixelColor(x + 1, 0).red() / factor));
	stlString.append(getVertexString(x, 0, image.pixelColor(x , 0).red() / factor));
	stlString.append(getVertexString(x, 0, minThickness));
	stlString.append(endTriangle());
	
	stlString.append(beginTriangle());
	stlString.append(getVertexString(x, 0, minThickness));
	stlString.append(getVertexString(x + 1, 0, minThickness));
	stlString.append(getVertexString(x + 1, 0, image.pixelColor(x + 1, 0).red() / factor));
	stlString.append(endTriangle());

	// Close bottom
	stlString.append(beginTriangle());
	stlString.append(getVertexString(x, image.height() - 1, minThickness));
	stlString.append(getVertexString(x, image.height() - 1, image.pixelColor(x , image.height() - 1).red() / factor));
	stlString.append(getVertexString(x + 1, image.height() - 1, image.pixelColor(x + 1, image.height() - 1).red() / factor));
	stlString.append(endTriangle());
	
	stlString.append(beginTriangle());
	stlString.append(getVertexString(x + 1, image.height() - 1, image.pixelColor(x + 1, image.height() - 1).red() / factor));
	stlString.append(getVertexString(x + 1, image.height() - 1, minThickness));
	stlString.append(getVertexString(x, image.height() - 1, minThickness));
	stlString.append(endTriangle());
      }
      stlString.append(beginTriangle());
      stlString.append(getVertexString(x, y, image.pixelColor(x, y).red() / factor));
      stlString.append(getVertexString(x + 1, y + 1, image.pixelColor(x + 1, y + 1).red() / factor));
      stlString.append(getVertexString(x, y + 1, image.pixelColor(x, y + 1).red() / factor));
      stlString.append(endTriangle());

      stlString.append(beginTriangle());
      stlString.append(getVertexString(x, y, image.pixelColor(x, y).red() / factor));
      stlString.append(getVertexString(x + 1, y, image.pixelColor(x + 1, y).red() / factor));
      stlString.append(getVertexString(x + 1, y + 1, image.pixelColor(x + 1, y + 1).red() / factor));
      stlString.append(endTriangle());
    }
    // Close right side
    stlString.append(beginTriangle());
    stlString.append(getVertexString(image.width() - 1, y + 1, image.pixelColor(image.width() - 1 , y + 1).red() / factor));
    stlString.append(getVertexString(image.width() - 1, y, image.pixelColor(image.width() - 1 , y).red() / factor));
    stlString.append(getVertexString(image.width() - 1, y, minThickness));
    stlString.append(endTriangle());
    
    stlString.append(beginTriangle());
    stlString.append(getVertexString(image.width() - 1, y, minThickness));
    stlString.append(getVertexString(image.width() - 1, y + 1, minThickness));
    stlString.append(getVertexString(image.width() - 1, y + 1, image.pixelColor(image.width() - 1 , y + 1).red() / factor));
    stlString.append(endTriangle());
  }
  // Create flat backside
  stlString.append(beginTriangle());
  stlString.append(getVertexString(0, 0, minThickness));
  stlString.append(getVertexString(image.width() - 1, image.height() - 1, minThickness));
  stlString.append(getVertexString(0, image.height() - 1, minThickness));
  stlString.append(endTriangle());
  stlString.append(beginTriangle());
  stlString.append(getVertexString(0, 0, minThickness));
  stlString.append(getVertexString(image.width() - 1, 0, minThickness));
  stlString.append(getVertexString(image.width() - 1, image.height() - 1, minThickness));
  stlString.append(endTriangle());

  stlString.append("endsolid\n");
  printf("Rendering finished...\n");
  QMessageBox::information(this, tr("Rendering completed"), tr("The 3D lithophane has been successfully rendered. You can now export it."));
}

void MainWindow::exportStl()
{
  printf("Exporting to file: '%s'... ", exportLineEdit->text().toStdString().c_str());
  QFile stlFile(exportLineEdit->text());
  if(stlFile.open(QIODevice::WriteOnly)) {
    stlFile.write(stlString.toUtf8());
    stlFile.close();
    printf("Success!\n");
    QMessageBox::information(this, tr("Export succeeded"), tr("The STL was successfully exported! You can now import it in your preferred 3D printing slicer."));
  } else {
    QMessageBox::warning(this, tr("Export failed"), tr("File could not be opened for writing. Please check export filename location permissions and try again."));
    printf("Failed!\n");
  }
}
