#include <iostream>

#include "networker.h"
using namespace std;

int main()
{
  cout << "Server Started!" << endl;

  networker protocol_worker;

  protocol_worker.init(8083, "0.0.0.0", 1);

  while (true)
  {
    protocol_worker.listen(1, 1);
  }
}