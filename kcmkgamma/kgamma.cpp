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

#include <unistd.h>

#include <qlabel.h>
#include <qpixmap.h>
#include <qstring.h>
#include <qlayout.h>
#include <qstringlist.h>
#include <qdir.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <QStackedWidget>
//Added by qt3to4:
#include <QTextStream>
#include <QGridLayout>
#include <QFrame>
#include <QHBoxLayout>
#include <QBoxLayout>
#include <Q3ValueList>
#include <QVBoxLayout>

#include <kstandarddirs.h>
#include <kconfig.h>
#include <klocale.h>
#include <kglobal.h>
#include <kprocess.h>
#include <kdialog.h>
#include <kgenericfactory.h>
#include <khbox.h>
#include "config.h"
#include "xf86configpath.h"
#include "gammactrl.h"
#include "xvidextwrap.h"
#include "kgamma.h"
#include "kgamma.moc"

extern "C"
{
       KDE_EXPORT KCModule *create_kcm_kgamma( QWidget *parent, const char * )
       {
           KInstance *inst = new KInstance( "kgamma" );
           return new KGamma( inst, parent, QStringList() );
       }

	bool KDE_EXPORT test_kgamma()
	{
		bool retval;
		(void) new XVidExtWrap(&retval, NULL);
		return retval;
	}
}

