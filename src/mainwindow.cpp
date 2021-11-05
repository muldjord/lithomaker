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
  if(settings->contains("main/windowState")) {
    restoreGeometry(settings->value("main/windowState", "").toByteArray());
  } else {
    setMinimumWidth(640);
  }

  setWindowTitle("LithoMaker v" VERSION);

  createActions();
  createMenus();

  QLabel *minThicknessLabel = new QLabel(tr("Minimum thickness (mm):"));
  minThicknessLineEdit = new QLineEdit(settings->value("main/minThickness", "0.8").toString());

  QLabel *totalThicknessLabel = new QLabel(tr("Total thickness (mm):"));
  totalThicknessLineEdit = new QLineEdit(settings->value("main/totalThickness", "4.0").toString());

  QLabel *borderLabel = new QLabel(tr("Frame border (mm):"));
  borderLineEdit = new QLineEdit(settings->value("main/frameBorder", "3.0").toString());

  QLabel *widthLabel = new QLabel(tr("Width, including frame borders (mm):"));
  widthLineEdit = new QLineEdit(settings->value("main/width", "150.0").toString());

  QLabel *inputLabel = new QLabel(tr("Input image filename:"));
  inputLineEdit = new QLineEdit(settings->value("main/inputFilePath", "example.png").toString());
  QPushButton *inputButton = new QPushButton(tr("..."));
  connect(inputButton, &QPushButton::clicked, this, &MainWindow::inputSelect);
  QHBoxLayout *inputLayout = new QHBoxLayout();
  inputLayout->addWidget(inputLineEdit);
  inputLayout->addWidget(inputButton);

  QPushButton *renderButton = new QPushButton(tr("Render"));
  connect(renderButton, &QPushButton::clicked, this, &MainWindow::renderStl);
  renderProgress = new QProgressBar(this);
  renderProgress->setMinimum(0);
  
  QLabel *exportLabel = new QLabel(tr("Export STL filename:"));
  exportLineEdit = new QLineEdit(settings->value("main/exportFilePath", "lithophane.stl").toString());
  QPushButton *exportButton = new QPushButton(tr("..."));
  connect(exportButton, &QPushButton::clicked, this, &MainWindow::exportSelect);
  QHBoxLayout *exportLayout = new QHBoxLayout();
  exportLayout->addWidget(exportLineEdit);
  exportLayout->addWidget(exportButton);

  QPushButton *exportNowButton = new QPushButton(tr("Export"));
  connect(exportNowButton, &QPushButton::clicked, this, &MainWindow::exportStl);

  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(minThicknessLabel);
  layout->addWidget(minThicknessLineEdit);
  layout->addWidget(totalThicknessLabel);
  layout->addWidget(totalThicknessLineEdit);
  layout->addWidget(borderLabel);
  layout->addWidget(borderLineEdit);
  layout->addWidget(widthLabel);
  layout->addWidget(widthLineEdit);
  layout->addWidget(inputLabel);
  layout->addLayout(inputLayout);
  layout->addWidget(renderProgress);
  layout->addWidget(renderButton);
  layout->addWidget(exportLabel);
  layout->addLayout(exportLayout);
  layout->addWidget(exportNowButton);

  setCentralWidget(new QWidget());
  centralWidget()->setLayout(layout);

  show();

}

