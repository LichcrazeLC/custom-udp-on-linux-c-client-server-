#include <iostream>
#include <math.h>
#include <vector>

using namespace std;
// find gcd
int lcm(int a, int b);

vector<long long> extended_euclid_gcd(long long a, long long b);

long long modulo_multiplicative_inverse(long long A, long long M);

double getPublicKey();

double getPrivateKey(double publicKey);

double encrypt(double message, double publicKey);

double decrypt(double message, double privateKey);
