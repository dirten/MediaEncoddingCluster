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
namespace org {
  namespace esb {
    namespace lang {
      
      class LANG_EXPORT SharedObjectLoader {
      public:
        SharedObjectLoader(std::string);
        virtual ~SharedObjectLoader();
        void * getFunctionHandle(std::string name);
      private:
        void *_lib_handle;
        
      };
    }
  }
}

#endif	/* SHAREDOBJECTLOADER_H */

