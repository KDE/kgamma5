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

#include <qhbox.h>
#include <qslider.h>

class QString;
class DisplayNumber;
class XVidExtWrap;

class GammaCtrl : public QHBox  {

  Q_OBJECT
  public:
    /** construktor */
    GammaCtrl(QWidget *parent=0, XVidExtWrap *xvid=0, int channel=0, \
      const QString& mingamma="0.40", const QString& maxgamma="3.50", \
      const QString& setgamma="1.00", const char *name=0 );
    /** destruktor */
    ~GammaCtrl();
    /** Return the current gamma value with precision prec */
    QString gamma(int);
    /** Set gamma, slider and textfield */
    void setGamma(const QString&);
    /** Set slider and textfield */
    void setControl(const QString&);
    /** Disable the slider */
    void disableSlider() { slider->setDisabled(true);};

  private:
    QString mgamma;
    QSlider *slider;
    DisplayNumber *textfield;
    bool suspended, changed;
    int gchannel, oldpos;
    double ming;
    XVidExtWrap *xv;

  public slots:
    /** Disable textfield */
    void suspend();

  protected slots:
    /** Set slider and textfield */
    void setCtrl(int);
    /** Set gamma and textfield */
    void setGamma(int);
    /** Change status of GammaCtrl when pressed */
    void pressed();

  signals:
    /** Gamma change signal */
    void gammaChanged(int);
};

#endif
