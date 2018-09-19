/***************************************************************************
                          gammactrl.h  -  description
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
/**A horizontal slider and a text field for the gamma value.
  *@author Michael v.Ostheim
  */

#ifndef GAMMACTRL_H
#define GAMMACTRL_H

#include <qslider.h>

class QString;
class DisplayNumber;
class XVidExtWrap;

class GammaCtrl : public QWidget  {

  Q_OBJECT
  public:
    /** construktor */
    explicit GammaCtrl(QWidget *parent=nullptr, XVidExtWrap *xvid=nullptr, int channel=0,
      const QString& mingamma=QStringLiteral("0.40"), const QString& maxgamma=QStringLiteral("3.50"),
      const QString& setgamma=QStringLiteral("1.00"));
    /** destruktor */
    ~GammaCtrl() override;
    /** Return the current gamma value with precision prec */
    QString gamma(int);
    /** Set gamma, slider and textfield */
    void setGamma(const QString&);
    /** Set slider and textfield */
    void setControl(const QString&);
    /** Disable the slider */
    void disableSlider() { slider->setDisabled(true);}

  private:
    QString mgamma;
    QSlider *slider;
    DisplayNumber *textfield;
    bool suspended, changed;
    int gchannel, oldpos;
    double ming;
    XVidExtWrap *xv;

  public Q_SLOTS:
    /** Disable textfield */
    void suspend();

  protected Q_SLOTS:
    /** Set slider and textfield */
    void setCtrl(int);
    /** Set gamma and textfield */
    void setGamma(int);
    /** Change status of GammaCtrl when pressed */
    void pressed();

  Q_SIGNALS:
    /** Gamma change signal */
    void gammaChanged(int);
};

#endif
