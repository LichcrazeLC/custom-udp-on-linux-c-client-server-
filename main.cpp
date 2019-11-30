#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

using namespace std;

int main()
{
   cout << "Execution Started!" << endl;

   //Create Socket
   int listening = socket(AF_INET, SOCK_DGRAM, 0);
   if (listening == -1)
   {
      cerr << "Socket creation failed!" << endl;
      return -1;
   }

   //Bind Socket to IP / Port
   sockaddr_in hint;
   memset(&hint, 0, sizeof(hint)); 
   hint.sin_family = AF_INET;
   hint.sin_port = htons(8083);
   inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

   if (bind(listening, (struct sockaddr *)&hint, sizeof(hint)) == -1)
   {
      cerr << "Binding to IP/Port failed!" << endl;
      return -2;
   }

   //Declare client socket address
   sockaddr_in client;
   memset(&client, 0, sizeof(client)); 
   socklen_t clientSize = sizeof(client);

   //Display messages received while listening
   char buf[4096];

   // Clear the buffer
   memset(buf, 0, 4096);

   // Wait for a message
   int bytesReceived = recvfrom(listening, buf, 4096, MSG_WAITALL, (struct sockaddr *) &client, &clientSize);
   if (bytesReceived == -1)
   {
      cerr << "Connection failed!" << endl;
   }

   if (bytesReceived == 0)
   {
      cout << "Nothing received!" << endl;
   }

   // Display message
   cout << "Received: " << string(buf, 0, bytesReceived) << endl;

   // Resend message
   sendto(listening, buf, 4096, MSG_CONFIRM, (struct sockaddr *) &client, clientSize);

   return 0;
}