/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            lineedit.cpp
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

#include "lineedit.h"

extern QSettings *settings;

LineEdit::LineEdit(QString group, QString name, QString stdValue, const bool &text) : isText(text)
{
  this->group = group;
  this->name = name;
  this->defaultValue = stdValue;
  
  if(group != "General")
    settings->beginGroup(group);
  if(!settings->contains(name)) {
    settings->setValue(name, stdValue);
  }
  setText(settings->value(name, stdValue).toString());
  if(group != "General")
    settings->endGroup();

  connect(this, SIGNAL(editingFinished()), this, SLOT(saveToConfig()));
  connect(this, SIGNAL(textChanged(QString)), this, SLOT(saveToConfig()));

}

LineEdit::~LineEdit()
{
}

void LineEdit::resetToDefault()
{
  setText(defaultValue);
}

void LineEdit::saveToConfig()
{
  if(!isText && text().contains(",")) {
    setText(text().replace(",", "."));
  }

  if(group != "General")
    settings->beginGroup(group);
  settings->setValue(name, this->text());
  if(group != "General")
    settings->endGroup();
  qDebug("Key '%s' saved to config with value '%s'\n", name.toStdString().c_str(), this->text().toStdString().c_str());
}
