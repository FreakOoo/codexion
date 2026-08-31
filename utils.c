#include "codexion.h"

long mytime(void)
{
  struct timeval tv;

  gettimeofday(&tv, NULL);
  return (tv.tv_sec * 1000L + tv.tv_usec/1000);
}

