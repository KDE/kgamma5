/***************************************************************************
                          kgamma.cpp  -  description
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
#define QT_CLEAN_NAMESPACE

#include <unistd.h>

#include <qlabel.h>
#include <qpixmap.h>
#include <qstring.h>
#include <qlayout.h>
#include <qwidget.h>
#include <qtabwidget.h>
#include <qvbox.h>
#include <qstringlist.h>
#include <qdir.h>
#include <qcheckbox.h>
#include <qcombobox.h>

#include <kstddirs.h>
#include <kconfig.h>
#include <klocale.h>
#include <kglobal.h>
#include <kprocess.h>

#include "config.h"
#include "xf86configpath.h"
#include "gammactrl.h"
#include "xvidextwrap.h"
#include "kgamma.h"


KGamma::KGamma(QWidget *parent, const char *name):KCModule(parent,name)
{
  bool ok;
  xv = new XVidExtWrap(&ok, NULL);
  ScreenCount = xv->_ScreenCount();
  currentScreen = xv->getScreen();
  xv->setGammaLimits(0.4, 3.5);

  for (int i = 0; i < ScreenCount; i++ ) {
    assign << 0;
    rgamma << "";
    ggamma << "";
    bgamma << "";

    // Store the current gamma values
    xv->setScreen(i);
    rbak << xv->getGamma(XVidExtWrap::Red);
    gbak << xv->getGamma(XVidExtWrap::Green);
    bbak << xv->getGamma(XVidExtWrap::Blue);
  }
  xv->setScreen(currentScreen);

  rootProcess = new KProcess;
  setupUI();
  saved = false;

  if (!loadSettings()) {  //try to load gamma values from config file
    // if failed, take current gamma values
    for (int i = 0; i < ScreenCount; i++ ) {
      rgamma[i].setNum(rbak[i], 'f', 2);
      ggamma[i].setNum(gbak[i], 'f', 2);
      bgamma[i].setNum(bbak[i], 'f', 2);
    }
  }
  load();
}

KGamma::~KGamma() {
  // Restore the old gamma settings, if the user has not saved
  // and there is no valid kgammarc.
  // Existing user settings overwrite system settings
  if ( loadUserSettings() ) load(); 
  else if ( !saved ) 
    for (int i = 0; i < ScreenCount; i++ ) {
      xv->setScreen(i);
      xv->setGamma( XVidExtWrap::Red, rbak[i] );
      xv->setGamma( XVidExtWrap::Green, gbak[i] );
      xv->setGamma( XVidExtWrap::Blue, bbak[i] );
    }
}

/** User interface */
void KGamma::setupUI() {
  QBoxLayout *topLayout = new QVBoxLayout(this, 8, 8);

  QTabWidget *folder = new QTabWidget(this);
  folder->setFocusPolicy(NoFocus);
  QPixmap background;
  background.load(locate("data", "kgamma/pics/background.png"));

  QLabel *pic1 = new QLabel(this);
  pic1->setMinimumSize(570, 220);
  pic1->setBackgroundPixmap(background);
  pic1->setPixmap(QPixmap(locate("data", "kgamma/pics/greyscale.png")));
  pic1->setAlignment(AlignCenter);
  folder->addTab(pic1, i18n("Grey Scale"));

  QLabel *pic2 = new QLabel(this);
  pic2->setBackgroundPixmap(background);
  pic2->setPixmap(QPixmap(locate("data", "kgamma/pics/rgbscale.png")));
  pic2->setAlignment(AlignCenter);
  folder->addTab(pic2,i18n("RGB Scale"));

  QLabel *pic3 = new QLabel(this);
  pic3->setBackgroundPixmap(background);
  pic3->setPixmap(QPixmap(locate("data", "kgamma/pics/cmyscale.png")));
  pic3->setAlignment(AlignCenter);
  folder->addTab(pic3, i18n("CMY Scale"));

  QLabel *pic4 = new QLabel(this);
  pic4->setBackgroundPixmap(background);
  pic4->setPixmap(QPixmap(locate("data", "kgamma/pics/darkgrey.png")));
  pic4->setAlignment(AlignCenter);
  folder->addTab(pic4, i18n("Dark Grey"));

  QLabel *pic5 = new QLabel(this);
  pic5->setBackgroundPixmap(background);
  pic5->setPixmap(QPixmap(locate("data", "kgamma/pics/midgrey.png")));
  pic5->setAlignment(AlignCenter);
  folder->addTab(pic5, i18n("Mid Grey"));

  QLabel *pic6 = new QLabel(this);
  pic6->setBackgroundPixmap(background);
  pic6->setPixmap(QPixmap(locate("data", "kgamma/pics/lightgrey.png")));
  pic6->setAlignment(AlignCenter);
  folder->addTab(pic6, i18n("Light Grey"));

  topLayout->addWidget(folder);

  gctrl = new GammaCtrl(this, "gamma", i18n("Gamma:"), xv);
  gctrl->setMargin(8);
  gctrl->setFrameStyle(QFrame::Box|QFrame::Sunken);
  connect(gctrl, SIGNAL(gammaChanged(int)), SLOT(Changed()));
  connect(gctrl, SIGNAL(gammaChanged(int)), SLOT(SyncScreens()));

  QVBox *rgbctrl = new QVBox(this);
  rgbctrl->setMargin(8);
  rgbctrl->setSpacing(2);
  rgbctrl->setFrameStyle(QFrame::Box|QFrame::Sunken);

  rgctrl = new GammaCtrl(rgbctrl,"red",i18n("Red:"),xv,XVidExtWrap::Red);
  connect(rgctrl, SIGNAL(gammaChanged(int)), SLOT(Changed()));
  connect(rgctrl, SIGNAL(gammaChanged(int)), SLOT(SyncScreens()));
  connect(gctrl, SIGNAL(gammaChanged(int)), rgctrl, SLOT(setCtrl(int)));
  connect(rgctrl, SIGNAL(gammaChanged(int)), gctrl, SLOT(suspend()));

  ggctrl = new GammaCtrl(rgbctrl,"green",i18n("Green:"),xv,XVidExtWrap::Green);
  connect(ggctrl, SIGNAL(gammaChanged(int)), SLOT(Changed()));
  connect(ggctrl, SIGNAL(gammaChanged(int)), SLOT(SyncScreens()));
  connect(gctrl, SIGNAL(gammaChanged(int)), ggctrl, SLOT(setCtrl(int)));
  connect(ggctrl, SIGNAL(gammaChanged(int)), gctrl, SLOT(suspend()));

  bgctrl = new GammaCtrl(rgbctrl,"blue",i18n("Blue:"),xv,XVidExtWrap::Blue);
  connect(bgctrl, SIGNAL(gammaChanged(int)), SLOT(Changed()));
  connect(bgctrl, SIGNAL(gammaChanged(int)), SLOT(SyncScreens()));
  connect(gctrl, SIGNAL(gammaChanged(int)), bgctrl, SLOT(setCtrl(int)));
  connect(bgctrl, SIGNAL(gammaChanged(int)), gctrl, SLOT(suspend()));

  QHBox *options = new QHBox(this);

  xf86cfgbox = new QCheckBox( i18n("Save settings to XF86Config"), options );
  connect(xf86cfgbox, SIGNAL(clicked()), SLOT(changeConfig()));

  syncbox = new QCheckBox( i18n("Sync Screens"), options );
  connect(syncbox, SIGNAL(clicked()), SLOT(SyncScreens()));
  connect(syncbox, SIGNAL(clicked()), SLOT(Changed()));

  screenselect = new QComboBox( options );
  for ( int i = 0; i < ScreenCount; i++ )
    screenselect->insertItem( QString( i18n("Screen") + " %1").arg(i) );
  screenselect->setCurrentItem(currentScreen);
  connect(screenselect, SIGNAL(activated(int)), SLOT(changeScreen(int)));

  options->setSpacing( 10 );
  options->setStretchFactor( xf86cfgbox, 10 );
  options->setStretchFactor( syncbox, 1 );
  options->setStretchFactor( screenselect, 1 );
  
  topLayout->addWidget(gctrl);
  topLayout->addWidget(rgbctrl);
  topLayout->addWidget(options);
}

