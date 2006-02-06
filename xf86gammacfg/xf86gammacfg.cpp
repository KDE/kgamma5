/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Wed Aug  7 18:11:19 CEST 2002
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

#include <stdio.h>

#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <config.h>

#ifdef HAVE_SSTREAM
#include <sstream>
#else
#include <strstream.h>
#define istringstream istrstream
#endif

using namespace std;

int main(int argc, char *argv[])
{

  bool cpyonly, secmon, success;

  cpyonly = secmon = success = false;

  if ( !((argc-1) % 3) ) {
    int Screen = 0;
    int ScreenCount = (argc-1) / 3;
    
    // First, search for XF86Config or xorg.conf
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

    std::vector<string>::iterator it = searchPaths.begin();
    for (; it != searchPaths.end(); ++it ) {
      //Try to open file
      std::ifstream in( (*it).c_str() );
            
      if ( in.is_open() ) {
        std::ofstream out( ( (*it) + ".tmp" ).c_str() );
        if ( out.is_open() ) {
          std::string s, buf;
          std::vector<string> words;
        
          while (getline(in, s,'\n')) {
            if (!cpyonly) {
              words.clear();
              std::istringstream ss(s.c_str());
              while (ss >> buf) words.push_back(buf);

              if ( !words.empty() ) {
                if ( words[0] == "Section" && words.size() > 1 ) {
                  if ( words[1] == "\"Monitor\"" ) {
                    secmon = true;
                    out << s << endl;
                    continue;
                  }
                }
                if ( secmon ) {
                  if ( words[0] == "Gamma" ) {
                    out << "  Gamma   " << argv[3*Screen+1] << "  " << \
                        argv[3*Screen+2]<< "  " << argv[3*Screen+3];
                    out << "  # created by KGamma" << endl;
                    cpyonly = success = ( ++Screen == ScreenCount );
                    secmon = false;
                    continue;
                  }
                  if ( words[0] == "EndSection" ) {
                    out << "  Gamma   " << argv[3*Screen+1] << "  " << \
                        argv[3*Screen+2]<< "  " << argv[3*Screen+3];
                    out << "  # created by KGamma" << endl;
                    out << s << endl;
                    cpyonly = success = ( ++Screen == ScreenCount );
                    secmon = false;
                    continue;
                  }
                }
              }
            }
            out << s << endl;
          } //endwhile
        
          in.close();
          out.close();

          if ( success ) {
            rename( (*it).c_str(),(*it + ".kgammaorig").c_str() );
            rename( (*it + ".tmp").c_str(),(*it).c_str() );
          }
          else remove( (*it + ".tmp").c_str() );
          break;
        }
      }
    }
  }
  if ( !success )
    cerr << "Usage: xf86gammacfg RGAMMA GGAMMA " \
            "BGAMMA [RGAMMA GGAMMA BGAMMA [...]]" << endl;

  return success;
}

#ifdef istringstream
#undef istringstream
#endif
