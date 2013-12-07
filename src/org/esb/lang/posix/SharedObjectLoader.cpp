/*----------------------------------------------------------------------
 *  File    : SharedObjectLoader.cpp
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

#include "../SharedObjectLoader.h"
#include "../NotFoundException.h"
//#include "org/esb/util/Log.h"
#include <dlfcn.h>
namespace org {
  namespace esb {
    namespace lang {

      SharedObjectLoader::SharedObjectLoader(std::string filename) {
        _lib_handle = NULL;
        _filename=filename;
        try {
          _lib_handle = dlopen(filename.c_str(), RTLD_LAZY);
        } catch (std::exception &exc) {
          _lib_handle = NULL;
        }
        if (!_lib_handle) {
          std::string message = std::string("Error occurred during loading SharedObject: ") + dlerror();
          throw NotFoundException(__FILE__,__LINE__,message);
        }
      }

      SharedObjectLoader::~SharedObjectLoader() {
        if(dlclose(_lib_handle)){
          //LOGERROR("closing shared object : "<<_filename<<" : "<<dlerror());
        }else{
          //LOGDEBUG("successfully closed shared object:"<<_filename);
        }
      }
    }
  }
}
