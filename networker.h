#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

#ifndef NETWORKER_H
#define NETWORKER_H

using namespace std;

#define HEADER_SIZE 128

class networker
{
private:
  int socketfd;
  sockaddr_in hint;
  hash<string> mystdhash;

  //RSA
  double publicKey = 0;
  double privateKey = 0;

  int sentKey = 0;
  int receivedKey;

  sockaddr_in client;
  socklen_t len = sizeof(client);
  
  char buf[4096];
  char sendBuf[4096];

  void flushBuffers();

public:

  int send(uint16_t port, const char *host, const char* message, int FLAGS);

  const char *listen(int SEND_RESPONSE, int SEND_RSA_KEY);

  void init(uint16_t port, const char *host, int generateKeyPair);
};

#endif
