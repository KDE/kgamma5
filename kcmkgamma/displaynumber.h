/***************************************************************************
                          displaynumber.h  -  description
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

#ifndef DISPLAYNUMBER_H
#define DISPLAYNUMBER_H

#include <qlabel.h>

/**
  *@author Michael v.Ostheim
  */

class DisplayNumber : public QLabel  {
   Q_OBJECT
public: 
  explicit DisplayNumber(QWidget *parent=nullptr, int digits=0, int prec=0);
  ~DisplayNumber() override;
  void setFont( const QFont & f );
  void setNum(double num);
  void setWidth(int digits);
  void setPrecision(int prec) { precision = prec; };
  
private:
  int dg, precision;
};

#endif
