/*----------------------------------------------------------------------
 *  File    : SystemCpuInfo.h
 *  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
 *  Purpose :
 *  Created : 15. Dezember 2009, 15:39 by Jan Hölscher <jan.hoelscher@esblab.com>
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

#include <string>
#include <map>
#include <vector>
//#include "org/esb/util/Log.h"
#include "exports.h"
#ifndef _SYSTEMCPUINFO_H
#define	_SYSTEMCPUINFO_H
namespace org {
    namespace esb {
        namespace lang {

            struct CpuInfo {
                int num;
                float freq;
                int family;
                int model;
                int step;
                int cores;
            };

            class LANG_EXPORT SystemCpuInfo {
              //classlogger("org.esb.lang.SystemCpuInfo")
            public:
                SystemCpuInfo();
                ~SystemCpuInfo();
                int getCpuCount();
                CpuInfo getCpuInfo(unsigned int cpu_num);
            private:
                void parseCpuInfo(std::string&);
                void parseProcessor(std::string);
                std::map<std::string, std::string> parseLines(std::string);
                std::vector<CpuInfo*> _processors;
            };
        }
    }
}


#endif	/* _SYSTEMCPUINFO_H */

