/***************************************************************************
                          xvidextwrap.cpp  -  description
                             -------------------
    begin                : Sat Jan 5 2002
    copyright            : (C) 2002 by Michael v.Ostheim
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
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/xf86vmode.h>

#include <stdlib.h>
#include <iostream>
#include <fstream>

#include <vector>
#include <string>

#include <config.h>

#ifdef HAVE_SSTREAM
#include <sstream>
#else
#include <strstream.h>
#define istringstream istrstream
#endif

#include <kdebug.h>

#include "xf86configpath.h"
#include "xvidextwrap.h"

using namespace std;


XVidExtWrap::XVidExtWrap(bool* OK, const char* displayname) {
  if ((dpy = XOpenDisplay(displayname))) {
    screen = DefaultScreen(dpy);
    setGammaLimits(0.1, 10.0);
    *OK = true;
  }
  else {
    kdDebug() << "KGamma: unable to open display " << displayname << endl;
    *OK = false;
  }
}

XVidExtWrap::~XVidExtWrap() {
  if (dpy) XCloseDisplay(dpy);
}

int XVidExtWrap::_DefaultScreen() {
  return DefaultScreen(dpy);
}

/** We have to parse XF86Config to get the number of screens, because */
/** ScreenCount() from X11/Xlib.h does not count correctly with xinerama */
int XVidExtWrap::_ScreenCount() {
  int count = 0;
  bool section = false;
  XF86ConfigPath Path;

  std::ifstream in( Path.get() );

  if ( in.is_open() ) {
    std::string s, buf;
    std::vector<string> words;
      
    while (getline(in, s,'\n')) {
      words.clear();
      istringstream ss(s.c_str());
      while (ss >> buf) words.push_back(buf);  // Split "s" into words

      if ( !words.empty() ) {
        if ( words[0] == "Section" && words.size() > 1 ) {
          if ( words[1] == "\"ServerLayout\"" ) section = true;
        }
        else if ( words[0] == "EndSection" )
           section = false;
        if ( section && words[0] == "Screen" ) ++count;
      }
    } //end while
    in.close();

  }

  if ( !count ) count = 1;  //If failed,fill count with a valid value;
  
  return( count );
}

const char* XVidExtWrap::DisplayName() {
  return DisplayString(dpy);
}

void XVidExtWrap::setGamma(int channel, float gam, bool* OK) {
  XF86VidModeGamma gamma;

  if ( gam >= mingamma && gam <= maxgamma ) {
    if (!XF86VidModeGetGamma(dpy, screen, &gamma)) {
      kdDebug() << "KGamma: Unable to query gamma correction" << endl;
      if ( OK ) *OK = false;
    }
    else {
      switch (channel) {
        case Value:
          gamma.red = gam;
          gamma.green = gam;
          gamma.blue = gam; break;
        case Red:
          gamma.red = gam; break;
        case Green:
          gamma.green = gam; break;
        case Blue:
          gamma.blue = gam;
      };
      if (!XF86VidModeSetGamma(dpy, screen, &gamma)) {
        kdDebug() << "KGamma: Unable to set gamma correction" << endl;
        if ( OK ) *OK = false;
      }
      else {
        XFlush(dpy);
        if ( OK ) *OK = true;
      }
    }
  }
}

float XVidExtWrap::getGamma(int channel, bool* OK) {
  XF86VidModeGamma gamma;
  float gam = 0;

  if (!XF86VidModeGetGamma(dpy, screen, &gamma)) {
    kdDebug() << "KGamma: Unable to query gamma correction" << endl;
    if ( OK ) *OK = false;
  }
  else {
    switch (channel) {
      case Value:
        gam = gamma.red; break;
      case Red:
        gam = gamma.red; break;
      case Green:
        gam = gamma.green; break;
      case Blue:
        gam = gamma.blue;
    };
    if ( OK ) *OK = true;
  }
  return(gam);
}

void XVidExtWrap::setGammaLimits( float min, float max ) {
  mingamma = (min < 0.1) ? 0.1 : min;
  maxgamma = (max > 10.0) ? 10.0 : max;
}

#ifdef istringstream
#undef istringstream
#endif
