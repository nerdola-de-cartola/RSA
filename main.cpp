#include <iostream>
#include <cstdlib>
#include <ctime>
#include <bitset> 
#include <stack> 
#include <boost/multiprecision/cpp_int.hpp>

#define MAX_PRIMES 100000

typedef boost::multiprecision::int1024_t i_1024;
typedef long long ll;

ll PRIMES[MAX_PRIMES];
ll PRIMES_QTD;
std::bitset<MAX_PRIMES> isPrime;

ll randomPrime();
void sieveOfEratosthenes();
i_1024 totienteDeEuler(i_1024 p, i_1024 q);
i_1024 inverso(i_1024 a, i_1024 n);
i_1024 modularExponential(i_1024 b, i_1024 e, i_1024 m);
i_1024 TCR(i_1024 *a, i_1024 *m, int size);

struct PrivateKey {
   i_1024 p;
   i_1024 q;
   i_1024 d;
   
   void print(char *name) {
      std::cout << "Private key of " << name << ": (" << p << ", " << q << ", " << d << ")\n";
   }
};

struct PublicKey {
   i_1024 n;
   i_1024 e;

   void print(char *name) {
      std::cout << "Public key of " << name << ": (" << n << ", " << e << ")\n";
   }
};

struct Keys {
   PrivateKey priv;
   PublicKey pub;

   Keys() {
         
      priv.p = randomPrime();
      priv.q = randomPrime();
      pub.n = priv.p * priv.q;
      
      i_1024 fi = totienteDeEuler(priv.p, priv.q);

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

i_1024 encryptRSA(i_1024 message, PublicKey key);
i_1024 decryptRSA(i_1024 N, PrivateKey key);

int main(void) {

   sieveOfEratosthenes();
   srand(time(NULL));

   Keys Ana;
   Keys Beto;
   i_1024 M;

   Ana.print( (char *) "Ana");
   printf("\n");
   Beto.print( (char *) "Beto");

   std::cout << "Digite a mensagem desejada: ";
   std::cin >> M;

   M = encryptRSA(M, Beto.pub);
   std::cout << "Ana manda para Beto a mensagem criptografada " <<  M << '\n';

   M = encryptRSA(M, Ana.pub);
   std::cout << "Beto manda de volta para Ana a mensagem duplamente criptografada " << M << '\n';

   M = decryptRSA(M, Ana.priv);
   std::cout << "Ana descriptografa a mensagem e envia de volta para Beto " << M << '\n';

   M = decryptRSA(M, Beto.priv);
   std::cout << "Por fim, Beto descriptografa a mensagem " << M << '\n';

   return 0;
}

i_1024 modularExponential(i_1024 b, i_1024 e, i_1024 m) {

   i_1024 answer = 1;

   while (e > 0) {

      if (e&1)
         answer = (answer * b) % m;

      e = e >> 1;

      b = (b * b) % m;

   }

   return answer % m;

}

i_1024 encryptRSA(i_1024 message, PublicKey key) {

   return modularExponential(message, key.e, key.n);

}

i_1024 inverso(i_1024 a, i_1024 n) {

   i_1024 r = 1;
   std::stack<i_1024> q;

   i_1024 mod = n;

   while(r != 0) {
      q.push(n / a);
      r = n % a;

      n = a;
      a = r;
   }

   if(n != 1)
      return -1;

   i_1024 s = 0, t = 1, t_antigo;
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

i_1024 totienteDeEuler(i_1024 p, i_1024 q) {
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

i_1024 decryptRSA(i_1024 N, PrivateKey key) {

   i_1024 a[2];
   i_1024 m[2];
   i_1024 e[2];

   m[0] = key.p;
   m[1] = key.q;

   for(int i = 0; i < 2; i++) {
      e[i] = key.d % (m[i] - 1);
      a[i] = modularExponential(N, e[i], m[i]);
   }

   return TCR(a, m, 2);

}

i_1024 TCR(i_1024 *a, i_1024 *m, int size) {

   int i;
   i_1024 m_total = 1;
   i_1024 result = 0;

   i_1024 M[size];
   i_1024 x[size];
   i_1024 y[size];
   i_1024 Y[size];
   i_1024 r[size];

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