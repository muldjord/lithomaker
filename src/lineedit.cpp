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

LineEdit::LineEdit(const QString &group, const QString &name, const QString &stdValue,
                   const bool &isText)
  : stdValue(stdValue), isText(isText)
{
  key = (group != "General"?group + "/":"") + name;

  if(!settings->contains(key)) {
    settings->setValue(key, stdValue);
  }
  setText(settings->value(key, stdValue).toString());

  connect(this, &QLineEdit::textChanged, this, &LineEdit::saveToConfig);
}

LineEdit::~LineEdit()
{
}

void LineEdit::resetToDefault()
{
  setText(stdValue);
}

void LineEdit::saveToConfig(const QString &)
{
  if(!isText && text().contains(",")) {
    setText(text().replace(",", "."));
  }

  settings->setValue(key, text());
  qDebug("Key '%s' saved to config with value '%s'\n", key.toStdString().c_str(), text().toStdString().c_str());
}
