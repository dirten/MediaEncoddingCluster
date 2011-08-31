/*----------------------------------------------------------------------
 *  File    : SharedObjectLoader.h
 *  Author  : Jan Hölscher <jan.hoelscher@codergrid.de>
 *  Purpose :
 *  Created : 27. August 2011, 18:50 by Jan Hölscher <jan.hoelscher@codergrid.de>
 *
 *
 * MediaEncodingCluster, Copyright (C) 2001   Jan Hölscher
 *
 * This program is free software; you can redistribute it and/or
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
#include "exports.h"
#ifndef SHAREDOBJECTLOADER_H
#define	SHAREDOBJECTLOADER_H
#include <string>
#include "org/esb/util/Log.h"
#include <boost/function.hpp>
#if defined __LINUX__ || defined __APPLE__ 
#include <dlfcn.h>
#endif
#include "NotFoundException.h"

namespace org {
  namespace esb {
    namespace lang {

      class LANG_EXPORT SharedObjectLoader {
        classlogger("org.esb.lang.SharedObjectLoader");
      public:
        SharedObjectLoader(std::string);
        virtual ~SharedObjectLoader();

        template<class T>
        boost::function<T>
        getFunctionHandle(std::string name) {
          void * result = NULL;
#if defined __LINUX__ || defined __APPLE__ 
          try {
            result = dlsym(_lib_handle, name.c_str());
          } catch (std::exception & ex) {
            result = NULL;
            LOGERROR("Error occurred during loading function :" << ex.what());
          }
          if (!result) {
            std::string message = std::string("Error occurred during loading SharedObject: ") + dlerror();
            throw NotFoundException(__FILE__,__LINE__,message);
          }
#elif __WIN32__/*
     HMODULE hMod = NULL;
	try
	{
		hMod = LoadLibrary( str.str().c_str() );
	}
	catch( exception &exc )
	{
		getLogger().log( Logger::LOG_ERROR, "[WinDllCreator#createObjectFromDll] Error occurred during loading DLL: %1", exc.what() );
		hMod = NULL;
	}

	if ( hMod == NULL )
	{
		ObjectCreationException exc( "Error during loading DLL." );
		throw exc;
	}
	
	try
	{
		pFunc = ( DLLPROC ) GetProcAddress(hMod, "createObject"); 
	}
	catch( exception &exc)
	{
		getLogger().log( Logger::LOG_ERROR, "[WinDllCreator#createObjectFromDll] Error occurred during calling DLL entry method, %1", exc.what() );
		pFunc = NULL;
	}

	if ( pFunc == NULL )
	{
		ObjectCreationException exc( "Error during loading object from DLL." );
		throw exc;
	}	     */
#else
#error "no SharedObjectLoader for this plattform"
#endif
          return reinterpret_cast<T*> (result); //result;
        }
      private:
        void *_lib_handle;
      };
    }
  }
}

#endif	/* SHAREDOBJECTLOADER_H */

