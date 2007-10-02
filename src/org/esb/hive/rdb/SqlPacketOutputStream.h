#include "org/esb/io/OutputStream.h"


namespace org
{
  namespace esb
  {
    namespace hive
    {
      namespace rdb
      {

	class SqlPacketOutputStream:public org::esb::io::OutputStream
	{
	public:
	  SqlPacketOutputStream ();
	  ~SqlPacketOutputStream ();

	};
      }
    }
  }
}
