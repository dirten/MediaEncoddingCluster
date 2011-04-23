/*----------------------------------------------------------------------
 *  File    : StringOutputStream.h
 *  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
 *  Purpose :
 *  Created : 17. Februar 2010, 13:20 by Jan Hölscher <jan.hoelscher@esblab.com>
 *
 *
 * MediaEncodingCluster, Copyright (C) 2001-2009   Jan Hölscher
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


#ifndef _STRINGOUTPUTSTREAM_H
#define	_STRINGOUTPUTSTREAM_H
#include "OutputStream.h"
#include "exports.h"

namespace org{
  namespace esb{
    namespace io{
      class IO_EXPORT StringOutputStream: public OutputStream {
      public:
        StringOutputStream(std::string &);
        void write(char * buffer, int length);
        void write(string & str);
		void write(vector <unsigned char >& buffer);
        std::string getData();
      private:
        std::string & _data;
      };
    }
  }
}


#endif	/* _STRINGOUTPUTSTREAM_H */

