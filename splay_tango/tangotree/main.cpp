#include <iostream>
#include <redblacktango.h>
#include <binarysearchtree.h>
#include <vector>
#include <cmath>
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
    RedBlackTango *_tree = new RedBlackTango();

    int valor;
    void* ptr;
    ptr = &valor;
    _tree->initializePerfectTree(10);

    for(int i=0; i<1000000; i++){
        valor+=i;
        _tree->insert(valor,(int*)(ptr));
    }

    int num;
    srand(time(NULL));

    for (int i=0; i<1000000; i++){
        num = 500 + rand() % (900001 - 500);
        cout<< num <<endl;

        TIME_THIS (_tree->search(num));
    }
}
