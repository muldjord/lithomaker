/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            combobox.cpp
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

#include "combobox.h"

extern QSettings *settings;

ComboBox::ComboBox(const QString &group, const QString &name,
                   const QString &stdValue)
  : stdValue(stdValue)
{
  key = (group != "General"?group + "/":"") + name;
  
  if(!settings->contains(key)) {
    settings->setValue(key, stdValue);
  }
  configValue = settings->value(key, "1").toString();
  
  connect(this, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ComboBox::saveToConfig);
}

ComboBox::~ComboBox()
{
}

void ComboBox::addConfigItem(const QString &text, const QString &value)
{
  addItem(text, value);
}

void ComboBox::setFromConfig()
{
  for(int i = 0; i < count(); ++i) {
    if(itemData(i).toString() == configValue) {
      setCurrentIndex(i);
    }
  }
}

void ComboBox::resetToDefault()
{
  for(int i = 0; i < count(); ++i) {
    if(itemData(i).toString() == stdValue) {
      setCurrentIndex(i);
    }
  }
}

void ComboBox::saveToConfig(int)
{
  settings->setValue(key, currentData().toString());

  qDebug("Key '%s' saved to config with value '%s'\n", key.toStdString().c_str(), currentData().toString().toStdString().c_str());
}
