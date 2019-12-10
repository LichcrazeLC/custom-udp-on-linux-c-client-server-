#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

#include "networker.h"
#include "rsa.h"

using namespace std;

int main()
{
  cout << "Client Started!" << endl;

  networker protocol_worker;

  protocol_worker.init(8084, "0.0.0.0", 0);

  string sendBuf;
  
  while (true)
  {
    cout << endl << "Enter Message to Send: " << endl;
    getline(cin, sendBuf);
    protocol_worker.send(8083, "127.0.0.1", sendBuf.c_str(), 3);
  }
}
