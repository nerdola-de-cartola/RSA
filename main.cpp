#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <bitset> 
#include <stack> 

typedef long long ll;

#define MAX_PRIMES 10000

ll PRIMES[MAX_PRIMES];
ll PRIMES_QTD;
std::bitset<MAX_PRIMES> isPrime;

ll randomPrime();
void sieveOfEratosthenes();
ll totienteDeEuler(ll p, ll q);
ll inverso(ll a, ll n);
ll modularExponential(ll b, ll e, ll m);
ll encryptRSA(ll message, ll e, ll n);

int main(void) {

   sieveOfEratosthenes();
   srand(time(NULL));

   ll p = randomPrime();
   ll q = randomPrime();
   ll n = p * q;
   ll fi = totienteDeEuler(p, q);
   ll e, d;
   ll M;

   do {
      e = rand() % fi;
      d = inverso(e, fi);

   } while(d == -1);

   printf(
      "p: %lld\n"\
      "q: %lld\n"\
      "n: %lld\n"\
      "fi: %lld\n"\
      "e: %lld\n"\
      "d: %lld\n",
      p,
      q,
      n,
      fi,
      e,
      d
   );

   printf("Digite a mensagem desejada: ");
   scanf("%lld", &M);

   ll N = encryptRSA(M, e, n);
   
   printf("Sua mensagem encriptada é: %lld\n", N);

   return 0;
}

ll modularExponential(ll b, ll e, ll m) {

   ll answer = 1;

   while (e > 0) {

      if (e&1)
         answer = (answer * b) % m;

      e = e >> 1;

      b = (b * b) % m;

   }

   return answer % m;

}

ll encryptRSA(ll message, ll e, ll n) {

   return modularExponential(message, e, n);

}

ll inverso(ll a, ll n) {

   ll r = 1;
   std::stack<ll> q;

   ll mod = n;

   while(r != 0) {
      q.push(n / a);
      r = n % a;

      n = a;
      a = r;
   }

   if(n != 1)
      return -1;

   ll s = 0, t = 1, t_antigo;
   q.pop();

   while(q.size() != 0) {
      t_antigo = t;
      t = s - (q.top() * t);
      q.pop();

      s = t_antigo;
   }

   if(t < 0)
      return mod + t;

   return t % mod;

}

ll totienteDeEuler(ll p, ll q) {
   return (p - 1) * (q - 1);
}

void sieveOfEratosthenes() {

   int i, j;

   isPrime.set();

   for(i = 4; i < MAX_PRIMES; i += 2)
      isPrime[i] = 0;

   for(i = 3; (i*i) < MAX_PRIMES; i += 2) {

      if(isPrime[i]) {

         for(j = i + i; j < MAX_PRIMES; j += i)
            isPrime[j] = 0; 

      }

   }

   PRIMES[0] = 2;
   for(i = 3, PRIMES_QTD = 1; i < MAX_PRIMES; i += 2) {
      if(isPrime[i]) {
         PRIMES[PRIMES_QTD] = i;
         PRIMES_QTD++;
      }
   }

}

ll randomPrime() {
   
   ll random_index = rand() % PRIMES_QTD;

   return PRIMES[random_index];

}