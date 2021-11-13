/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            configpages.cpp
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

#include "configpages.h"
#include "lineedit.h"
#include "combobox.h"
#include "checkbox.h"
#include "slider.h"

extern QSettings *settings;

MainPage::MainPage(QWidget *parent) : QWidget(parent)
{
  QPushButton *resetButton = new QPushButton(tr("Reset all to defaults"));

  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(resetButton);
  layout->addStretch();
  setLayout(layout);
}

RenderPage::RenderPage(QWidget *parent) : QWidget(parent)
{
  QPushButton *resetButton = new QPushButton(tr("Reset all to defaults"));

  CheckBox *enableStabilizersCheckBox = new CheckBox("render", "enableStabilizers", tr("Enable stabilizers"), true);
  connect(resetButton, &QPushButton::clicked, enableStabilizersCheckBox, &CheckBox::resetToDefault);

  CheckBox *permanentStabilizersCheckBox = new CheckBox("render", "permanentStabilizers", tr("Make stabilizers permanent"), false);
  connect(resetButton, &QPushButton::clicked, permanentStabilizersCheckBox, &CheckBox::resetToDefault);

  QLabel *stabilizerThresholdLabel = new QLabel(tr("Minimum height before adding stabilizers:"));
  LineEdit *stabilizerThresholdLineEdit = new LineEdit("render", "stabilizerThreshold", "60.0");
  connect(resetButton, &QPushButton::clicked, stabilizerThresholdLineEdit, &LineEdit::resetToDefault);

  QLabel *stabilizerHeightFactorLabel = new QLabel(tr("Stabilizer height factor:"));
  LineEdit *stabilizerHeightFactorLineEdit = new LineEdit("render", "stabilizerHeightFactor", "0.15");
  connect(resetButton, &QPushButton::clicked, stabilizerHeightFactorLineEdit, &LineEdit::resetToDefault);

  QLabel *frameSlopeFactorLabel = new QLabel(tr("Frame slope factor:"));
  LineEdit *frameSlopeFactorLineEdit = new LineEdit("render", "frameSlopeFactor", "1.5");
  connect(resetButton, &QPushButton::clicked, frameSlopeFactorLineEdit, &LineEdit::resetToDefault);

  CheckBox *enableHangersCheckBox = new CheckBox("render", "enableHangers", tr("Enable hangers"), true);
  connect(resetButton, &QPushButton::clicked, enableHangersCheckBox, &CheckBox::resetToDefault);

  QLabel *hangersLabel = new QLabel(tr("Number of hangers:"));
  Slider *hangersSlider = new Slider("render", "hangers", 1, 4, 2, 1);
  connect(resetButton, &QPushButton::clicked, hangersSlider, &Slider::resetToDefault);

  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(resetButton);
  layout->addWidget(enableStabilizersCheckBox);
  layout->addWidget(permanentStabilizersCheckBox);
  layout->addWidget(stabilizerThresholdLabel);
  layout->addWidget(stabilizerThresholdLineEdit);
  layout->addWidget(stabilizerHeightFactorLabel);
  layout->addWidget(stabilizerHeightFactorLineEdit);
  layout->addWidget(frameSlopeFactorLabel);
  layout->addWidget(frameSlopeFactorLineEdit);
  layout->addWidget(enableHangersCheckBox);
  layout->addWidget(hangersLabel);
  layout->addWidget(hangersSlider);
  layout->addStretch();
  setLayout(layout);
}

ExportPage::ExportPage(QWidget *parent) : QWidget(parent)
{
  QPushButton *resetButton = new QPushButton(tr("Reset all to defaults"));

  QLabel *stlFormatLabel = new QLabel(tr("STL format:"));
  ComboBox *stlFormatComboBox = new ComboBox("export", "stlFormat", "binary");
  stlFormatComboBox->addConfigItem("Ascii", "ascii");
  stlFormatComboBox->addConfigItem("Binary", "binary");
  stlFormatComboBox->setFromConfig();
  connect(resetButton, &QPushButton::clicked, stlFormatComboBox, &ComboBox::resetToDefault);

  CheckBox *alwaysOverwriteCheckBox = new CheckBox("export", "alwaysOverwrite", tr("Always overwrite existing file"), false);
  connect(resetButton, &QPushButton::clicked, alwaysOverwriteCheckBox, &CheckBox::resetToDefault);
  /*
  QLabel *delimiterLabel = new QLabel(tr("Delimiter:"));
  ComboBox *delimiterComboBox = new ComboBox("Export", "delimiter", "tab");
  delimiterComboBox->addConfigItem("Tab", "tab");
  delimiterComboBox->addConfigItem("Semicolon", "semicolon");
  delimiterComboBox->addConfigItem("Comma", "comma");
  delimiterComboBox->setFromConfig();
  connect(resetButton, SIGNAL(clicked()), delimiterComboBox, SLOT(resetToDefault()));

  QLabel *distanceUnitLabel = new QLabel(tr("Distance unit:"));
  LineEdit *distanceUnitLineEdit = new LineEdit("Export", "distanceUnit", "μm");
  connect(resetButton, SIGNAL(clicked()), distanceUnitLineEdit, SLOT(resetToDefault()));

  QLabel *timeUnitLabel = new QLabel(tr("Time unit:"));
  LineEdit *timeUnitLineEdit = new LineEdit("Export", "timeUnit", "s");
  connect(resetButton, SIGNAL(clicked()), timeUnitLineEdit, SLOT(resetToDefault()));
  */
  
  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(resetButton);
  layout->addWidget(stlFormatLabel);
  layout->addWidget(stlFormatComboBox);
  layout->addWidget(alwaysOverwriteCheckBox);
  /*
  layout->addWidget(delimiterLabel);
  layout->addWidget(delimiterComboBox);
  layout->addWidget(distanceUnitLabel);
  layout->addWidget(distanceUnitLineEdit);
  layout->addWidget(timeUnitLabel);
  layout->addWidget(timeUnitLineEdit);
  */
  layout->addStretch();
  setLayout(layout);
}
