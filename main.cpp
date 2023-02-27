#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <bitset> 
#include <stack> 

#define MAX_PRIMES 10000

typedef long long ll;

ll PRIMES[MAX_PRIMES];
ll PRIMES_QTD;
std::bitset<MAX_PRIMES> isPrime;

ll randomPrime();
void sieveOfEratosthenes();
ll totienteDeEuler(ll p, ll q);
ll inverso(ll a, ll n);
ll modularExponential(ll b, ll e, ll m);
ll TCR(ll *a, ll *m, int size);

struct PrivateKey {
   ll p;
   ll q;
   ll d;
   
   void print(char *name) {
      printf("Private key of %s: (%lld, %lld, %lld)\n", name, p, q, d);
   }
};

struct PublicKey {
   ll n;
   ll e;

   void print(char *name) {
      printf("Public key of %s: (%lld, %lld)\n", name, n, e);
   }
};

struct Keys {
   PrivateKey priv;
   PublicKey pub;

   Keys() {
         
      priv.p = randomPrime();
      priv.q = randomPrime();
      pub.n = priv.p * priv.q;
      
      ll fi = totienteDeEuler(priv.p, priv.q);

      do {
         pub.e = rand() % fi;
         priv.d = inverso(pub.e, fi);

      } while(priv.d == -1);
   }

   ~Keys() {}

   
   void print(char *name) {
      priv.print(name);
      pub.print(name);
   }
};

ll encryptRSA(ll message, PublicKey key);
ll decryptRSA(ll N, PrivateKey key);

int main(void) {

   sieveOfEratosthenes();
   srand(time(NULL));

   Keys Ana;
   Keys Beto;
   ll M;

   Ana.print( (char *) "Ana");
   printf("\n");
   Beto.print( (char *) "Beto");

   printf("Digite a mensagem desejada: ");
   scanf("%lld", &M);

   M = encryptRSA(M, Beto.pub);
   printf("Ana manda para Beto a mensagem criptografada %lld\n", M);

   M = encryptRSA(M, Ana.pub);
   printf("Beto manda de volta para Ana a mensagem duplamente criptografada %lld\n", M);

   M = decryptRSA(M, Ana.priv);
   printf("Ana descriptografa a mensagem e envia de volta para Beto %lld\n", M);

   M = decryptRSA(M, Beto.priv);
   printf("Por fim, Beto descriptografa a mensagem %lld\n", M);


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

ll encryptRSA(ll message, PublicKey key) {

   return modularExponential(message, key.e, key.n);

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

ll decryptRSA(ll N, PrivateKey key) {

   ll a[2];
   ll m[2];
   ll e[2];

   m[0] = key.p;
   m[1] = key.q;

   for(int i = 0; i < 2; i++) {
      e[i] = key.d % (m[i] - 1);
      a[i] = modularExponential(N, e[i], m[i]);
   }

   return TCR(a, m, 2);

}

ll TCR(ll *a, ll *m, int size) {

   int i;
   ll m_total = 1;
   ll result = 0;

   ll M[size];
   ll x[size];
   ll y[size];
   ll Y[size];
   ll r[size];

   for(i = 0; i < size; i++)
      m_total *= m[i];

   for(i = 0; i < size; i++) {
      M[i] = m_total / m[i];
      x[i] = M[i] % m[i];
      y[i] = inverso(x[i], m[i]);
      Y[i] = a[i] * M[i] * y[i];
      r[i] = Y[i] % m_total;
   }

   for(i = 0; i < size; i++)
      result = (result + r[i]) % m_total;

   return result;
}