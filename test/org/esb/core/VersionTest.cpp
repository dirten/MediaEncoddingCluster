
#include <iostream>
#include <assert.h>
#include "org/esb/core/Version.h"

using org::esb::core::Version;
int main(int argc, char**argv){



  Version a("1.2.3");
  Version a2("1.2.3");
  Version b("1.2.4");
  //Version b2("MediaEncodingCluster-1.2.4-Linux.tar.gz");

  assert(a.newerAs(a2)==false);
  assert(a.olderAs(a2)==false);

  assert(a.newerAs(b)==false);
  assert(a.olderAs(b)==true);

  assert(b.newerAs(a)==true);
  assert(b.olderAs(a)==false);

  assert(a.equals(b)==false);
  assert(a.equals(a2)==true);
  assert(((&a)->equals(a2))==true);

}
