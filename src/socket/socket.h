#ifndef SOCKET_H
#define SOCKET_H
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct Socket_t{
    char * hostname;
    int port;
};

class Socket{
    private:
	int 	port;
	char *	hostname;
	int 	socketFd;
	int 	connectFd;
	struct sockaddr_in socketaddr;
	void 	init();
    public:
			Socket(int);
			Socket();
			~Socket();
	bool 		send(unsigned char*);
	char          *	Recv();
	void 		setHostname(char*);
	void 		setPort(int);
	char 	      * getHostname();
	int 		getPort();
	void		Listen();
	int		Accept();
	void		Connect();
	void		Close();
};
#endif
