#pragma once 
#ifndef TIMER_H_INL
#define TIMER_H_INL
//#include <stdafx.h>
#include <time.h>
#include <cassert>
//#include "stdafx.h"
#include <WinSock2.h>
namespace Util
{
	  #if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif
 
struct timezone 
{
  int  tz_minuteswest; /* minutes W of Greenwich */
  int  tz_dsttime;     /* type of dst correction */
};
 

  class Timer
  {
	
  private:
    struct timeval timeStart, timeSplit, timeStop;
    int intervalms_, intervalus_;
    bool timerStarted;
	 public:
	static int gettimeofday(struct timeval *tv, struct timezone *tz)
{
  FILETIME ft;
  unsigned __int64 tmpres = 0;
  static int tzflag;
 
  if (NULL != tv)
  {
    GetSystemTimeAsFileTime(&ft);
 
    tmpres |= ft.dwHighDateTime;
    tmpres <<= 32;
    tmpres |= ft.dwLowDateTime;
 
    /*converting file time to unix epoch*/
    tmpres -= DELTA_EPOCH_IN_MICROSECS; 
    tmpres /= 10;  /*convert into microseconds*/
    tv->tv_sec = (long)(tmpres / 1000000UL);
    tv->tv_usec = (long)(tmpres % 1000000UL);
  }
 
  if (NULL != tz)
  {
    if (!tzflag)
    {
      _tzset();
      tzflag++;
    }
    tz->tz_minuteswest = _timezone / 60;
    tz->tz_dsttime = _daylight;
  }
 
  return 0;
}
 
    Timer() :
      timerStarted(false)
    { }

    ~Timer()
    { }

    inline void start(void)
    {
      int ret = gettimeofday(&timeStart, NULL);
	
      assert(ret == 0);
      timerStarted = true;
    }

    inline int split(void)
    {
      assert(timerStarted == true);
      int ret = gettimeofday(&timeSplit, NULL);
      assert(ret == 0);
      return ((timeSplit.tv_sec - timeStart.tv_sec) * 1000 + (timeSplit.tv_usec - timeStart.tv_usec) / 1000);
    }

    inline int stopms(void)
    {
      assert(timerStarted == true);
      int ret = gettimeofday(&timeStop, NULL);
      assert(ret == 0);
      timerStarted = false;
      return (intervalms_ = (timeStop.tv_sec - timeStart.tv_sec) * 1000 + (timeStop.tv_usec - timeStart.tv_usec) / 1000);
    }

    inline int stopus(void)
    {
      assert(timerStarted == true);
      int ret = gettimeofday(&timeStop, NULL);
      assert(ret == 0);
      timerStarted = false;
      return (intervalus_ = (timeStop.tv_sec - timeStart.tv_sec) * 1000000 + (timeStop.tv_usec - timeStart.tv_usec));
    }
    
    inline int intervalms(void) const
    {
      return intervalms_;
    }
    
    inline int intervalus(void) const
    {
      return intervalus_;
    }
  };
}

#endif // TIMER_H_INL
