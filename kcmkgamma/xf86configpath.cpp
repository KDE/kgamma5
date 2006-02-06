/***************************************************************************
                          xf86configpath.cpp  -  description
                             -------------------
    begin                : Mon Dec 30 2002
    copyright            : (C) 2002 by Michael v.Ostheim
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

#include <stdlib.h>
#include <unistd.h>

#include <vector>

#include "xf86configpath.h"

using namespace std;

XF86ConfigPath::XF86ConfigPath(){
    vector <string> searchPaths;
    searchPaths.push_back("/etc/X11/XF86Config-4");
    searchPaths.push_back("/etc/X11/XF86Config");
    searchPaths.push_back("/etc/XF86Config");
    searchPaths.push_back("/usr/X11R6/etc/X11/XF86Config-4");
    searchPaths.push_back("/usr/X11R6/etc/X11/XF86Config");
    searchPaths.push_back("/usr/X11R6/lib/X11/XF86Config-4");
    searchPaths.push_back("/usr/X11R6/lib/X11/XF86Config");

    searchPaths.push_back("/etc/X11/xorg.conf-4");
    searchPaths.push_back("/etc/X11/xorg.conf");
    searchPaths.push_back("/etc/xorg.conf");
    searchPaths.push_back("/usr/X11R6/etc/X11/xorg.conf-4");
    searchPaths.push_back("/usr/X11R6/etc/X11/xorg.conf");
    searchPaths.push_back("/usr/X11R6/lib/X11/xorg.conf-4");
    searchPaths.push_back("/usr/X11R6/lib/X11/xorg.conf");

    vector<string>::iterator it = searchPaths.begin();
    for (; it != searchPaths.end(); ++it )
      if ( !access( (Path = *it).c_str(), F_OK ) ) break;
}

XF86ConfigPath::~XF86ConfigPath(){
}

/** Returns path */
const char* XF86ConfigPath::get(){
  return( Path.c_str() );
}
