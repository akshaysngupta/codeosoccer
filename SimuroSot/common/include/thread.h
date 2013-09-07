// This filec contains the Thread class

#ifndef THREAD_H
#define THREAD_H

#ifdef WIN32
# include <winsock2.h>
#else
# include <pthread.h>
#endif

namespace Util
{
  /* class Thread
   * Represents a thread of execution
   * in the process. To create a new Thread
   * write an inherited class of Thread and
   * override the run() function
   */
  class Thread
  {
  private:
#ifdef WIN32
    HANDLE              handle;
    static DWORD WINAPI threadFunc(LPVOID pv);
#else
    pthread_t    handle;
    static void* threadFunc(void* arg);
#endif // !WIN32

  public:
    Thread();

    ~Thread();

    /* start(int flags)
     * creates a low-level thread object and calls the run() function
     */
    void start();

    virtual void run() = 0;

    /* sleep(long ms)
     * holds back the thread's execution for
     * "ms" milliseconds
     */
    void sleep(long ms);

    /* stop()
     * stops the running thread and frees the thread handle
     */
    void stop();

#ifdef WIN32
    /* setPriority(int tp)
     * sets the priority of the thread to "tp"
     * "tp" must be a valid priority defined in the
     * Thread class
     */
    void setPriority(int tp);

    /* suspend()
     * suspends the thread
     */
    void suspend();

    /* resume()
     * resumes a suspended thread
     */
    void resume();
#endif // WIN32
  }; // class Thread
} // namespace Util

#endif // THREAD_H
