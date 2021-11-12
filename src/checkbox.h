/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            checkbox.h
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

#ifndef __CHECKBOX_H__
#define __CHECKBOX_H__

#include <QCheckBox>

class CheckBox : public QCheckBox
{
  Q_OBJECT
    
public:
  CheckBox(const QString &title, const QString &group,
           const QString &name,
           const bool &stdValue);
  ~CheckBox();
  void refreshState();

public slots:
  void resetToDefault();

private slots:
  void saveToConfig(int);

private:
  QString key;
  bool stdValue;
};

#endif // __CHECKBOX_H__
