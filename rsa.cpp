#include <iostream>
#include <math.h>
#include <vector>

#include "rsa.h"

using namespace std;
// find gcd
int lcm(int a, int b)
{
  int lcmvl;
  if (a > b)
    lcmvl = a;
  else
    lcmvl = b;
  while (1)
  {
    if (lcmvl % a == 0 && lcmvl % b == 0)
    {
      //cout << "The LCM of " << a << " and " << b << " is " << lcmvl << endl;
      return lcmvl;
      break;
    }
    lcmvl++;
  }
  return 0;
}

vector<long long> extended_euclid_gcd(long long a, long long b) {
    // Returns a vector `result` of size 3 where:
    // Referring to the equation ax + by = gcd(a, b)
    //     result[0] is gcd(a, b)
    //     result[1] is x
    //     result[2] is y 
    
    long long s = 0; long long old_s = 1;
    long long t = 1; long long old_t = 0;
    long long r = b; long long old_r = a;

    while(r != 0) {
        long long quotient = old_r/r;
        // We are overriding the value of r, before that we store it's current
        // value in temp variable, later we assign it to old_r
        long long temp = r;
        r = old_r - quotient*r;
        old_r = temp;

        // We treat s and t in the same manner we treated r
        temp = s;
        s = old_s - quotient*s;
        old_s = temp;

        temp = t;
        t = old_t - quotient*t;
        old_t = temp;
    }
    vector<long long> result;
    result.push_back(old_r);
    result.push_back(old_s);
    result.push_back(old_t);
    return result;
}

long long modulo_multiplicative_inverse(long long A, long long M) {
    // Assumes that A and M are co-prime
    // Returns multiplicative modulo inverse of A under M
    
    // Find gcd using Extended Euclid's Algorithm
    vector<long long> v = extended_euclid_gcd(A, M);
    long long gcd = v[0];
    long long x = v[1];
    long long y = v[2]; // We don't really need this though

    // In case x is negative, we handle it by adding extra M
    // Because we know that multiplicative inverse of A in range M lies
    // in the range [0, M-1]
    if(x < 0) {
        x += M;
    }
    
    return x;
}

double getPublicKey()
{
  double p = 13;
  double q = 17;
  double n = p * q; //calculate n
  double track;
  double lcmValue = lcm(p - 1, q - 1); //calculate phi
  //public key
  double publicKey = 7;
  //for checking that 1 < e < phi(n) and gcd(e, phi(n)) = 1; i.e., e and phi(n) are coprime.

  return publicKey;
}

double getPrivateKey(double publicKey)
{
  double p = 13;
  double q = 17;
  double n = p * q; //calculate n
  //double track;
  double track;

  double d = modulo_multiplicative_inverse(publicKey, lcm(p - 1, q - 1));

  return d;
}

double encrypt(double message, double publicKey)
{
  //2 random prime numbers
  double p = 13;
  double q = 17;
  double n = p * q; //calculate n
  //private key
  //d stands for decrypt
  //choosing d such that it satisfies d*e = 1 mod phi
  double c = pow(message, publicKey); //encrypt the message
  c =  fmod(c,n);

  return c;
}

double decrypt(double message, double privateKey)
{

  double p = 13;
  double q = 17;
  double n = p * q;

  double m = (double) pow(message, privateKey);

  m =  fmod(m,n);

  return m;
}

// int main()
// {
//   double myPublicKey = getPublicKey();
//   double privateKey = getPrivateKey(myPublicKey);
//   cout << "Public Key is: " << myPublicKey << endl;
//   cout << "Private Key is: " << privateKey << endl;
//   cout << "Encrypted: " << encrypt(119, myPublicKey) << endl;
//   cout << "Decrypted: " << decrypt( encrypt(119, myPublicKey), privateKey) << endl;
// }

