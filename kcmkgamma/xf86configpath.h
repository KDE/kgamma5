/***************************************************************************
                          xf86configpath.h  -  description
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

#ifndef XF86CONFIGPATH_H
#define XF86CONFIGPATH_H

#include <string>

/**Search for XF86Config or XF86Config-4 which can be located at different
  places.
  *@author Michael v.Ostheim
  */

class XF86ConfigPath {
  
public: 
  XF86ConfigPath();
  ~XF86ConfigPath();

  /** Returns Path variable */
  const char* get();

private: // Private attributes
  /** Contains the path of XF86Config file */
  std::string Path;
};

#endif
