/***************************************************************************
                          gammactrl.cpp  -  description
                             -------------------
    begin                : Sun Oct 7 2001
    copyright            : (C) 2001 by Michael v.Ostheim
    email                : MvOstheim@web.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qlabel.h>
#include <qlineedit.h>
#include <qstring.h>

#include <kdialog.h>

#include "gammactrl.h"
#include "xvidextwrap.h"
#include "displaynumber.h"
#include "gammactrl.moc"

GammaCtrl::GammaCtrl(QWidget *parent, XVidExtWrap *xvid, int channel, \
  const QString& mingamma, const QString& maxgamma, const QString& setgamma, \
  const char *name) : QHBox(parent, name)
{
  int maxslider = (int)( ( maxgamma.toDouble() - mingamma.toDouble() \
                  + 0.0005 ) * 20 );
  int setslider = (int)( ( setgamma.toDouble() - mingamma.toDouble() \
                  + 0.0005 ) * 20 );
  setslider = (setslider > maxslider) ? maxslider : setslider;
  setslider = (setslider < 0) ? 0 : setslider;

  suspended = false;
  changed=false;
  ming = mingamma.toFloat();
  mgamma = mingamma;
  oldpos = setslider;
  gchannel = channel;
  xv = xvid;

  setSpacing(KDialog::spacingHint());

  slider = new QSlider(Horizontal, this);
  slider->setFixedHeight(24);
  slider->setTickmarks(QSlider::Below);
  slider->setRange(0, maxslider);
  slider->setTickInterval(2);
  slider->setValue(setslider);
  connect(slider, SIGNAL(valueChanged(int)), SLOT(setGamma(int)));
  connect(slider, SIGNAL(sliderPressed()), SLOT(pressed()));

  textfield = new DisplayNumber(this, 4, 2);
  textfield->setText(setgamma);

}

GammaCtrl::~GammaCtrl()
{
}

/** set gamma, slider and textfield */
void GammaCtrl::setGamma(const QString& gamma){
  int sliderpos;

  sliderpos = (int)( ( gamma.toDouble() - mgamma.toDouble() + 0.0005 ) * 20 );
  changed=true;
  slider->setValue(sliderpos);

  setGamma(sliderpos);
  if (suspended) {
    suspended=false;
    textfield->setDisabled(false);
  }
}

/** set slider and textfield */
void GammaCtrl::setControl(const QString& gamma){
  int sliderpos;

  sliderpos = (int)( ( gamma.toDouble() - mgamma.toDouble() + 0.0005 ) * 20 );
  setCtrl(sliderpos);
}

/** Return the current gamma value with precision prec */
QString GammaCtrl::gamma(int prec){
  QString gammatext;
  gammatext.setNum(xv->getGamma(gchannel) + 0.0005, 'f', prec);

  return(gammatext);
}

/** Slot: set gamma and textfield */
void GammaCtrl::setGamma(int sliderpos){
  if (sliderpos != oldpos || changed) {
    xv->setGamma(gchannel, ming+(float)(slider->value())*0.05);
    textfield->setNum(xv->getGamma(gchannel));
    oldpos = sliderpos;
    changed=false;
    emit gammaChanged(sliderpos);
  }
}

/** Slot: set slider and textfield */
void GammaCtrl::setCtrl(int sliderpos){
  if (suspended) {
    suspended=false;
    textfield->setDisabled(false);
  }
  oldpos = sliderpos;
  slider->setValue(sliderpos);
  textfield->setNum(xv->getGamma(gchannel));
}

/** Slot: disable textfield */
void GammaCtrl::suspend(){
  if (!suspended) {
    suspended = true;
    textfield->setDisabled(true);
  }
}

/** Slot: Change status of GammaCtrl when pressed */
void GammaCtrl::pressed(){
  if (suspended) {
    suspended=false;
    textfield->setDisabled(false);
    changed=true;
    setGamma(slider->value());
  }
}

