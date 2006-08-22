/***************************************************************************
                          kgamma.h  -  description
                             -------------------
    begin                : Sun Dec 16 13:52:24 CET 2001
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
#ifndef KGAMMA_H_
#define KGAMMA_H_

#include <kcmodule.h>

class GammaCtrl;
class QCheckBox;
class QComboBox;
class XVidExtWrap;
class KProcess;

class KGamma: public KCModule
{
   Q_OBJECT
   public:
      KGamma(QWidget *parent, const char *name, const QStringList&);
      virtual ~KGamma();

      void load();
      void load(bool useDefaults);
      void save();
      void defaults();
      int buttons();
      QString quickHelp() const;

    protected: // Protected methods
      /** The user interface */
      void setupUI();
      /** Decides if to load settings from user or system config */
      bool loadSettings();
      /** Load settings from kgammarc */
      bool loadUserSettings();
      /** Load settings from XF86Config */
      bool loadSystemSettings();
      /** Validate the loaded gamma values */
      bool validateGammaValues();

    private slots:
      /** Called if the user changesd something */
      void Changed() { emit changed(true); }
      /** Called if the user marked or unmarked the XF86Config checkbox */
      void changeConfig();
      /** Called if the user marked or unmarked the sync screen checkbox */
      void SyncScreens();
      /** Called if the user chooses a new screen */
      void changeScreen(int sn);

    private:
      bool saved, GammaCorrection;
      int ScreenCount, currentScreen;
      QStringList rgamma, ggamma, bgamma;
      QValueList<int> assign;
      QValueList<float> rbak, gbak, bbak;
      GammaCtrl *gctrl, *rgctrl, *ggctrl, *bgctrl;
      QCheckBox *xf86cfgbox, *syncbox;
      QComboBox *screenselect;
      KProcess *rootProcess;
      XVidExtWrap *xv;
};

#endif

