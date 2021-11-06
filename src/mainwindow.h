/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            mainwindow.h
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

#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QMainWindow>
#include <QLineEdit>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QProgressBar>

class MainWindow : public QMainWindow
{
  Q_OBJECT
    
 public:
  MainWindow();
  ~MainWindow();
  
public slots:
  
protected:
  
signals:

private slots:
  void showAbout();
  void showPreferences();
  void inputSelect();
  void exportSelect();
  
private:
  void renderStl();
  void exportStl();
  int getPixel(const QImage &image, const int &x, const int &y);
  void createActions();
  void createMenus();
  //QByteArray stlString;
  QList<QVector3D> polygons;
  QLineEdit *minThicknessLineEdit;
  QLineEdit *totalThicknessLineEdit;
  QLineEdit *borderLineEdit;
  QLineEdit *widthLineEdit;
  QLineEdit *inputLineEdit;
  QProgressBar *renderProgress;
  QLineEdit *exportLineEdit;
  QAction *quitAct;
  QAction *preferencesAct;
  QAction *aboutAct;
  QMenu *fileMenu;
  QMenu *optionsMenu;
  QMenu *helpMenu;
  QMenuBar *menuBar;

  float depthFactor = -1.0;
  float widthFactor = -1.0;
  float border = -1.0;
  
  QByteArray beginTriangle();
  QVector3D getVertexString(float x, float y, float z, const bool &scale = false);
  QByteArray endTriangle();
  
  QList<QVector3D> addStabilizer(const float &x, const float &height);
  QList<QVector3D> addCube(const float &x, const float &y, const float &z, const float &size);
  QList<QVector3D> addFrame(const float &width, const float &height);
};

#endif // __MAINWINDOW_H__
