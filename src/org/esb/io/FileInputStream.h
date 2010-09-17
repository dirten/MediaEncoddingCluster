/*----------------------------------------------------------------------
 *  File    : FileInputStream.h
 *  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
 *  Purpose :
 *  Created : 6. November 2009, 12:30 by Jan Hölscher <jan.hoelscher@esblab.com>
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
#ifndef ORG_ESB_IO_FILEINPUTSTREAM_H
#define ORG_ESB_IO_FILEINPUTSTREAM_H
#include "InputStream.h"
#include "File.h"
#include "org/esb/lang/Exception.h"
#include <iostream>
#include <stdio.h>
namespace org {
    namespace esb {
        namespace io {
class FileInputStream : public InputStream {
public:
/**
 * Creates a <code>FileInputStream</code> by
 * opening a connection to an actual file,
 * the file named by the <code>File</code>
 * object <code>file</code> in the file system.
 * A new <code>FileDescriptor</code> object
 * is created to represent this file connection.
 * <p>
 * If the named file does not exist, is a directory rather than a regular
 * file, or for some other reason cannot be opened for reading then a
 * <code>FileNotFoundException</code> is thrown.
 *
 * @param      file   the file to be opened for reading.
 * @exception  FileNotFoundException  if the file does not exist,
 *                   is a directory rather than a regular file,
 *                   or for some other reason cannot be opened for
 *                   reading.
 * @exception  IOException
 *               <code>read</code> method denies read access to the file.
 */

                FileInputStream(File * file);
                FileInputStream(const std::string);
                ~FileInputStream();
                long long int available(bool i=false);
                int read(string & str);
                int readLine(string & str);                
                int read(unsigned char * buffer, int length);
                int read(vector<unsigned char>&buffer);
                int read();
                void close();
private:
                void open(const std::string name);
                FILE * _file_handle;
                long _filePointer;
                char _byte;
            };
        }
    }
}
#endif
