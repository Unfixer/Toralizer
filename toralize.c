// toralize.c

/*
To redirect the library functions, that is in order to intercept any command from a program we need to:
1. Turn the client into a LIB (SHARED LIBRARY) .SO
2. Turn main() into out own connect()
3. Replace regular connect()
4. Grab the IP and Port from the original connect(). okay?
5. Then we leave the rest to the original application to take care of the communication

*/
#include "toralize.h"

Req *request(stuct sockaddr_in *sock2) { // this is the function the packet that initializes the proxy connection
    Req *req;
    req = malloc(reqsize); // allocating memory for the structure

    req->vn = 4; // dont panic the arrow is the same as dot(.) structure
    req->cd = 1;
    req->dstport = sock2->sin_port;  // Flag 2
    req->dstip = sock2->sin_addr.s_addr;  // Flag 3
    strncpy(req->userID, USERNAME, 8);

    return req;
    }

    int connect(int s2, const struct sockaddr *sock2, socklen_t addrlen) {// refer to documentation (.txt file)
    // Flag 1
    int s; // s for socket file descriptor
    struct sockaddr_in sock;
    Req *req;
    char buf[ressize];
    int success; // to be used as a predicate/property : Look at Documentation for definition
    char tmp[512];
     int (*p)(int , const struct sockaddr*, socklen_t );  // function pointer with same signature as connect

    
    p = dlsyms(RTLD_NEXT, "connect"); // connect function points here

    s = socket(AF_INET, SOCK_STREAM, 0); // don't panic these are standardized
    if (s < 0) {    // to check serial number
        perror("socket"); // to see which function returned the error
        return -1;
    }
    // To create the TCP connection
    sock.sin_family = AF_INET;
    sock.sin_port = htons(PROXYPORT);
    sock.sin_addr.s_addr = inet_addr(PROXY);

    // Intialize the actual connect command

    if (p(s, (struct sockaddr*)&sock, sizeof(sock))) { // This line connects to the server
    perror("connect");

    return -1;
    }

    printf("Connected to proxy\n");
    req = request((struct sockaddr_in *)sock2);
    write(s, req, reqsize); // to send the data
    
    memset(buf, 0, ressize); // always make sure to clear the buffer like this. Use it in your day to day programming as well
    if (read(s, buf, resszie) < 1) // number of bytes are returned
    {
        perror("read");
        free(req);
        close(s); // close the socket files

        return -1;
    }

    res = (Res *)buf;
    success = (res->cd == 90);  // if the cd is 90 then success will be true and otherwise
    if(!success) {
       fprintf(stderr, "Unable to traverse the proxy, error code: %d\n", res->cd);
         close(s);
       free(req);

       return -1;
    }

    printf("Successfully connected through the proxy.\n");
    
   // Flag 4
    
   /*Here we need to pipe our socket to the end application socket
   -> s -> s2
   -> s2 -> s
   Basically becoming the man in the middle
   This is also called piping and can be done by a system call called dup2
   Basically it creates a new file descriptor with the same value as the second argument*/

   dup2(s, s2);

    free(req);

    return 0;
}