// toralize.h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <dlfcn.h> // for dlsyms
/*For Ubuntu use #include <sys/socket.h, the mentioned header is defined in IEEE Std. 1003.1 (POSIX), Windows is not compliant with POSIX Standard
Or you can emulate the POSIX environment in Windows by installing GCC using Cygwin, or Virtual Box with a Ubuntu VM is always an option*/
/*#include <arpa/inet.h>
#include <netinet/in.h>
These includes in Windows will give a Fatal error
For windows you require to use #include <winsock2.h>*/

socket() // to initialize the socket
connect()
close()
htons() // port number
inet_addr()  // takes a string (IP) and transforms it to a big number which transform to the network byte order

#define PROXY "127.0.0.1"
#define PROXYPORT 9050
#define USERNAME "toraliz"
#define reqsize sizeof(struct proxy_request)
#define ressize sizeof(struct proxy_response)

typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;

/*
        +----+----+----+----+----+----+----+----+----+----+....+----+
		| VN | CD | DSTPORT |      DSTIP        | USERID       |NULL|
		+----+----+----+----+----+----+----+----+----+----+....+----+
 #bytes	    1    1      2              4           variable       1

*/

struct proxy_request{
    int8 vn;   // refer to the fields
    int8 cd;   // go on, search up "socks v4 rfc" and onto the first link
    int16 dstport;
    int32 dstip;
    unsigned char userID[8];
};
typedef struct proxy_request Req;
/*
        +----+----+----+----+----+----+----+----+
		| VN | CD | DSTPORT |      DSTIP        |
		+----+----+----+----+----+----+----+----+
 #bytes:   1    1      2              4

*/

struct proxy_response{
    int8 vn;
    int8 cd;
    int16 _;   //dstport, these are not named because they are not important
    int32 __;  //dstip

};

typedef struct proxy_response Res;

Req *request(struct sockaddr_in*); // Signature
int connect(int, const struct sockaddr*, socklen_t); 