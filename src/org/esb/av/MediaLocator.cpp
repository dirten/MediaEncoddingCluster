/* 
 * File:   MediaLocator.cpp
 * Author: jholscher
 * 
 * Created on 29. August 2010, 14:22
 */

#include "MediaLocator.h"
#include "org/esb/util/StringTokenizer.h"
namespace org {
    namespace esb {
        namespace av {

            MediaLocator::MediaLocator(std::string inputfile):_file(inputfile) {
            }

            MediaLocator::~MediaLocator() {
            }

            std::string MediaLocator::getProtocol() {
              org::esb::util::StringTokenizer tok(_file,":");
              if(tok.countTokens()!=2)return "file";
              return tok.nextToken();
            }

            std::string MediaLocator::getRemainder() {
              org::esb::util::StringTokenizer tok(_file,":");
              if(tok.countTokens()!=2)return "";
              tok.nextToken();
              return tok.nextToken().substr(2);
            }

            std::string MediaLocator::toExternalForm() {
              return _file;
            }

            std::string MediaLocator::toString() {
              return _file;
            }

        }
    }
}