/** Restore latest saved gamma values */
void KGamma::load() {

  KConfig *config = new KConfig("kgammarc");
  config->setGroup("ConfigFile");

  // save checkbox status
  if ( xf86cfgbox->isChecked() ) config->writeEntry("use", "XF86Config");
  else config->writeEntry("use", "kgammarc");

  // load syncbox status
  config->setGroup("SyncBox");
  if ( config->readEntry("sync") == "yes" ) syncbox->setChecked(true);
  else syncbox->setChecked(false);

  config->sync();
  delete config;

  for (int i = 0; i < ScreenCount; i++) {
    xv->setScreen(i);
    if (rgamma[i] == ggamma[i] && rgamma[i] == bgamma[i])
      if (i == currentScreen) gctrl->setGamma(rgamma[i]);
      else xv->setGamma(XVidExtWrap::Value, rgamma[i].toFloat());
    else {
      if (i == currentScreen) {
        rgctrl->setGamma(rgamma[i]);
        ggctrl->setGamma(ggamma[i]);
        bgctrl->setGamma(bgamma[i]);
        gctrl->suspend();
      }
      else {
        xv->setGamma(XVidExtWrap::Red, rgamma[i].toFloat());
        xv->setGamma(XVidExtWrap::Green, ggamma[i].toFloat());
        xv->setGamma(XVidExtWrap::Blue, bgamma[i].toFloat());
      }
    }
  }
  xv->setScreen(currentScreen);

  emit changed(false);
}

