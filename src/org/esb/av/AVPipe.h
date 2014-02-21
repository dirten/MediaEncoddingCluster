/*----------------------------------------------------------------------
 *  File    : AVPipe.h
 *  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
 *  Purpose :
 *  Created : 1. December 2013, 12:30 by Jan Hölscher <jan.hoelscher@esblab.com>
 *
 *
 * MediaEncodingCluster, Copyright (C) 2001-2013   Jan Hölscher
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
#ifndef AVPIPE_H
#define AVPIPE_H
#include "org/esb/lang/Ptr.h"
#include "exports.h"
#include <list>
namespace org {
  namespace esb {
    namespace av {
      class Packet;
      class Frame;
/**
  * @TODO : think about the need of the shared / plain pointer in addTarget
  *
*/
      class AV_EXPORT AVPipe
      {
        public:
          AVPipe();
          virtual ~AVPipe();
          void addTarget(AVPipe * pipe);
          void clearTargets();
        //protected:
          bool pushPacket(Ptr<Packet> p);
          bool pushFrame(Ptr<Frame> p);

        private:
          virtual bool newPacket(Ptr<Packet> p)=0;
          virtual bool newFrame(Ptr<Frame> p)=0;
          std::list< AVPipe * > _targets;
      };
    }
  }
}
#endif // AVPIPE_H