KGamma::KGamma(KInstance* instance, QWidget *parent, const QStringList& args)
    :KCModule(instance, parent, args)
{
  bool ok;
  GammaCorrection = true;
  xv = new XVidExtWrap(&ok, NULL);
  if (ok) { /* KDE 4: Uneccessary test, when all KCM wrappers do conditional loading */
    xv->getGamma(XVidExtWrap::Red, &ok);
    if (ok) {
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
    else {  //something is wrong, show only error message
      GammaCorrection = false;
      setupUI();
    }
  }
}

KGamma::~KGamma() {
  // Restore the old gamma settings, if the user has not saved
  // and there is no valid kgammarc.
  // Existing user settings overwrite system settings
  if (GammaCorrection) {
    if ( loadUserSettings() ) load();
    else if ( !saved )
      for (int i = 0; i < ScreenCount; i++ ) {
        xv->setScreen(i);
        xv->setGamma( XVidExtWrap::Red, rbak[i] );
        xv->setGamma( XVidExtWrap::Green, gbak[i] );
        xv->setGamma( XVidExtWrap::Blue, bbak[i] );
      }
    delete rootProcess;
  }
}

/** User interface */
void KGamma::setupUI() {
  QBoxLayout *topLayout = new QVBoxLayout(this);
  topLayout->setSpacing(KDialog::spacingHint());
  topLayout->setMargin(0);

  if (GammaCorrection) {
    QHBoxLayout *hbox = new QHBoxLayout();
    topLayout->addItem( hbox );
    QLabel *label = new QLabel( i18n( "&Select test picture:" ) , this);
    QComboBox *combo = new QComboBox( this );
    label->setBuddy( combo );

    QStringList list;
    list << i18n( "Gray Scale" )
         << i18n( "RGB Scale" )
         << i18n( "CMY Scale" )
         << i18n( "Dark Gray" )
         << i18n( "Mid Gray" )
         << i18n( "Light Gray" );
    combo->addItems( list );

    hbox->addWidget( label );
    hbox->addWidget( combo );
    hbox->addStretch();

    QStackedWidget  *stack = new QStackedWidget ( this );
    stack->setFrameStyle( QFrame::Box | QFrame::Raised );

    connect( combo, SIGNAL( activated( int ) ),
             stack, SLOT( setCurrentIndex( int ) ) );

    QPixmap background;
    background.load(KStandardDirs::locate("data", "kgamma/pics/background.png"));

    QLabel *pic1 = new QLabel(stack);
    pic1->setMinimumSize(530, 171);
    pic1->setBackgroundPixmap(background);
    pic1->setPixmap(QPixmap(KStandardDirs::locate("data", "kgamma/pics/greyscale.png")));
    pic1->setAlignment(Qt::AlignCenter);
    stack->insertWidget( 0,pic1 );

    QLabel *pic2 = new QLabel(stack);
    pic2->setBackgroundPixmap(background);
    pic2->setPixmap(QPixmap(KStandardDirs::locate("data", "kgamma/pics/rgbscale.png")));
	pic2->setAlignment(Qt::AlignCenter);
    stack->insertWidget( 1,pic2 );

    QLabel *pic3 = new QLabel(stack);
    pic3->setBackgroundPixmap(background);
    pic3->setPixmap(QPixmap(KStandardDirs::locate("data", "kgamma/pics/cmyscale.png")));
    pic3->setAlignment(Qt::AlignCenter);
    stack->insertWidget( 2,pic3 );

    QLabel *pic4 = new QLabel(stack);
    pic4->setBackgroundPixmap(background);
    pic4->setPixmap(QPixmap(KStandardDirs::locate("data", "kgamma/pics/darkgrey.png")));
    pic4->setAlignment(Qt::AlignCenter);
    stack->insertWidget( 3,pic4 );

    QLabel *pic5 = new QLabel(stack);
    pic5->setBackgroundPixmap(background);
    pic5->setPixmap(QPixmap(KStandardDirs::locate("data", "kgamma/pics/midgrey.png")));
    pic5->setAlignment(Qt::AlignCenter);
    stack->insertWidget( 4,pic5 );

    QLabel *pic6 = new QLabel(stack);
    pic6->setBackgroundPixmap(background);
    pic6->setPixmap(QPixmap(KStandardDirs::locate("data", "kgamma/pics/lightgrey.png")));
    pic6->setAlignment(Qt::AlignCenter);
    stack->insertWidget( 5,pic6 );

    topLayout->addWidget(stack, 10);

    //Sliders for gamma correction
    QFrame *frame1 = new QFrame(this);
    frame1->setFrameStyle( QFrame::GroupBoxPanel | QFrame::Plain );

    QFrame *frame2 = new QFrame(this);
    frame2->setFrameStyle( QFrame::GroupBoxPanel | QFrame::Plain );

    QLabel *gammalabel = new QLabel(this);
    gammalabel->setText(i18n("Gamma:"));

    QLabel *redlabel = new QLabel(this);
    redlabel->setText(i18n("Red:"));

    QLabel *greenlabel = new QLabel(this);
    greenlabel->setText(i18n("Green:"));

    QLabel *bluelabel = new QLabel(this);
    bluelabel->setText(i18n("Blue:"));

    gctrl = new GammaCtrl(this, xv);
    connect(gctrl, SIGNAL(gammaChanged(int)), SLOT(Changed()));
    connect(gctrl, SIGNAL(gammaChanged(int)), SLOT(SyncScreens()));
    gammalabel->setBuddy( gctrl );

    rgctrl = new GammaCtrl(this, xv, XVidExtWrap::Red);
    connect(rgctrl, SIGNAL(gammaChanged(int)), SLOT(Changed()));
    connect(rgctrl, SIGNAL(gammaChanged(int)), SLOT(SyncScreens()));
    connect(gctrl, SIGNAL(gammaChanged(int)), rgctrl, SLOT(setCtrl(int)));
    connect(rgctrl, SIGNAL(gammaChanged(int)), gctrl, SLOT(suspend()));
    redlabel->setBuddy( rgctrl );

    ggctrl = new GammaCtrl(this, xv, XVidExtWrap::Green);
    connect(ggctrl, SIGNAL(gammaChanged(int)), SLOT(Changed()));
    connect(ggctrl, SIGNAL(gammaChanged(int)), SLOT(SyncScreens()));
    connect(gctrl, SIGNAL(gammaChanged(int)), ggctrl, SLOT(setCtrl(int)));
    connect(ggctrl, SIGNAL(gammaChanged(int)), gctrl, SLOT(suspend()));
    greenlabel->setBuddy( ggctrl );

    bgctrl = new GammaCtrl(this, xv, XVidExtWrap::Blue);
    connect(bgctrl, SIGNAL(gammaChanged(int)), SLOT(Changed()));
    connect(bgctrl, SIGNAL(gammaChanged(int)), SLOT(SyncScreens()));
    connect(gctrl, SIGNAL(gammaChanged(int)), bgctrl, SLOT(setCtrl(int)));
    connect(bgctrl, SIGNAL(gammaChanged(int)), gctrl, SLOT(suspend()));
    bluelabel->setBuddy( bgctrl );

    QGridLayout *grid = new QGridLayout();
    grid->setSpacing(8);
    grid->addMultiCellWidget(frame1, 0, 2, 0, 3);
    grid->addMultiCellWidget(frame2, 4, 8, 0, 3);
    grid->addWidget(gammalabel, 1, 1, Qt::AlignRight);
    grid->addWidget(redlabel, 5, 1, Qt::AlignRight);
    grid->addWidget(greenlabel, 6, 1, Qt::AlignRight);
    grid->addWidget(bluelabel, 7, 1, Qt::AlignRight);
    grid->addWidget(gctrl, 1, 2);
    grid->addWidget(rgctrl, 5, 2);
    grid->addWidget(ggctrl, 6, 2);
    grid->addWidget(bgctrl, 7, 2);

    topLayout->addLayout(grid);

    //Options
    KHBox *options = new KHBox(this);

    xf86cfgbox = new QCheckBox( i18n("Save settings system wide"), options );
    connect(xf86cfgbox, SIGNAL(clicked()), SLOT(changeConfig()));

    syncbox = new QCheckBox( i18n("Sync screens"), options );
    connect(syncbox, SIGNAL(clicked()), SLOT(SyncScreens()));
    connect(syncbox, SIGNAL(clicked()), SLOT(Changed()));

    screenselect = new QComboBox( options );
    for ( int i = 0; i < ScreenCount; i++ )
      screenselect->addItem( i18n("Screen %1", i+1) );
    screenselect->setCurrentIndex(currentScreen);
    connect(screenselect, SIGNAL(activated(int)), SLOT(changeScreen(int)));

    options->setSpacing( 10 );
    options->setStretchFactor( xf86cfgbox, 10 );
    options->setStretchFactor( syncbox, 1 );
    options->setStretchFactor( screenselect, 1 );

    topLayout->addWidget(options);
  }
  else {
    QLabel *error = new QLabel(this);
    error->setText(i18n("Gamma correction is not supported by your"
    " graphics hardware or driver."));
    error->setAlignment(Qt::AlignCenter);
    topLayout->addWidget(error);
  }
}

/** Restore latest saved gamma values */
void KGamma::load() {
  if (GammaCorrection) {
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
}

void KGamma::save() {
  if (GammaCorrection) {
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
          Arguments += rgamma[assign[i]] + ' ' + ggamma[assign[i]] + ' ' + \
                       bgamma[assign[i]] + ' ';
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
}

void KGamma::defaults() {
  if (GammaCorrection) {
    for (int i = 0; i < ScreenCount; i++) {
      xv->setScreen(i);
      gctrl->setGamma("1.00");
    }
    xv->setScreen(currentScreen);

  }
  xf86cfgbox->setChecked(false);
  syncbox->setChecked(false);
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
  Q3ValueList<int> ScreenNr;
  QString Section;
  XF86ConfigPath Path;

  QFile f( Path.get() );
  if ( f.open(QIODevice::ReadOnly) ) {
    QTextStream t( &f );
    QString s;
    int sn = 0;
    bool gm = false;

    // Analyse Screen<->Monitor assignments of multi-head configurations
    while ( !t.atEnd() ) {
      s = (t.readLine()).simplified();
      QStringList words = QStringList::split(' ', s);

      if ( !words.empty() ) {
        if ( words[0] == "Section" && words.size() > 1 ) {
          if ( (Section = words[1]) == "\"Monitor\"" ) gm = false;
        }
        else if ( words[0] == "EndSection" ) {
          if ( Section == "\"Monitor\"" && !gm ) {
            Gamma << "";
            gm = false;
          }
          Section = "";
        }
        else if ( words[0] == "Identifier" && words.size() > 1 ) {
          if ( Section == "\"Monitor\"" ) Monitor << words[1];
          else if ( Section == "\"Screen\"" ) Screen << words[1];
        }
        else if ( words[0] == "Screen" && words.size() > 1 ) {
          if ( Section == "\"ServerLayout\"" ) {
            bool ok;
            int i = words[1].toInt(&ok);
            if ( ok && words.size() > 2 ) {
              ScreenNr << i;
              ScreenLayout << words[2];
            }
            else {
              ScreenNr << sn++;
              ScreenLayout << words[1];
            }
          }
        }
        else if ( words[0] == "Monitor" && words.size() > 1 ) {
          if ( Section == "\"Screen\"" )
            ScreenMonitor << words[1];
        }
        else if ( words[0] == "Gamma" ) {
          if ( Section == "\"Monitor\"" ) {
            Gamma << s;
            gm = true;
          }
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
  return Default|Apply|Help;
}

QString KGamma::quickHelp() const
{
  return i18n("<h1>Monitor Gamma</h1> This is a tool for changing monitor gamma"
    " correction. Use the four sliders to define the gamma correction either"
    " as a single value, or separately for the red, green and blue components."
    " You may need to correct the brightness and contrast settings of your"
    " monitor for good results. The test images help you to find proper"
    " settings.<br> You can save them system-wide to XF86Config (root access"
    " is required for that) or to your own KDE settings. On multi head"
    " systems you can correct the gamma values separately for all screens.");
}


// ------------------------------------------------------------------------

extern "C"
{
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
