/***************************************************************************
                          displaynumber.cpp  -  description
                             -------------------
    begin                : Sun Feb 23 2003
    copyright            : (C) 2003 by Michael v.Ostheim
    email                : ostheimm@users.berlios.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qstring.h>
#include <qfontmetrics.h>

#include "displaynumber.h"

DisplayNumber::DisplayNumber(QWidget *parent, int digits, \
    int prec, const char *name) : QLabel(parent,name) {

  setPrecision(prec);
  setWidth(digits);  

  setFrameStyle(QFrame::Panel | QFrame::Sunken);
  setBackgroundMode(Qt::PaletteBase);
  setAlignment(Qt::AlignCenter);
  setFocusPolicy(NoFocus);
}

DisplayNumber::~DisplayNumber(){
}

void DisplayNumber::setFont( const QFont & f ) {
  QLabel::setFont(f);
  setWidth(dg);
}

void DisplayNumber::setWidth(int digits) {
  QFontMetrics fm(font());
  QString s("0123456789.+-");
  int width = 0, charWidth=0;

  for (int i = 0; i < 11; i++, width = fm.width(s[i]))
    charWidth = (width > charWidth) ? width : charWidth;

  dg = digits;
  setMinimumWidth( dg * charWidth + charWidth/2 );
}

void DisplayNumber::setNum(double num) {
  QString text;
  setText(text.setNum(num, 'f', precision));
}
    

#include "displaynumber.moc"
