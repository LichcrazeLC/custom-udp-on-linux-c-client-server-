#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

#define SERVER_PORT 8083

using namespace std;

// Driver code
int main()
{

  //Create Socket
  int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sockfd == -1)
  {
    cerr << "Socket creation failed!" << endl;
    return -1;
  }

  //Bind the Socket
  sockaddr_in hint;
  memset(&hint, 0, sizeof(hint));
  hint.sin_family = AF_INET;
  hint.sin_port = htons(8084);
  inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

  if (bind(sockfd, (struct sockaddr *)&hint, sizeof(hint)) == -1)
  {
    cerr << "Binding to IP/Port failed!" << endl;
    return -2;
  }

  //Buffer for receiving bytes
  char buf[4096];
  memset(buf, 0, 4096);

  //Buffer for sending bytes
  char sendBuf[4096];
  memset(sendBuf, 0, 4096);

  //Declare server sockaddr
  struct sockaddr_in servaddr;
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(8083);
  inet_pton(AF_INET, "localhost", &servaddr.sin_addr);
  socklen_t len = sizeof(servaddr);

  int n;

  string message;

  while (true)
  {

    cout << "Enter Message to Send: " << endl;

    cin.getline(sendBuf,sizeof(sendBuf));;

    if (sendto(sockfd, sendBuf, sizeof(sendBuf), 0, (const struct sockaddr *)&servaddr, len) != -1)
    {
      cout << "Message Sent!" << endl;
    };

    n = recvfrom(sockfd, buf, 4096, 0, (struct sockaddr *)&servaddr, &len);

    cout << "Message Received from Server: " << buf << endl;

    memset(buf, 0, 4096);
  }

  return 0;
}