void KGamma::save() {
  for (int i = 0; i < ScreenCount; i++) {
    xv->setScreen(i);
    rgamma[i] = rgctrl->gamma(2);
    ggamma[i] = ggctrl->gamma(2);
    bgamma[i] = bgctrl->gamma(2);
  }
  xv->setScreen(currentScreen);

  KConfig *config = new KConfig("kgammarc");
  config->setGroup("SyncBox");
  if ( syncbox->isChecked() ) config->writeEntry("sync", "yes");
  else config->writeEntry("sync", "no");
  
  if ( !xf86cfgbox->isChecked() ) { //write gamma settings to the users config
    for (int i = 0; i < ScreenCount; i++) {
      config->setGroup( QString("Screen %1").arg(i) );
      config->writeEntry("rgamma", rgamma[i]);
      config->writeEntry("ggamma", ggamma[i]);
      config->writeEntry("bgamma", bgamma[i]);
    }
    config->setGroup("ConfigFile");
    config->writeEntry("use", "kgammarc");
  }
  else {  // write gamma settings to section "Monitor" of XF86Config
    config->setGroup("ConfigFile");
    config->writeEntry("use", "XF86Config");

    if ( !rootProcess->isRunning() ) {
      QString Arguments = "xf86gammacfg ";
      for (int i = 0; i < ScreenCount; i++)
        Arguments += rgamma[assign[i]] + " " + ggamma[assign[i]] + " " + \
        bgamma[assign[i]] + " ";
      rootProcess->clearArguments();
      *rootProcess << "kdesu" << Arguments;
      rootProcess->start();
    }     
  }
  config->sync();
  delete config;
  saved = true;
  emit changed(false);
}

void KGamma::defaults() {
  for (int i = 0; i < ScreenCount; i++) {
    xv->setScreen(i);
    gctrl->setGamma("1.00");
  }
  xv->setScreen(currentScreen);
}

