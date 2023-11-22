#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <cstring>
/*
unsigned long long silnia(unsigned long long n){
    if(n == 1){
        return 1;
    }
    else{
        return n * silnia(n-1);
    }

}

void fib(long long n){
    long long a = 1;
    long long b = 1;
    long long swap = 0;
    if(n==1){
        printf("%ld", 1);
    }else if(n==2){
        printf("%ld %ld ", 1, 1);
    }else{
         printf("%ld %ld ", 1, 1);
        for(long long i=0;i<n;i++){
            a = a+b;
            swap = b;
            b = a;
            a = swap;
            printf("%ld ",b);
        }
    }
}
*/

/*
double modul(double x, double y, double z){
    return sqrt(x*x + y*y + z*z);
}
*/

int main(){
    /*long long a;

    scanf("%ld", &a);
    printf("Liczba: %d, silnia: %d", a, silnia(a));

    long long n;
    scanf("%ld", &n);
    fib(n);*/

    /*
    double a = 1.0, b = 1.0, c = 1.0;

    scanf("%lf%lf%lf", &a, &b, &c);
    double delta = b * b - 4 * a * c;

    if(delta < 0.0 ){
        printf("brak rozwiazan w R");
    }else if(delta == 0.0 ){
        printf("jedno rozwiazanei w r: %lf", ((-1.0) * b) / (2 * a));
    }
    else{
         printf("2 rozwiazania w r: %lf, %lf", ((-1.0) * b + delta) / (2 * a), ((-1.0) * b - delta) / (2 * a));
    }
    */
   /*
   double a = 0.0, r = 0.0;
   int n=0;
   scanf("%lf%lf%d", &a, &r, &n);
   double suma = 0.0;
   for(int i=0;i<n;i++){    
        suma += a;
        a+=r;
   }
    printf("%lf", suma);
    */
   /*
   double a = 0.0, q = 0.0;
   int n=0;
   scanf("%lf%lf%d", &a, &q, &n);
   double suma = 0.0;
   for(int i=0;i<n;i++){    
        suma += a;
        a*=q;
   }
    printf("%lf", suma);
    */

   /*
   srand(time(NULL));

    double x1 = rand() % 1001;
    double x2 = rand() % 1001;
    double r1 = rand() % 501;
    
    double y1 = rand() % 1001;
    double y2 = rand() % 1001;
    double r2 = rand() % 501;

    double dist = sqrt((x1-x2) * (x1-x2) + (y1-y2) * (y1-y2));

    if(dist < r1 + r2){
        printf("2 pkt wspolne");
    }else if(r1 + r2 == dist){
         printf("1 pkt wspolny");
    }
    else{
         printf("0 pkt wspolnych");
    }
    */

    /*
    srand(time(NULL));
    for(int i=0;i<10;i++){
        double a = (double)rand()/RAND_MAX * 10.0 - 5.0;
        double b = (double)rand()/RAND_MAX * 10.0 - 5.0;
        double c = (double)rand()/RAND_MAX * 10.0 - 5.0;
        printf("%lf \n", modul(a, b ,c));
    }
    */


    srand(time(NULL));

    int t[55];
    for(int i=0;i<55;i++){
        t[i]=0;
    }
    int a=0, b=0, c=0, d=0, e=0, f=0;
    scanf("%d%d%d%d%d%d", &a, &b, &c, &d, &e, &f);
    t[a]=1;
    t[b]=1;
    t[c]=1;
    t[d]=1;
    t[e]=1;
    t[f]=1;

    long long wynik[7];
    for(int i=0;i<7;i++){
        wynik[i]=0;
    }

    for(long long j=0;j<1000000;j++){
        int correct = 0;

        for(int i=0;i<6;i++){
            int temp = rand() % 49 + 1;
            if(t[temp] == 1){
                correct++;
            }
        }
        wynik[correct]++;
        correct = 0;
    }

    for(int i=0;i<7;i++){
        printf("%d, %d, %lf \n", i, wynik[i], (double)wynik[i]/10000000.0);
    }


    return 0;
}