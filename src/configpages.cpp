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

extern QSettings *settings;

GeneralPage::GeneralPage(QWidget *parent) : QWidget(parent)
{
  QPushButton *resetButton = new QPushButton(tr("Reset all to defaults"));

  CheckBox *alwaysOnTopCheckBox = new CheckBox(tr("Always keep window on top"), "General",
                                               "alwaysOnTop", false);
  connect(resetButton, SIGNAL(clicked()), alwaysOnTopCheckBox, SLOT(resetToDefault()));

  CheckBox *alwaysCenterCheckBox = new CheckBox(tr("Always center window on startup"), "General",
                                               "alwaysCenter", false);
  connect(resetButton, SIGNAL(clicked()), alwaysCenterCheckBox, SLOT(resetToDefault()));

  CheckBox *alwaysMaximizeCheckBox = new CheckBox(tr("Always maximize window on startup"), "General", "alwaysMaximize", true);
  connect(resetButton, SIGNAL(clicked()), alwaysMaximizeCheckBox, SLOT(resetToDefault()));

  CheckBox *removeFromListCheckBox = new CheckBox(tr("Remove each item from input list after processing"), "General", "removeFromList", false);
  connect(resetButton, SIGNAL(clicked()), removeFromListCheckBox, SLOT(resetToDefault()));

  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(resetButton);
  layout->addWidget(alwaysOnTopCheckBox);
  layout->addWidget(alwaysCenterCheckBox);
  layout->addWidget(alwaysMaximizeCheckBox);
  //layout->addWidget(removeFromListCheckBox);
  layout->addStretch();
  setLayout(layout);
}

