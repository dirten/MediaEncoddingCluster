/* 
 * File:   MediaLocator.h
 * Author: jholscher
 *
 * Created on 29. August 2010, 14:22
 */

#ifndef MEDIALOCATOR_H
#define	MEDIALOCATOR_H
#include "org/esb/io/File.h"
#include "exports.h"
namespace org {
    namespace esb {
        namespace av {

            class AV_EXPORT MediaLocator {
            public:
                MediaLocator(std::string);
                virtual ~MediaLocator();
                std::string getProtocol();
                std::string getRemainder();
                std::string toExternalForm();
                std::string toString();
            private:
                std::string _file;

            };
        }
    }
}

#endif	/* MEDIALOCATOR_H */

