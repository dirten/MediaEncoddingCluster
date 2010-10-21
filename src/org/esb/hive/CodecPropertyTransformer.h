/* 
 * File:   CodecPropertyTransformer.h
 * Author: jholscher
 *
 * Created on 21. Oktober 2010, 11:54
 */

#ifndef CODECPROPERTYTRANSFORMER_H
#define	CODECPROPERTYTRANSFORMER_H
#include "org/esb/db/hivedb.hpp"
namespace org {
  namespace esb {
    namespace hive {

      class CodecPropertyTransformer {

      public:
        CodecPropertyTransformer(std::vector<db::StreamParameter> p);
        CodecPropertyTransformer(std::map<std::string, std::string> p);
        virtual ~CodecPropertyTransformer();
        std::map<std::string, std::string> getCodecProperties();
        std::string toString();
      private:
        std::map<std::string, std::string> _result;
        std::map<std::string, std::string> _prop2parent;
        void init();
        void process(std::map<std::string, std::string>);
      };
    }
  }
}

#endif	/* CODECPROPERTYTRANSFORMER_H */

