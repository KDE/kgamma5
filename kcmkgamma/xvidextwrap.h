/***************************************************************************
                          xvidextwrap.h  -  description
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

#ifndef XVIDEXTWRAP_H
#define XVIDEXTWRAP_H

struct _XDisplay;

/**A wrapper for XF86VidMode Extension
  *@author Michael v.Ostheim
  */

class XVidExtWrap {

  public:

    enum GammaChannel { Value = 0, Red = 1, Green = 2, Blue = 3 };
    XVidExtWrap(bool *OK, const char* displayname = NULL);
    ~XVidExtWrap();

    /** Returns the default screen */
    int _DefaultScreen();
    /** Returns the number of screens (extracted from XF86Config) */
    int _ScreenCount();
    /** Returns the displayname */
    const char* DisplayName();
    /** Sets the screen actions are take effect */
    void setScreen( int scrn ) { screen = scrn; };
    /** Returns the current screen */
    int getScreen() { return screen; };
    /** Sets the gamma value on the current screen */
    void setGamma( int channel, float gam, bool *OK = NULL );
    /** Gets the gamma value of the current screen */
    float getGamma( int channel, bool *OK = NULL );
    /** Limits the possible gamma values (default 0.1-10.0) */
    void setGammaLimits( float min, float max );

  private:
    float mingamma, maxgamma;
    int screen;
    Display* dpy;
};

#endif

