/*----------------------------------------------------------------------
 *  File    : SharedObjectLoader.cpp
 *  Author  : Jan Hölscher <jan.hoelscher@codergrid.de>
 *  Purpose :
 *  Created : 27. August 2011, 18:50 by Jan Hölscher <jan.hoelscher@codergrid.de>
 *
 *
 * MediaEncodingCluster, Copyright (C) 2001   Jan Hölscher
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA
 *
 * ----------------------------------------------------------------------
 */

#include "../SharedObjectLoader.h"
#include "../NotFoundException.h"
namespace org {
  namespace esb {
    namespace lang {

      SharedObjectLoader::SharedObjectLoader(std::string name) {
		      hMod = NULL;
	try
	{
		hMod = LoadLibrary( name.c_str() );
	}
	catch( std::exception &exc )
	{
		//LOGERROR("Could not load library"<<exc.what())
		//getLogger().log( Logger::LOG_ERROR, "[WinDllCreator#createObjectFromDll] Error occurred during loading DLL: %1", exc.what() );
		hMod = NULL;
	}

	if ( hMod == NULL )
	{
		std::string message = std::string("Error occurred during loading SharedObject: ") ;
        throw NotFoundException(__FILE__,__LINE__,message);
		//ObjectCreationException exc( "Error during loading DLL." );
		//throw exc;
	}

      }
      
      SharedObjectLoader::~SharedObjectLoader() {
        if(hMod)
          FreeLibrary(hMod);
      }
    }
  }
}
