/* 
 * File:   PresetVerifier.h
 * Author: HoelscJ
 *
 * Created on 19. September 2011, 13:37
 */

#ifndef PRESETVERIFIER_H
#define	PRESETVERIFIER_H
#include "org/esb/libjson/libjson.h"
namespace org {
  namespace esb {
    namespace plugin {

      class PresetVerifier {
      public:
        static std::string verify(JSONNode&root);
      private:
        PresetVerifier();
        virtual ~PresetVerifier();

      };
    }
  }
}

#endif	/* PRESETVERIFIER_H */