MainWindow::~MainWindow()
{
  settings->setValue("main/windowState", saveGeometry());
  settings->setValue("main/minThickness", minThicknessLineEdit->text());
  settings->setValue("main/totalThickness", totalThicknessLineEdit->text());
  settings->setValue("main/frameBorder", borderLineEdit->text());
  settings->setValue("main/width", widthLineEdit->text());
  settings->setValue("main/inputFilePath", inputLineEdit->text());
  settings->setValue("main/exportFilePath", exportLineEdit->text());
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

void MainWindow::renderStl()
{
  if(!QFileInfo::exists(inputLineEdit->text())) {
    QMessageBox::warning(this, tr("File not found"), tr("Input file doesn't exist. Please check filename and permissions."));
    return;
  }
  if(borderLineEdit->text().toDouble() * 2 > widthLineEdit->text().toDouble()) {
    QMessageBox::warning(this, tr("Border too thick"), tr("The chosen frame border size exceeds the size of the total lithophane width. Please correct this."));
    return;
  }
  
  printf("Rendering STL...\n");
  QImage image(inputLineEdit->text());
  image.invertPixels();
  stlString = "solid lithophane\n";
  border = borderLineEdit->text().toDouble();
  depthFactor = (totalThicknessLineEdit->text().toDouble() - minThicknessLineEdit->text().toDouble()) / 255.0;
  widthFactor = (widthLineEdit->text().toDouble() - (border * 2)) / image.width();
  double minThickness = minThicknessLineEdit->text().toDouble() * -1;
  renderProgress->setMaximum(image.height() - 1);
  renderProgress->setValue(0);
  renderProgress->setFormat(tr("Rendering %p%"));
  for(int y = 0; y < image.height() - 1; ++y) {
    // Close left side
    stlString.append(beginTriangle());
    stlString.append(getVertexString(0, y, minThickness, true));
    stlString.append(getVertexString(0, y, getPixel(image, 0, y) * depthFactor, true));
    stlString.append(getVertexString(0, y + 1, getPixel(image, 0, y + 1) * depthFactor, true));
    stlString.append(endTriangle());
    
    stlString.append(beginTriangle());
    stlString.append(getVertexString(0, y + 1, getPixel(image, 0, y + 1) * depthFactor, true));
    stlString.append(getVertexString(0, y + 1, minThickness, true));
    stlString.append(getVertexString(0, y, minThickness, true));
    stlString.append(endTriangle());
    for(int x = 0; x < image.width() - 1; ++x) {
      if(y == 0) {
        // Close top
        stlString.append(beginTriangle());
        stlString.append(getVertexString(x + 1, 0, getPixel(image, x + 1, 0) * depthFactor, true));
        stlString.append(getVertexString(x, 0, getPixel(image, x, 0) * depthFactor, true));
        stlString.append(getVertexString(x, 0, minThickness, true));
        stlString.append(endTriangle());
	
        stlString.append(beginTriangle());
        stlString.append(getVertexString(x, 0, minThickness, true));
        stlString.append(getVertexString(x + 1, 0, minThickness, true));
        stlString.append(getVertexString(x + 1, 0, getPixel(image, x + 1, 0) * depthFactor, true));
        stlString.append(endTriangle());

        // Close bottom
        stlString.append(beginTriangle());
        stlString.append(getVertexString(x, image.height() - 1, minThickness, true));
        stlString.append(getVertexString(x, image.height() - 1, getPixel(image, x, image.height() - 1) * depthFactor, true));
        stlString.append(getVertexString(x + 1, image.height() - 1, getPixel(image, x + 1, image.height() - 1) * depthFactor, true));
        stlString.append(endTriangle());
	
        stlString.append(beginTriangle());
        stlString.append(getVertexString(x + 1, image.height() - 1, getPixel(image, x + 1, image.height() - 1) * depthFactor, true));
        stlString.append(getVertexString(x + 1, image.height() - 1, minThickness, true));
        stlString.append(getVertexString(x, image.height() - 1, minThickness, true));
        stlString.append(endTriangle());
      }
      // The lithophane heightmap
      stlString.append(beginTriangle());
      stlString.append(getVertexString(x, y, getPixel(image, x, y) * depthFactor, true));
      stlString.append(getVertexString(x + 1, y + 1, getPixel(image, x + 1, y + 1) * depthFactor, true));
      stlString.append(getVertexString(x, y + 1, getPixel(image, x, y + 1) * depthFactor, true));
      stlString.append(endTriangle());

      stlString.append(beginTriangle());
      stlString.append(getVertexString(x, y, getPixel(image, x, y) * depthFactor, true));
      stlString.append(getVertexString(x + 1, y, getPixel(image, x + 1, y) * depthFactor, true));
      stlString.append(getVertexString(x + 1, y + 1, getPixel(image, x + 1, y + 1) * depthFactor, true));
      stlString.append(endTriangle());
    }
    // Close right side
    stlString.append(beginTriangle());
    stlString.append(getVertexString(image.width() - 1, y + 1, getPixel(image, image.width() - 1, y + 1) * depthFactor, true));
    stlString.append(getVertexString(image.width() - 1, y, getPixel(image, image.width() - 1, y) * depthFactor, true));
    stlString.append(getVertexString(image.width() - 1, y, minThickness, true));
    stlString.append(endTriangle());
    
    stlString.append(beginTriangle());
    stlString.append(getVertexString(image.width() - 1, y, minThickness, true));
    stlString.append(getVertexString(image.width() - 1, y + 1, minThickness, true));
    stlString.append(getVertexString(image.width() - 1, y + 1, getPixel(image, image.width() - 1, y + 1) * depthFactor, true));
    stlString.append(endTriangle());
    renderProgress->setValue(renderProgress->value() + 1);
  }
  renderProgress->setValue(renderProgress->maximum());
  renderProgress->setFormat("Ready!");

  // Backside
  stlString.append(beginTriangle());
  stlString.append(getVertexString(0, 0, minThickness, true));
  stlString.append(getVertexString(image.width() - 1, image.height() - 1, minThickness, true));
  stlString.append(getVertexString(0, image.height() - 1, minThickness, true));
  stlString.append(endTriangle());
  stlString.append(beginTriangle());
  stlString.append(getVertexString(0, 0, minThickness, true));
  stlString.append(getVertexString(image.width() - 1, 0, minThickness, true));
  stlString.append(getVertexString(image.width() - 1, image.height() - 1, minThickness, true));
  stlString.append(endTriangle());

  // Stabilizers
  if(settings->value("render/enableStabilizers", true).toBool()) {
    stlString.append(addStabilizer(0, ((border * 2) + (image.height() * widthFactor)) * 0.15));
    stlString.append(addStabilizer(widthLineEdit->text().toDouble() - (border < 4?border:4), ((border * 2) + (image.height() * widthFactor)) * 0.15));
  }

  // Frame
  stlString.append(addFrame(widthLineEdit->text().toDouble(), (border * 2) + (image.height() * widthFactor)));
  
  stlString.append("endsolid\n");
  printf("Rendering finished...\n");
}

void MainWindow::exportStl()
{
  if(stlString.isEmpty()) {
    QMessageBox::warning(this, tr("Empty STL buffer"), tr("There is currently no rendered lithophane in the STL buffer. You need to render one before you can export it."));
    return;
  }
  printf("Exporting to file: '%s'... ", exportLineEdit->text().toStdString().c_str());
  QFile stlFile(exportLineEdit->text());
  if(stlFile.open(QIODevice::WriteOnly)) {
    stlFile.write(stlString.toUtf8());
    stlFile.close();
    printf("Success!\n");
    QMessageBox::information(this, tr("Export succeeded"), tr("The STL was successfully exported. You can now import it in your preferred 3D printing slicer."));
  } else {
    QMessageBox::warning(this, tr("Export failed"), tr("File could not be opened for writing. Please check export filename and try again."));
    printf("Failed!\n");
  }
}

QString MainWindow::addStabilizer(const double &x, const double &height)
{
  double z = totalThicknessLineEdit->text().toDouble() - minThicknessLineEdit->text().toDouble() + 1;
  double depth = height * 0.5;

  QString stabilizer = "";
  // Front
  stabilizer.append(beginTriangle());
  stabilizer.append(getVertexString(x + (border < 4?border:4), 0, z));
  stabilizer.append(getVertexString(x + (border < 4?border:4), height, z));
  stabilizer.append(getVertexString(x, height, z));
  stabilizer.append(endTriangle());
  stabilizer.append(beginTriangle());
  stabilizer.append(getVertexString(x, height, z));
  stabilizer.append(getVertexString(x, 0, z));
  stabilizer.append(getVertexString(x + (border < 4?border:4), 0, z));
  stabilizer.append(endTriangle());
  stabilizer.append(beginTriangle());
  stabilizer.append(getVertexString(x, 0, z));
  stabilizer.append(getVertexString(x, 0, z + depth));
  stabilizer.append(getVertexString(x + (border < 4?border:4), 0, z));
  stabilizer.append(endTriangle());
  stabilizer.append(beginTriangle());
  stabilizer.append(getVertexString(x + (border < 4?border:4), 0, z));
  stabilizer.append(getVertexString(x, 0, z + depth));
  stabilizer.append(getVertexString(x + (border < 4?border:4), 0, z + depth));
  stabilizer.append(endTriangle());
  stabilizer.append(beginTriangle());
  stabilizer.append(getVertexString(x, 0, z));
  stabilizer.append(getVertexString(x, height, z));
  stabilizer.append(getVertexString(x, 0, z + depth));
  stabilizer.append(endTriangle());
  stabilizer.append(beginTriangle());
  stabilizer.append(getVertexString(x + (border < 4?border:4), 0, z + depth));
  stabilizer.append(getVertexString(x + (border < 4?border:4), height, z));
  stabilizer.append(getVertexString(x + (border < 4?border:4), 0, z));
  stabilizer.append(endTriangle());
  stabilizer.append(beginTriangle());
  stabilizer.append(getVertexString(x, 0, z + depth));
  stabilizer.append(getVertexString(x, height, z));
  stabilizer.append(getVertexString(x + (border < 4?border:4), height, z));
  stabilizer.append(endTriangle());
  stabilizer.append(beginTriangle());
  stabilizer.append(getVertexString(x + (border < 4?border:4), height, z));
  stabilizer.append(getVertexString(x + (border < 4?border:4), 0, z + depth));
  stabilizer.append(getVertexString(x, 0, z + depth));
  stabilizer.append(endTriangle());

  // Front cubes
  stabilizer.append(addCube(x, height - 4, z - 1, 1));
  stabilizer.append(addCube(x + (border < 4?border:4) - 1, height - 4, z - 1, 1));

  z = (minThicknessLineEdit->text().toDouble() * -1) - 1;
  // Back
  stabilizer.append(beginTriangle());
  stabilizer.append(getVertexString(x, height, z));
  stabilizer.append(getVertexString(x + (border < 4?border:4), height, z));
  stabilizer.append(getVertexString(x + (border < 4?border:4), 0, z));
  stabilizer.append(endTriangle());
  stabilizer.append(beginTriangle());
  stabilizer.append(getVertexString(x + (border < 4?border:4), 0, z));
  stabilizer.append(getVertexString(x, 0, z));
  stabilizer.append(getVertexString(x, height, z));
  stabilizer.append(endTriangle());
  stabilizer.append(beginTriangle());
  stabilizer.append(getVertexString(x + (border < 4?border:4), 0, z));
  stabilizer.append(getVertexString(x, 0, z - depth));
  stabilizer.append(getVertexString(x, 0, z));
  stabilizer.append(endTriangle());
  stabilizer.append(beginTriangle());
  stabilizer.append(getVertexString(x + (border < 4?border:4), 0, z - depth));
  stabilizer.append(getVertexString(x, 0, z - depth));
  stabilizer.append(getVertexString(x + (border < 4?border:4), 0, z));
  stabilizer.append(endTriangle());
  stabilizer.append(beginTriangle());
  stabilizer.append(getVertexString(x, 0, z - depth));
  stabilizer.append(getVertexString(x, height, z));
  stabilizer.append(getVertexString(x, 0, z));
  stabilizer.append(endTriangle());
  stabilizer.append(beginTriangle());
  stabilizer.append(getVertexString(x + (border < 4?border:4), 0, z));
  stabilizer.append(getVertexString(x + (border < 4?border:4), height, z));
  stabilizer.append(getVertexString(x + (border < 4?border:4), 0, z - depth));
  stabilizer.append(endTriangle());
  stabilizer.append(beginTriangle());
  stabilizer.append(getVertexString(x + (border < 4?border:4), height, z));
  stabilizer.append(getVertexString(x, height, z));
  stabilizer.append(getVertexString(x, 0, z - depth));
  stabilizer.append(endTriangle());
  stabilizer.append(beginTriangle());
  stabilizer.append(getVertexString(x, 0, z - depth));
  stabilizer.append(getVertexString(x + (border < 4?border:4), 0, z - depth));
  stabilizer.append(getVertexString(x + (border < 4?border:4), height, z));
  stabilizer.append(endTriangle());

  // Back cubes
  stabilizer.append(addCube(x, height - 4, z, 1));
  stabilizer.append(addCube(x + (border < 4?border:4) - 1, height - 4, z, 1));

  return stabilizer;
}

QString MainWindow::addCube(const double &x, const double &y, const double &z, const double &size)
{
  QString cube = "";
  cube.append(beginTriangle());
  cube.append(getVertexString(x, y, z));
  cube.append(getVertexString(x, y + size, z));
  cube.append(getVertexString(x, y + size, z + size));
  cube.append(endTriangle());
  cube.append(beginTriangle());
  cube.append(getVertexString(x, y + size, z + size));
  cube.append(getVertexString(x, y, z + size));
  cube.append(getVertexString(x, y, z));
  cube.append(endTriangle());
  cube.append(beginTriangle());
  cube.append(getVertexString(x, y, z + size));
  cube.append(getVertexString(x, y + size, z + size));
  cube.append(getVertexString(x + size, y + size, z + size));
  cube.append(endTriangle());
  cube.append(beginTriangle());
  cube.append(getVertexString(x + size, y + size, z + size));
  cube.append(getVertexString(x + size, y, z + size));
  cube.append(getVertexString(x, y, z + size));
  cube.append(endTriangle());
  cube.append(beginTriangle());
  cube.append(getVertexString(x, y + size, z));
  cube.append(getVertexString(x + size, y + size, z));
  cube.append(getVertexString(x, y + size, z + size));
  cube.append(endTriangle());
  cube.append(beginTriangle());
  cube.append(getVertexString(x, y + size, z + size));
  cube.append(getVertexString(x + size, y + size, z));
  cube.append(getVertexString(x + size, y + size, z + size));
  cube.append(endTriangle());
  cube.append(beginTriangle());
  cube.append(getVertexString(x, y, z));
  cube.append(getVertexString(x + size, y, z));
  cube.append(getVertexString(x, y + size, z));
  cube.append(endTriangle());
  cube.append(beginTriangle());
  cube.append(getVertexString(x, y + size, z));
  cube.append(getVertexString(x + size, y, z));
  cube.append(getVertexString(x + size, y + size, z));
  cube.append(endTriangle());
  cube.append(beginTriangle());
  cube.append(getVertexString(x, y, z));
  cube.append(getVertexString(x, y, z + size));
  cube.append(getVertexString(x + size, y, z));
  cube.append(endTriangle());
  cube.append(beginTriangle());
  cube.append(getVertexString(x + size, y, z));
  cube.append(getVertexString(x, y, z + size));
  cube.append(getVertexString(x + size, y, z + size));
  cube.append(endTriangle());
  cube.append(beginTriangle());
  cube.append(getVertexString(x + size, y, z));
  cube.append(getVertexString(x + size, y, z + size));
  cube.append(getVertexString(x + size, y + size, z));
  cube.append(endTriangle());
  cube.append(beginTriangle());
  cube.append(getVertexString(x + size, y + size, z));
  cube.append(getVertexString(x + size, y, z + size));
  cube.append(getVertexString(x + size, y + size, z + size));
  cube.append(endTriangle());
  return cube;
}

QString MainWindow::addFrame(const double &width, const double &height)
{
  double minThickness = minThicknessLineEdit->text().toDouble();
  double depth = totalThicknessLineEdit->text().toDouble() - minThickness;
  double frameSlope = depth / settings->value("render/frameSlopeFactor", "1.5").toDouble();
  QString frame = "";
  frame.append(beginTriangle());
  frame.append(getVertexString(width, height, - minThickness));
  frame.append(getVertexString(0.000000, height, - minThickness));
  frame.append(getVertexString(0.000000, height, depth));
  frame.append(endTriangle());
  frame.append(beginTriangle());
  frame.append(getVertexString(width, height, - minThickness));
  frame.append(getVertexString(0.000000, height, depth));
  frame.append(getVertexString(width, height, depth));
  frame.append(endTriangle());
  frame.append(beginTriangle());
  frame.append(getVertexString(width - border - frameSlope, border + frameSlope, 0.100000));
  frame.append(getVertexString(width - border - frameSlope, height - border - frameSlope, 0.100000));
  frame.append(getVertexString(border + frameSlope, height - border - frameSlope, 0.100000));
  frame.append(endTriangle());
  frame.append(beginTriangle());
  frame.append(getVertexString(width - border - frameSlope, border + frameSlope, 0.100000));
  frame.append(getVertexString(border + frameSlope, height - border - frameSlope, 0.100000));
  frame.append(getVertexString(border + frameSlope, border + frameSlope, 0.100000));
  frame.append(endTriangle());
  frame.append(beginTriangle());
  frame.append(getVertexString(0.000000, 0.000000, depth));
  frame.append(getVertexString(0.000000, height, depth));
  frame.append(getVertexString(0.000000, height, - minThickness));
  frame.append(endTriangle());
  frame.append(beginTriangle());
  frame.append(getVertexString(0.000000, 0.000000, depth));
  frame.append(getVertexString(0.000000, height, - minThickness));
  frame.append(getVertexString(0.000000, 0.000000, - minThickness));
  frame.append(endTriangle());
  frame.append(beginTriangle());
  frame.append(getVertexString(0.000000, 0.000000, - minThickness));
  frame.append(getVertexString(width, 0.000000, - minThickness));
  frame.append(getVertexString(width, 0.000000, depth));
  frame.append(endTriangle());
  frame.append(beginTriangle());
  frame.append(getVertexString(0.000000, 0.000000, - minThickness));
  frame.append(getVertexString(width, 0.000000, depth));
  frame.append(getVertexString(0.000000, 0.000000, depth));
  frame.append(endTriangle());
  frame.append(beginTriangle());
  frame.append(getVertexString(width, 0.000000, - minThickness));
  frame.append(getVertexString(width, height, - minThickness));
  frame.append(getVertexString(width, height, depth));
  frame.append(endTriangle());
  frame.append(beginTriangle());
  frame.append(getVertexString(width, 0.000000, - minThickness));
  frame.append(getVertexString(width, height, depth));
  frame.append(getVertexString(width, 0.000000, depth));
  frame.append(endTriangle());
  frame.append(beginTriangle());
  frame.append(getVertexString(0.000000, 0.000000, - minThickness));
  frame.append(getVertexString(0.000000, height, - minThickness));
  frame.append(getVertexString(width, height, - minThickness));
  frame.append(endTriangle());
  frame.append(beginTriangle());
  frame.append(getVertexString(0.000000, 0.000000, - minThickness));
  frame.append(getVertexString(width, height, - minThickness));
  frame.append(getVertexString(width, 0.000000, - minThickness));
  frame.append(endTriangle());
  frame.append(beginTriangle());
  frame.append(getVertexString(border, border, depth));
  frame.append(getVertexString(border, height - border, depth));
  frame.append(getVertexString(0.000000, height, depth));
  frame.append(endTriangle());
  frame.append(beginTriangle());
  frame.append(getVertexString(border, border, depth));
  frame.append(getVertexString(0.000000, height, depth));
  frame.append(getVertexString(0.000000, 0.000000, depth));
  frame.append(endTriangle());
  frame.append(beginTriangle());
  frame.append(getVertexString(width - border, height - border, depth));
  frame.append(getVertexString(width - border, border, depth));
  frame.append(getVertexString(width, 0.000000, depth));
  frame.append(endTriangle());
  frame.append(beginTriangle());
  frame.append(getVertexString(width - border, height - border, depth));
  frame.append(getVertexString(width, 0.000000, depth));
  frame.append(getVertexString(width, height, depth));
  frame.append(endTriangle());
  frame.append(beginTriangle());
  frame.append(getVertexString(border, height - border, depth));
  frame.append(getVertexString(width - border, height - border, depth));
  frame.append(getVertexString(width, height, depth));
  frame.append(endTriangle());
  frame.append(beginTriangle());
  frame.append(getVertexString(border, height - border, depth));
  frame.append(getVertexString(width, height, depth));
  frame.append(getVertexString(0.000000, height, depth));
  frame.append(endTriangle());
  frame.append(beginTriangle());
  frame.append(getVertexString(width - border, border, depth));
  frame.append(getVertexString(border, border, depth));
  frame.append(getVertexString(0.000000, 0.000000, depth));
  frame.append(endTriangle());
  frame.append(beginTriangle());
  frame.append(getVertexString(width - border, border, depth));
  frame.append(getVertexString(0.000000, 0.000000, depth));
  frame.append(getVertexString(width, 0.000000, depth));
  frame.append(endTriangle());
  frame.append(beginTriangle());
  frame.append(getVertexString(border + frameSlope, border + frameSlope, 0.100000));
  frame.append(getVertexString(border + frameSlope, height - border - frameSlope, 0.100000));
  frame.append(getVertexString(border, height - border, depth));
  frame.append(endTriangle());
  frame.append(beginTriangle());
  frame.append(getVertexString(border + frameSlope, border + frameSlope, 0.100000));
  frame.append(getVertexString(border, height - border, depth));
  frame.append(getVertexString(border, border, depth));
  frame.append(endTriangle());
  frame.append(beginTriangle());
  frame.append(getVertexString(width - border - frameSlope, height - border - frameSlope, 0.100000));
  frame.append(getVertexString(width - border - frameSlope, border + frameSlope, 0.100000));
  frame.append(getVertexString(width - border, border, depth));
  frame.append(endTriangle());
  frame.append(beginTriangle());
  frame.append(getVertexString(width - border - frameSlope, height - border - frameSlope, 0.100000));
  frame.append(getVertexString(width - border, border, depth));
  frame.append(getVertexString(width - border, height - border, depth));
  frame.append(endTriangle());
  frame.append(beginTriangle());
  frame.append(getVertexString(border + frameSlope, height - border - frameSlope, 0.100000));
  frame.append(getVertexString(width - border - frameSlope, height - border - frameSlope, 0.100000));
  frame.append(getVertexString(width - border, height - border, depth));
  frame.append(endTriangle());
  frame.append(beginTriangle());
  frame.append(getVertexString(border + frameSlope, height - border - frameSlope, 0.100000));
  frame.append(getVertexString(width - border, height - border, depth));
  frame.append(getVertexString(border, height - border, depth));
  frame.append(endTriangle());
  frame.append(beginTriangle());
  frame.append(getVertexString(width - border - frameSlope, border + frameSlope, 0.100000));
  frame.append(getVertexString(border + frameSlope, border + frameSlope, 0.100000));
  frame.append(getVertexString(border, border, depth));
  frame.append(endTriangle());
  frame.append(beginTriangle());
  frame.append(getVertexString(width - border - frameSlope, border + frameSlope, 0.100000));
  frame.append(getVertexString(border, border, depth));
  frame.append(getVertexString(width - border, border, depth));
  frame.append(endTriangle());
  return frame;
}

int MainWindow::getPixel(const QImage &image, const int &x, const int &y)
{
  return image.pixelColor(x, image.height() - 1 - y).red();
}

QString MainWindow::beginTriangle()
{
  return QString("\tfacet normal 0.0 0.0 0.0\n\t\touter loop\n");
}

QString MainWindow::getVertexString(double x, double y, double z, const bool &scale)
{
  double add = 0.0;
  if(scale) {
    x = x * widthFactor;
    y = y * widthFactor;
    //z = z * widthFactor;
    add = border;
  }
  return QString("\t\t\tvertex " + QString::number((double)x + add, 'g') + " " + QString::number((double)y + add, 'g') + " " + QString::number((double)z, 'g') + "\n");
}

QString MainWindow::endTriangle()
{
  return QString("\t\tendloop\n\tendfacet\n");
}

void MainWindow::inputSelect()
{
  QString selectedFile = QFileDialog::getOpenFileName(this, tr("Select input file"), QFileInfo(inputLineEdit->text()).absolutePath(), "*.png");
  if(selectedFile != QString()) {
    inputLineEdit->setText(selectedFile);
  }
}

void MainWindow::exportSelect()
{
  QString selectedFile = QFileDialog::getSaveFileName(this, tr("Enter export file"), QFileInfo(exportLineEdit->text()).absolutePath(), "*.stl");
  if(selectedFile != QString()) {
    if(selectedFile.right(4).toLower() != ".stl") {
      selectedFile.append(".stl");
    }
    exportLineEdit->setText(selectedFile);
  }
}
