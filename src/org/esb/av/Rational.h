/* 
 * File:   Rational.h
 * Author: jholscher
 *
 * Created on 29. August 2010, 16:34
 */

#ifndef RATIONAL_H
#define	RATIONAL_H
#include "AV.h"
namespace org {
  namespace esb {
    namespace av {
      
      class Rational:public AVRational {
      public:
        Rational(int num=1, int den=1000000);
        Rational(AVRational r);
        virtual ~Rational();
        bool operator==(const Rational &);
      private:

      };
    }
  }
}

#endif	/* RATIONAL_H */

