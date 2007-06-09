#include "org/esb/io/OutputStream.h"
#include "Packet.h"

using namespace org::esb::io;
namespace org{
	namespace esb{
		namespace av{
			class PacketOutputStream: public OutputStream{
				public:
					PacketOutputStream(OutputStream * os);
					~PacketOutputStream();
					void writePacket(Packet * packet);
					void write(char * buffer, int length);
				private:
					OutputStream * _target;
			};
		}
	}
}

