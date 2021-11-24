/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            slider.cpp
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

#define DEBUG

#include <stdio.h>
#include <QSettings>
#include <QHBoxLayout>

#include "slider.h"

extern QSettings *settings;

Slider::Slider(const QString &group, const QString &name,
               const int &minValue, const int &maxValue, const int &stdValue, const int &exponent)
  : stdValue(stdValue), exponent(exponent)
{
  lineEdit = new QLineEdit();
  lineEdit->setMaximumWidth(50);
  slider = new QSlider(Qt::Horizontal);
  slider->setMinimum(minValue);
  slider->setMaximum(maxValue);
  slider->setTickInterval(1);
  slider->setSingleStep(1);
  slider->setPageStep(1);
  slider->setToolTip(tr("Default value: ") + QString::number((float)stdValue / exponent));

  key = (group != "General"?group + "/":"") + name;
  
  if(!settings->contains(key)) {
    settings->setValue(key, QString::number((float)stdValue / exponent));
  }
  slider->setValue(settings->value(key).toFloat() * exponent);
  lineEdit->setText(QString::number((float)slider->value() / exponent));
  lineEdit->setToolTip(tr("Default value: ") + QString::number((float)stdValue / exponent));
  
  QHBoxLayout *layout = new QHBoxLayout();
  layout->addWidget(lineEdit);
  layout->addWidget(slider);

  setLayout(layout);

  connect(slider, &QSlider::valueChanged, this, &Slider::saveToConfig);
  connect(lineEdit, &QLineEdit::editingFinished, this, &Slider::setSlider);
}

Slider::~Slider()
{
}

void Slider::resetToDefault()
{
  slider->setValue((float)stdValue * exponent);
  lineEdit->setText(QString::number((float)stdValue / exponent));
}

void Slider::saveToConfig()
{
  lineEdit->setText(QString::number((float)slider->value() / exponent));

  settings->setValue(key, lineEdit->text());
  qDebug("Key '%s' saved to config with value '%s'\n", key.toStdString().c_str(), lineEdit->text().toStdString().c_str());
}

void Slider::setSlider()
{
  slider->setValue(lineEdit->text().toFloat() * exponent);
}
