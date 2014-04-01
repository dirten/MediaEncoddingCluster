#ifndef STORAGEPLUGIN_H
#define STORAGEPLUGIN_H

#include "PluginRegistry.h"
#include "Plugin.h"


namespace org {
  namespace esb {
    namespace core {
      class StorageEngine;
      class StoragePlugin: public org::esb::core::Plugin{
      public:
        virtual void startup()=0;
        virtual void shutdown()=0;
        virtual ~StoragePlugin(){}
      private:

      };
    }
  }
}
/*
#define REGISTER_STORAGE(name,type) \
  class Register##type \
          { \
                        org::esb::core::StorageEngine* element##type; \
                  public: \
                          Register##type() \
                          { \
                                element##type=new type(); \
                          org::esb::core::PluginRegistry::getInstance()->registerStorage(std::string(name), element##type); \
                          } \
                                ~Register##type(){ \
                                delete element##type; \
                                } \
          } Register##type##Instance;

*/
#endif // STORAGEPLUGIN_H