bool KGamma::loadSettings() {
  KConfig *config = new KConfig("kgammarc");
  config->setGroup("ConfigFile");
  QString ConfigFile( config->readEntry("use") );
  config->setGroup("SyncBox");
  if ( config->readEntry("sync") == "yes" ) syncbox->setChecked(true);   
  delete config;

  if ( ConfigFile == "XF86Config" ) {  // parse XF86Config
    xf86cfgbox->setChecked(true);
    return( loadSystemSettings() );
  }
  else { //get gamma settings from user config
    return( loadUserSettings() );
  }
}

bool KGamma::loadUserSettings() {
  KConfig *config = new KConfig("kgammarc");

  for (int i = 0; i < ScreenCount; i++) {
    config->setGroup(QString( "Screen %1" ).arg(i) );
    rgamma[i] = config->readEntry("rgamma");
    ggamma[i] = config->readEntry("ggamma");
    bgamma[i] = config->readEntry("bgamma");
  }
  delete config;

  return( validateGammaValues() );
}

bool KGamma::loadSystemSettings() {
  QStringList Monitor, Screen, ScreenLayout, ScreenMonitor, Gamma;
  QValueList<int> ScreenNr;
  QString Section;
  XF86ConfigPath Path;
    
  QFile f( Path.get() );
  if ( f.open(IO_ReadOnly) ) {
    QTextStream t( &f );
    QString s;
    int sn = 0;
    bool gm = false;
    
    // Analyse Screen<->Monitor assignments of multi-head configurations      
    while ( !t.eof() ) {
      s = (t.readLine()).simplifyWhiteSpace();
      QStringList words = QStringList::split(' ', s);

      if ( words[0] == "Section" ) {
        if ( (Section = words[1]) == "\"Monitor\"" ) gm = false;
      }
      else if ( words[0] == "EndSection" ) {
        if ( Section == "\"Monitor\"" && !gm ) {
          Gamma << "";
          gm = false;
        }
        Section = "";
      }
      else if ( words[0] == "Identifier" ) {
        if ( Section == "\"Monitor\"" ) Monitor << words[1];
        else if ( Section == "\"Screen\"" ) Screen << words[1];
      }
      else if ( words[0] == "Screen" ) {
        if ( Section == "\"ServerLayout\"" ) {
          bool ok;
          int i = words[1].toInt(&ok);
          if ( ok ) {
            ScreenNr << i;
            ScreenLayout << words[2];
          }
          else {
            ScreenNr << sn++;
            ScreenLayout << words[1];
          }
        }
      }
      else if ( words[0] == "Monitor" ) {
        if ( Section == "\"Screen\"" )
          ScreenMonitor << words[1];
      }
      else if ( words[0] == "Gamma" ) {
        if ( Section == "\"Monitor\"" ) {
          Gamma << s;
          gm = true;
        }
      }
    } // End while
    f.close();

    for ( int i = 0; i < ScreenCount; i++ ) {
      for ( int j = 0; j < ScreenCount; j++ ) {
        if ( ScreenLayout[i] == Screen[j] ) {
          for ( int k = 0; k < ScreenCount; k++ ) {
            if ( Monitor[k] == ScreenMonitor[j] )
              assign[ScreenNr[i]] = k;  
          }
        }
      }
    }
 
    // Extract gamma values
    for ( int i = 0; i < ScreenCount; i++) {
      rgamma[i] = ggamma[i] = bgamma[i] = "";
      
      QStringList words = QStringList::split(' ', Gamma[assign[i]]);
      QStringList::ConstIterator it = words.begin();
      if ( words.size() < 4 )
        rgamma[i] = ggamma[i] = bgamma[i] = *(++it);   // single gamma value
      else  {
        rgamma[i] = *(++it);  // eventually rgb gamma values
        ggamma[i] = *(++it);
        bgamma[i] = *(++it);
      }
    }

  }
  return( validateGammaValues() );
}

bool KGamma::validateGammaValues() {
  bool rOk, gOk, bOk, result;

  result = true;
  for (int i = 0; i < ScreenCount; i++ ) {
    rgamma[i].toFloat( &rOk );
    ggamma[i].toFloat( &gOk );
    bgamma[i].toFloat( &bOk );

    if ( !(rOk && gOk && bOk) ) {
      if ( rOk ) ggamma[i] = bgamma[i] = rgamma[i];
      else result = false;
    }
  }
  return(result);
}

