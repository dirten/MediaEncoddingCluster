/* 
 * File:   CodecPropertyTransformer.cpp
 * Author: jholscher
 * 
 * Created on 21. Oktober 2010, 11:54
 */

#include <vector>

#include "CodecPropertyTransformer.h"
#include "org/esb/av/AV.h"
#include "org/esb/util/Log.h"
namespace org {
  namespace esb {
    namespace hive {

      CodecPropertyTransformer::CodecPropertyTransformer(std::vector<db::StreamParameter> p) {
        init();
        std::vector<db::StreamParameter>::iterator it=p.begin();
        std::map<std::string, std::string> data;
        for(;it!=p.end();it++){
          data[(*it).name]=(*it).val;
        }
        process(data);
      }

      CodecPropertyTransformer::CodecPropertyTransformer(std::map<std::string, std::string> p) {
        init();
        process(p);
      }

      CodecPropertyTransformer::~CodecPropertyTransformer() {
      }

      void CodecPropertyTransformer::process(std::map<std::string, std::string> data) {
        std::map<std::string, std::string>::iterator it=data.begin();
        for(;it!=data.end();it++){
          LOGDEBUG("Processing Property "<<(*it).first);
          if(_result.count((*it).first)>0){
            _result[(*it).first]=(*it).second;
          }else
          if(_prop2parent.count((*it).first)>0){
            LOGDEBUG("Properyt parent found : "<<_prop2parent[(*it).first]);
            if((*it).second.length()>0&&(*it).second!="0")
              _result[_prop2parent[(*it).first]].append("+").append((*it).first);
          }else{
            LOGWARN("Property \""<<(*it).first<< "\" with value \""<<(*it).second<<"\" is unknown!");
          }
        }
      }

      void CodecPropertyTransformer::init() {
        const AVOption * option = NULL;
        AVCodecContext * codec = avcodec_alloc_context2(AVMEDIA_TYPE_UNKNOWN);
        while (option = av_next_option(codec, option)) {
          if (option->flags & AV_OPT_FLAG_ENCODING_PARAM||option->flags==0) {
            std::string name = option->name;
            std::string unit = option->unit != NULL ? option->unit : "";
            if (option->offset == 0)
              _prop2parent[name] = unit;
            else
              _result[name] = "";
          }
        }
      }

      std::map<std::string, std::string> CodecPropertyTransformer::getCodecProperties() {
        return _result;
      }

      std::string CodecPropertyTransformer::toString() {
        std::map<std::string, std::string>::iterator it = _result.begin();
        std::string result;
        for (; it != _result.end(); it++) {
          if ((*it).second.length() > 0)
            result += (*it).first + "=" + (*it).second + ";";
        }
        return result;
      }
    }
  }
}

