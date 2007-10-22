#include <vector>

using namespace std;
namespace org{
namespace esb{
namespace hive{
namespace job{
class ClientHandler{
    public:
	ClientHandler();
	vector<void*>getPacketList();
	void setPacketList(vector<void*>);
    private:
	vector<void*> list;
};
}}}}