void KGamma::changeConfig() {
  bool Ok = false;
  
  if ( xf86cfgbox->isChecked() ) Ok = loadSystemSettings();
  else Ok = loadUserSettings();

  if ( !Ok ) {
    for (int i = 0; i < ScreenCount; i++ ) {
      xv->setScreen(i);
      rgamma[i].setNum(xv->getGamma(XVidExtWrap::Red), 'f', 2);
      ggamma[i].setNum(xv->getGamma(XVidExtWrap::Green), 'f', 2);
      bgamma[i].setNum(xv->getGamma(XVidExtWrap::Blue), 'f', 2);
    }
    xv->setScreen(currentScreen);
  }
  load();
}

void KGamma::SyncScreens() {
  if ( syncbox->isChecked() ) {
    float rg = xv->getGamma(XVidExtWrap::Red);
    float gg = xv->getGamma(XVidExtWrap::Green);
    float bg = xv->getGamma(XVidExtWrap::Blue);

    for (int i = 0; i < ScreenCount; i++ ) {
      if ( i != currentScreen ) {
        xv->setScreen(i);
        xv->setGamma(XVidExtWrap::Red, rg);
        xv->setGamma(XVidExtWrap::Green, gg);
        xv->setGamma(XVidExtWrap::Blue, bg);
      }
    }
    xv->setScreen(currentScreen);
  }
}

void KGamma::changeScreen(int sn) {
  QString red, green, blue;

  xv->setScreen(sn);
  currentScreen = sn;
  
  red.setNum(xv->getGamma(XVidExtWrap::Red), 'f', 2);
  green.setNum(xv->getGamma(XVidExtWrap::Green), 'f', 2);
  blue.setNum(xv->getGamma(XVidExtWrap::Blue), 'f', 2);

  gctrl->setControl(red);
  rgctrl->setControl(red);
  ggctrl->setControl(green);
  bgctrl->setControl(blue);
  if (red != green || red != blue) gctrl->suspend();
}

int KGamma::buttons () {
  return KCModule::Default|KCModule::Apply|KCModule::Help;
}

QString KGamma::quickHelp() const
{
  return i18n("<h1>Monitor Gamma</h1> This is a tool for monitor calibration."
    " Use the four sliders to define the gamma correction either as a"
    " single value, or separately for the red, green and blue components."
    " You may need to correct the brightness and contrast settings of your"
    " monitor for good results. The test images helps you to find proper"
    " settings.<br> If the checkbox is checked, the settings will be stored"
    " to XF86Config. This are system wide settings, root access is required"
    " for that. If not, the settings will be stored in the users config file,"
    " and will be restored at next KDE startup of this user.");
}


// ------------------------------------------------------------------------

extern "C"
{

  KCModule *create_kgamma(QWidget *parent, const char *name)
  {
    KGlobal::locale()->insertCatalogue("kgamma");
    return new KGamma(parent, name);
  }

  // Restore the user gamma settings
  void init_kgamma()
  {
    bool ok;
    XVidExtWrap xv(&ok);
    
    if (ok) {
      xv.setGammaLimits(0.4, 3.5);
      float rgamma, ggamma, bgamma;
      KConfig *config = new KConfig("kgammarc");

      for (int i = 0; i < xv._ScreenCount(); i++) {
        xv.setScreen(i);
        config->setGroup( QString("Screen %1").arg(i) );

        if ((rgamma = config->readEntry("rgamma").toFloat()))
          xv.setGamma(XVidExtWrap::Red, rgamma);
        if ((ggamma = config->readEntry("ggamma").toFloat()))
          xv.setGamma(XVidExtWrap::Green, ggamma);
        if ((bgamma = config->readEntry("bgamma").toFloat()))
          xv.setGamma(XVidExtWrap::Blue, bgamma);
      }
      delete config;
    }
  }
}
