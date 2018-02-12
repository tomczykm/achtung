#include "inc/Global.h"

extern int RandomInt( int min, int max ) {
  if ( min > max ) return max;
  return min + ( std::rand() % ( max - min + 1 ) );
}