AlgorithmPage::AlgorithmPage(QWidget *parent) : QWidget(parent)
{
  QPushButton *resetButton = new QPushButton(tr("Reset all to defaults"));

  /*
  const int maxDrops = settings->value("maxDrops", "2").toInt();
  */
  
  QLabel *minAmplitudeLabel = new QLabel(tr("Minimum accepted wave amplitude:"));
  LineEdit *minAmplitudeLineEdit = new LineEdit("Algorithm", "minAmplitude", "75");
  connect(resetButton, SIGNAL(clicked()), minAmplitudeLineEdit, SLOT(resetToDefault()));

  QLabel *minSpanLabel = new QLabel(tr("Minimum accepted wave span in data points:"));
  LineEdit *minSpanLineEdit = new LineEdit("Algorithm", "minSpan", "5");
  connect(resetButton, SIGNAL(clicked()), minSpanLineEdit, SLOT(resetToDefault()));

  QLabel *maxSpanLabel = new QLabel(tr("Maximum accepted wave span in data points:"));
  LineEdit *maxSpanLineEdit = new LineEdit("Algorithm", "maxSpan", "50");
  connect(resetButton, SIGNAL(clicked()), maxSpanLineEdit, SLOT(resetToDefault()));

  CheckBox *averageFilterCheckBox = new CheckBox(tr("Apply running average filter to input data"),
                                                 "Algorithm", "averageFilter", true);
  connect(resetButton, SIGNAL(clicked()), averageFilterCheckBox, SLOT(resetToDefault()));

  QLabel *averageFilterSpanLabel = new QLabel(tr("Running average filter span:"));
  LineEdit *averageFilterSpanLineEdit = new LineEdit("Algorithm", "averageFilterSpan", "4");
  connect(resetButton, SIGNAL(clicked()), averageFilterSpanLineEdit, SLOT(resetToDefault()));

  CheckBox *plateauCheckCheckBox = new CheckBox(tr("Enable plateau check"),
                                                 "Algorithm", "plateauCheck", true);
  connect(resetButton, SIGNAL(clicked()), plateauCheckCheckBox, SLOT(resetToDefault()));

  QLabel *plateauSpanLabel = new QLabel(tr("Plateau check span:"));
  LineEdit *plateauSpanLineEdit = new LineEdit("Algorithm", "plateauSpan", "5");
  connect(resetButton, SIGNAL(clicked()), plateauSpanLineEdit, SLOT(resetToDefault()));

  QLabel *risingThresLabel = new QLabel(tr("Rising threshold:"));
  LineEdit *risingThresLineEdit = new LineEdit("Algorithm", "risingThres", "25");
  connect(resetButton, SIGNAL(clicked()), risingThresLineEdit, SLOT(resetToDefault()));

  QLabel *waveInitSpanLabel = new QLabel(tr("Required number of consecutive rises for wave init:"));
  LineEdit *waveInitSpanLineEdit = new LineEdit("Algorithm", "waveInitSpan", "4");
  connect(resetButton, SIGNAL(clicked()), waveInitSpanLineEdit, SLOT(resetToDefault()));

  QLabel *fallingThresLabel = new QLabel(tr("Falling threshold:"));
  LineEdit *fallingThresLineEdit = new LineEdit("Algorithm", "fallingThres", "-25");
  connect(resetButton, SIGNAL(clicked()), fallingThresLineEdit, SLOT(resetToDefault()));

  QLabel *maxOpposingNoiseLabel = new QLabel(tr("Maximum number of opposing noise data points accepted:"));
  LineEdit *maxOpposingNoiseLineEdit = new LineEdit("Algorithm", "maxOpposingNoise", "2");
  connect(resetButton, SIGNAL(clicked()), maxOpposingNoiseLineEdit, SLOT(resetToDefault()));

  CheckBox *resetOpposingCountersCheckBox = new CheckBox(tr("Reset opposing noise counter on new rise/drop"),
                                                         "Algorithm", "resetOpposingCounters", true);
  connect(resetButton, SIGNAL(clicked()), resetOpposingCountersCheckBox, SLOT(resetToDefault()));

  QLabel *vertDevThresFactorLabel = new QLabel(tr("Vertical deviation threshold division factor:"));
  LineEdit *vertDevThresFactorLineEdit = new LineEdit("Algorithm", "vertDevThresFactor", "2.0");
  connect(resetButton, SIGNAL(clicked()), vertDevThresFactorLineEdit, SLOT(resetToDefault()));

  QLabel *maxBeginEndDevLabel = new QLabel(tr("Maximum vertical begin / end deviation:"));
  LineEdit *maxBeginEndDevLineEdit = new LineEdit("Algorithm", "maxBeginEndDev", "500");
  connect(resetButton, SIGNAL(clicked()), maxBeginEndDevLineEdit, SLOT(resetToDefault()));

  /*
  QLabel *concludeThresDivideLabel = new QLabel(tr("Wave conclusion threshold divider:"));
  LineEdit *concludeThresDivideLineEdit = new LineEdit("Algorithm", "concludeThresDivide", "8.0");
  connect(resetButton, SIGNAL(clicked()), concludeThresDivideLineEdit, SLOT(resetToDefault()));
  */

  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(resetButton);
  layout->addWidget(minAmplitudeLabel);
  layout->addWidget(minAmplitudeLineEdit);
  layout->addWidget(minSpanLabel);
  layout->addWidget(minSpanLineEdit);
  layout->addWidget(maxSpanLabel);
  layout->addWidget(maxSpanLineEdit);
  layout->addWidget(averageFilterCheckBox);
  layout->addWidget(averageFilterSpanLabel);
  layout->addWidget(averageFilterSpanLineEdit);
  /*
  layout->addWidget(plateauCheckCheckBox);
  layout->addWidget(plateauSpanLabel);
  layout->addWidget(plateauSpanLineEdit);
  */
  layout->addWidget(risingThresLabel);
  layout->addWidget(risingThresLineEdit);
  layout->addWidget(waveInitSpanLabel);
  layout->addWidget(waveInitSpanLineEdit);
  layout->addWidget(fallingThresLabel);
  layout->addWidget(fallingThresLineEdit);
  layout->addWidget(maxOpposingNoiseLabel);
  layout->addWidget(maxOpposingNoiseLineEdit);
  layout->addWidget(resetOpposingCountersCheckBox);
  layout->addWidget(vertDevThresFactorLabel);
  layout->addWidget(vertDevThresFactorLineEdit);
  layout->addWidget(maxBeginEndDevLabel);
  layout->addWidget(maxBeginEndDevLineEdit);
  //layout->addWidget(concludeThresDivideLabel);
  //layout->addWidget(concludeThresDivideLineEdit);
  layout->addStretch();
  setLayout(layout);
}

