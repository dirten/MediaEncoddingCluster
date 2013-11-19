#ifndef SELECTOR_H
#define SELECTOR_H
#include <string>
namespace org{
  namespace esb{
    namespace mq{
      class QueueMessage;
      class Selector
      {
        public:
          Selector();
          Selector(std::string sel);
          bool match(QueueMessage & msg);

        private:
          std::string _name;
          std::string _value;
      };
    }
  }
}
#endif // SELECTOR_H
