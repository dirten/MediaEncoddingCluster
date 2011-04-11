/*----------------------------------------------------------------------
 *  File    : StringInputStream.h
 *  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
 *  Purpose :
 *  Created : 17. Februar 2010, 13:17 by Jan Hölscher <jan.hoelscher@esblab.com>
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


#ifndef _STRINGINPUTSTREAM_H
#define	_STRINGINPUTSTREAM_H
#include "InputStream.h"
namespace org {
  namespace esb {
    namespace io {

      class StringInputStream : public InputStream {
      public:
        StringInputStream(std::string &);
        int read(unsigned char * buffer, int length);
        int read(vector<unsigned char>&buffer);
        int read(string & str);
        int read();
        long long int available(bool blocked=false);
      private:
        std::string _data;
        unsigned int _position;
      };
    }
  }
}


#endif	/* _STRINGINPUTSTREAM_H */

