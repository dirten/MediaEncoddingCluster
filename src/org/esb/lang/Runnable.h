#ifndef ORG_ESB_LANG_RUNNABLE_H
#define ORG_ESB_LANG_RUNNABLE_H

namespace org
{
  namespace esb
  {
    namespace lang
    {
      class Runnable
      {
      public:
        virtual ~Runnable(){}
        virtual void run() = 0;
      };

    }
  };

#endif /*ACTIVEMQ_CONCURRENT_RUNNABLE_H_*/
