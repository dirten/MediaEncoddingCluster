/* 
 * File:   NodeCmdHandler.h
 * Author: HoelscJ
 *
 * Created on 1. Oktober 2010, 13:28
 */

#ifndef NODECMDHANDLER_H
#define	NODECMDHANDLER_H
namespace org {
  namespace esb {
    namespace grid {

      class NodeCmdHandler {
      public:
        NodeCmdHandler();
        bool handleCommand(std::string & cmddata);
        virtual ~NodeCmdHandler();
      private:

      };
    }
  }
}

#endif	/* NODECMDHANDLER_H */