OutputPage::OutputPage(QWidget *parent) : QWidget(parent)
{
  QPushButton *resetButton = new QPushButton(tr("Reset all to defaults"));

  QLabel *delimiterLabel = new QLabel(tr("Delimiter:"));
  ComboBox *delimiterComboBox = new ComboBox("Output", "delimiter", "tab");
  delimiterComboBox->addConfigItem("Tab", "tab");
  delimiterComboBox->addConfigItem("Semicolon", "semicolon");
  delimiterComboBox->addConfigItem("Comma", "comma");
  delimiterComboBox->setFromConfig();
  connect(resetButton, SIGNAL(clicked()), delimiterComboBox, SLOT(resetToDefault()));

  /*
  QLabel *distanceUnitLabel = new QLabel(tr("Distance unit:"));
  LineEdit *distanceUnitLineEdit = new LineEdit("Output", "distanceUnit", "μm");
  connect(resetButton, SIGNAL(clicked()), distanceUnitLineEdit, SLOT(resetToDefault()));

  QLabel *timeUnitLabel = new QLabel(tr("Time unit:"));
  LineEdit *timeUnitLineEdit = new LineEdit("Output", "timeUnit", "s");
  connect(resetButton, SIGNAL(clicked()), timeUnitLineEdit, SLOT(resetToDefault()));
  */
  
  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(resetButton);
  layout->addWidget(delimiterLabel);
  layout->addWidget(delimiterComboBox);
  /*
  layout->addWidget(distanceUnitLabel);
  layout->addWidget(distanceUnitLineEdit);
  layout->addWidget(timeUnitLabel);
  layout->addWidget(timeUnitLineEdit);
  */
  layout->addStretch();
  setLayout(layout);
}

DiagramPage::DiagramPage(QWidget *parent) : QWidget(parent)
{
  QPushButton *resetButton = new QPushButton(tr("Reset all to defaults"));

  QLabel *waveMarkingColorLabel = new QLabel(tr("Wave marking color:"));
  LineEdit *waveMarkingColorLineEdit = new LineEdit("Diagram", "waveMarkingColor", "0,255,0", true);
  connect(resetButton, SIGNAL(clicked()), waveMarkingColorLineEdit, SLOT(resetToDefault()));

  QLabel *slope1ColorLabel = new QLabel(tr("Slope 1 color:"));
  LineEdit *slope1ColorLineEdit = new LineEdit("Diagram", "slope1Color", "255,0,0", true);
  connect(resetButton, SIGNAL(clicked()), slope1ColorLineEdit, SLOT(resetToDefault()));

  QLabel *slope2ColorLabel = new QLabel(tr("Slope 2 color:"));
  LineEdit *slope2ColorLineEdit = new LineEdit("Diagram", "slope2Color", "0,0,255", true);
  connect(resetButton, SIGNAL(clicked()), slope2ColorLineEdit, SLOT(resetToDefault()));

  /*
  QLabel *distanceUnitLabel = new QLabel(tr("Distance unit:"));
  LineEdit *distanceUnitLineEdit = new LineEdit("Output", "distanceUnit", "μm");
  connect(resetButton, SIGNAL(clicked()), distanceUnitLineEdit, SLOT(resetToDefault()));

  QLabel *timeUnitLabel = new QLabel(tr("Time unit:"));
  LineEdit *timeUnitLineEdit = new LineEdit("Output", "timeUnit", "s");
  connect(resetButton, SIGNAL(clicked()), timeUnitLineEdit, SLOT(resetToDefault()));
  */
  
  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(resetButton);
  layout->addWidget(waveMarkingColorLabel);
  layout->addWidget(waveMarkingColorLineEdit);
  layout->addWidget(slope1ColorLabel);
  layout->addWidget(slope1ColorLineEdit);
  layout->addWidget(slope2ColorLabel);
  layout->addWidget(slope2ColorLineEdit);
  /*
  layout->addWidget(distanceUnitLabel);
  layout->addWidget(distanceUnitLineEdit);
  layout->addWidget(timeUnitLabel);
  layout->addWidget(timeUnitLineEdit);
  */
  layout->addStretch();
  setLayout(layout);
}
