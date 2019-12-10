#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>

#include "networker.h"
#include "rsa.h"

using namespace std;

void networker::flushBuffers()
{
  memset(buf, 0, 4096);
  memset(sendBuf, 0, 4096);
}

int networker::send(uint16_t port, const char *host, const char *message, int FLAGS)
{

  //IF THIS IS A RESENDING OF A PREVIOUS MESSAGE
  if (FLAGS == 2)
  {
    sendto(socketfd, sendBuf, sizeof(sendBuf), 0, (const struct sockaddr *)&client, sizeof(client));
    listen(0, 0);
    return 2;
  }

  flushBuffers();

  size_t chksum = mystdhash(message);

  //IF THIS IS A RESENDING REQUEST TO A CLIENT
  if (FLAGS == 1)
  {
    chksum = mystdhash("RESEND_REQUEST");
  }
  //IF THIS IS A KEY SEND
  else if (FLAGS == 4)
  {
    chksum = mystdhash("RSA_KEY");
    message = to_string(publicKey).c_str();
  }

  cout << "checksum of msg to send: " << chksum << endl;

  char headerContent[1028];
  memset(headerContent, 0, 1028);

  size_t *checksumptr = &chksum;

  memcpy(headerContent, checksumptr, sizeof(chksum));
  int headerSize = sizeof(headerContent);

  checksumptr = (size_t *)headerContent;

  memcpy(sendBuf, &headerSize, sizeof(headerSize));
  memcpy(&sendBuf[sizeof(headerSize)], headerContent, headerSize);
  memcpy(&sendBuf[sizeof(headerSize) + headerSize], message, strlen(message));

  memset(&client, 0, sizeof(client));
  client.sin_port = htons(port);
  client.sin_family = AF_INET;
  inet_pton(AF_INET, host, &client.sin_addr);

  if (receivedKey == 1)
  {
    for (int i = headerSize + sizeof(headerSize); i < headerSize + sizeof(headerSize) + strlen(message); ++i)
    {
      uint8_t nr = sendBuf[i];
      cout << "Char to be encrypted: " << nr << endl;
      sendBuf[i] = (uint8_t)(encrypt(nr, publicKey));
      cout << "After encryption: " << encrypt(nr, getPublicKey()) << endl;
    }
  }

  if (sendto(socketfd, sendBuf, sizeof(sendBuf), 0, (const struct sockaddr *)&client, sizeof(client)) != -1)
  {
    cout << "Message Sent!" << endl;
  };

  //IF THE SENDER ALSO WANTS TO LISTEN
  if (FLAGS == 3)
  {
    listen(0, 0);
    return 3;
  }
  else
  {
    return 0;
  }
}

const char *networker::listen(int SEND_RESPONSE, int SEND_RSA_KEY)
{
  memset(buf, 0, 4096);

  int bytesReceived = recvfrom(socketfd, buf, 4096, 0, (struct sockaddr *)&client, &len);

  if (bytesReceived == -1)
  {
    cerr << "Connection failed!" << endl;
  }

  if (bytesReceived == 0)
  {
    cout << "Nothing received!" << endl;
  }

  //Display message received while listening
  int *headerLength = (int *)buf;

  cout << "Header Length: " << *headerLength << endl;

  size_t *checksum = (size_t *)&buf[sizeof(*headerLength)];

  // size_t ch = mystdhash("whateverValue");
  // size_t *checksum = &ch;

  cout << "Header: " << endl;

  cout << *checksum << endl;

  string msgReceived;

  cout << endl
       << "Message Received: " << endl;

  for (int i = *headerLength + sizeof(*headerLength); i < sizeof(buf); ++i)
  {
    cout << (char)buf[i];
    //cout << "Decrypted char:" << (char) decrypt((uint8_t) buf[i], getPrivateKey(getPublicKey()));
    if (sentKey == 1)
    {
      msgReceived += (char)decrypt((uint8_t)buf[i], privateKey);
    }
    else
    {
      msgReceived += buf[i];
    }
  }

  if (sentKey == 1)
  {
    cout << endl
         << "Decrypted message: " << msgReceived << endl;
  }

  char addr[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &(client.sin_addr), addr, INET_ADDRSTRLEN);

  string message;

  if (*checksum == mystdhash("RESEND_REQUEST"))
  {
    cout << endl
         << "Server Spotted an Error! Resending Previous Packet...." << endl;
    send(8084, addr, "This is a resend!!!", 2);
    return buf;
  }

  if (*checksum == mystdhash("RSA_KEY"))
  {
    cout << endl
         << "Received encryption key! Will use it to encrypt further messages...." << endl;
    publicKey = atof(msgReceived.c_str());
    cout << "Key copied: " << publicKey << endl;
    receivedKey = 1;
    return buf;
  }

  if (*checksum != mystdhash(msgReceived.data()))
  {
    cout << endl
         << "Error spotted, Asking for a resend...!" << endl;
    send(8084, addr, "Message Received is Flawed! Resend it to me!", 1);
    return buf;
  }

  //Resend message
  if (SEND_RESPONSE == 1)
  {

    if (sentKey == 0 && SEND_RSA_KEY == 1)
    {
      cout << endl << "Sending encryption key to client... Continuing listening..." << endl;
      send(8084, addr, message.c_str(), 4);
      sentKey = 1; //TOGGLE FLAG
    }

    else
    {
      cout << endl
           << "Enter Message to Send: " << endl;
      getline(cin, message);
      send(8084, addr, message.c_str(), 0);
    }
  }

  return buf;
}

void networker::init(uint16_t port, const char *host, int GenerateKeyPair)
{
  flushBuffers();

  socketfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  memset(&hint, 0, sizeof(hint));
  hint.sin_family = AF_INET;
  hint.sin_port = htons(port);
  inet_pton(AF_INET, host, &hint.sin_addr);

  bind(socketfd, (struct sockaddr *)&hint, sizeof(hint));

  memset(&client, 0, sizeof(client));
  client.sin_family = AF_INET;

  if (GenerateKeyPair == 1)
  {
    publicKey = getPublicKey();
    privateKey = getPrivateKey(publicKey);
    sentKey = 0; //SET FLAG TO ZERO
  }

  receivedKey = 0;
}
