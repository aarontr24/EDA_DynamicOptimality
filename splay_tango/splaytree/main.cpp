#include "splay.h"
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <ctime>

#define TIME_THIS(X)                                         \
  {                                                          \
    struct timespec ts1, ts2;                                \
    clock_gettime( CLOCK_REALTIME, &ts1 );                   \
    X;                                                       \
    clock_gettime( CLOCK_REALTIME, &ts2 );                   \
    printf( #X " demora: %f\n",                              \
      (float) ( 1.0*(1.0*ts2.tv_nsec - ts1.tv_nsec*1.0)*1e-9 \
      + 1.0*ts2.tv_sec - 1.0*ts1.tv_sec ) );                 \
  }

using namespace std;

int main()
{
    splay* root = NULL;

    //int r;

    for (int i = 0; i<20000000 ; i++){
            //r = rand()%3000000;
            root = Insert(i, root);
            //InOrder(root);

    }
    //std::cout<<"\nInOrder: \n";
    //InOrder(root);

    clock_t start_time;
    clock_t final_time;
    double total_time;
    start_time = clock();

    //int num;
    //srand(time(NULL));

    for (int i = 0; i<2000000 ; i++){

            //num = 5 + rand() % (1900001 - 5);
            //cout<< num <<endl;

            start_time = clock();

            root = Search(i, root);

            final_time = clock();

            //InOrder(root);
            total_time = ((double)(final_time - start_time)) / CLOCKS_PER_SEC;

            //cout<<"Tiempo Ejecucion: "<<total_time<<endl;
    }

    cout<<"Tiempo Ejecucion: "<<total_time<<endl;

    //std::cout<<"\nAfter Search: \n";
    //InOrder(root);

    return 0;
}
