/*----------------------------------------------------------------------
 *  File    : StreamInfo.h
 *  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
 *  Purpose :
 *  Created : 02.03.2010, 20:30 by Jan Hölscher <jan.hoelscher@esblab.com>
 *
 *
 * MediaEncodingCluster, Copyright (C) 2001-2010   Jan Hölscher
 *
 * This program License under the terms in the LICENSE file
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * ----------------------------------------------------------------------
 */
#ifndef STREAMINFO_H_
#define STREAMINFO_H_
#include <Wt/Ext/Panel>
#include "org/esb/sql/ResultSet.h"
namespace org {
  namespace esb{
    namespace web{
      class StreamInfo : public Wt::Ext::Panel{
      public:
        StreamInfo(org::esb::sql::ResultSet *);
        virtual ~StreamInfo();
      };
    }
  }
}

#endif /* STREAMINFO_H_ */
