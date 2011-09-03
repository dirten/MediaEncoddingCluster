/* 
 * File:   Application.h
 * Author: HoelscJ
 *
 * Created on 2. September 2011, 11:13
 */

#ifndef APPLICATION_H
#define	APPLICATION_H
#include <string>
namespace org {
  namespace esb {
    namespace core {

      class Application {
      public:
        Application(int argc, char * argv[]);
        virtual ~Application();
      private:

      };
    }
  }
}

std::string getConfig(std::string key);
void putConfig(std::string key, std::string val);


#endif	/* APPLICATION_H */

