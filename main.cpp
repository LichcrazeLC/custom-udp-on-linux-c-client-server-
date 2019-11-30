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
   hint.sin_family = AF_INET;
   hint.sin_port = htons(54000);
   inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

   if (bind(listening, (struct sockaddr *) &hint, sizeof(hint)) == -1)
   {
      cerr << "Binding to IP/Port failed!" << endl;
      return -2;
   }

   //Tell the socket to listen
   if (listen(listening, SOMAXCONN) == -1)
   {
      cerr << "Listening failed!" << endl;
      return -3;
   }

   //Accept the call
   sockaddr_in client;
   socklen_t clientSize = sizeof(client);
   char host[NI_MAXHOST];
   char svc[NI_MAXSERV];

   int clientSocket = accept(listening, (struct sockaddr *) &client, &clientSize);

   if (clientSocket == -1)
   {
      cerr << "Client connecting failed!" << endl;
      return -4;
   }

   //Close the listening socket
   close(listening);

   memset(host, 0, NI_MAXHOST);
   memset(svc, 0, NI_MAXSERV);

   int result = getnameinfo((sockaddr*)&client, 
                           sizeof(client), 
                           host, NI_MAXSERV, 
                           svc, NI_MAXSERV, 0);

   if (result)
   {
      cout << host << " connected on " << svc << endl;
   }
   else
   {
      inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
      cout << host << " connected on " << ntohs(client.sin_port) << endl;
   }

   //Display messages received while listening
   char buf[4096];
   while (true)
   {
      // Clear the buffer
      memset(buf, 0, 4096);

      // Wait for a message
      int bytesReceived = recv(clientSocket, buf, 4096, 0);
      if (bytesReceived == -1)
      {
         cerr << "Connection failed!" << endl;
         break;
      }

      if (bytesReceived == 0)
      {
         cout << "The client disconnected" << endl;
         break;
      }

      // Display message
      cout << "Received: " << string(buf, 0, bytesReceived) << endl;
  
      // Resend message
      send(clientSocket, buf, bytesReceived + 1, 0);

   }

   //Close the socket
   close(clientSocket);

   return 0;

}