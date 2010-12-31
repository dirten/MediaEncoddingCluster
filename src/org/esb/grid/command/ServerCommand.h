/* 
 * File:   ServerCommand.h
 * Author: HoelscJ
 *
 * Created on 31. Dezember 2010, 13:03
 */

#ifndef SERVERCOMMAND_H
#define	SERVERCOMMAND_H
#include "Command.h"
namespace org {
  namespace esb {
    namespace grid {
      class ServerCommand: public Command {
      public:
      enum Type{
        START,
        STOP
      };
        ServerCommand(Type);
        virtual ~ServerCommand();
      private:

      };
    }
  }
}
#endif	/* SERVERCOMMAND_H */

