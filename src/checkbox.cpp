/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            checkbox.cpp
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

#include "checkbox.h"

extern QSettings *settings;

CheckBox::CheckBox(const QString &group, const QString &name,
                   const QString &title,
                   const bool &stdValue)
  : QCheckBox(title), stdValue(stdValue)
{
  key = (group != "General"?group + "/":"") + name;
  
  if(!settings->contains(key)) {
    settings->setValue(key, (stdValue?"true":"false"));
  }
  setChecked(settings->value(key, stdValue).toBool());

  setToolTip(tr("Default state: ") + (stdValue?tr("True"):tr("False")));

  connect(this, &QCheckBox::stateChanged, this, &CheckBox::saveToConfig);
}

CheckBox::~CheckBox()
{
}

void CheckBox::refreshState()
{
  toggle();
  toggle();
}

void CheckBox::resetToDefault()
{
  setChecked(stdValue);
}

void CheckBox::saveToConfig(int)
{
  if(isChecked()) {
    settings->setValue(key, "true");
  } else {
    settings->setValue(key, "false");
  }

  qDebug("Key '%s' saved to config with value '%s'\n", key.toStdString().c_str(), (isChecked()?"true":"false"));
}
