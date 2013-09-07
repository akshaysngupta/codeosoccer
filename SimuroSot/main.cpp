#include "stdafx.h"
//#include <signal.h>
//#include <cstdio>
//#include <cstdlib>
//#include <ctime>
//
//#include <iostream>
//#pragma comment(lib, "ws2_32.lib")
//#include "./Core/tester.hpp"
//#include "./Core/worker.hpp"
//#include "./Core/paramLoaders.h"
//#include "./Core/beliefState.h"
//#include "./Utils/comdef.h"
//#include "./Tactics/robot.h"
//#include <WinSock2.h>
//using namespace std;
//using namespace MyStrategy;
//
//// logger is to be used for logging throughout the project. Use it in other files using extern
//
///* This variable decides if the application should continue running or not.
// * It is true by default and becomes false by pressing Ctrl+C during the runtime.
// */
//static bool run = true;
//const bool test = true;
//const bool qtdebug = true;
//// Define the function to be called when Ctrl+C (SIGINT) signal is sent to process
//void signal_callback_handler(int signum)
//{
//  NOT_USED(signum);
//  run = false;
//}
//
//
///* This function performs some sanity checks required for the project
// * to run properly besides other initializations.
// * It is executed before main()
// */
//void main_constructor(void)
//{
//  srand(time(NULL));
//  // Register signal and signal handler
//  signal(SIGINT, signal_callback_handler);
//}
//int kbhit(void)
//{
//  struct timeval tv;
//  fd_set read_fd;
//  /* Do not wait at all, not even a microsecond */
//  tv.tv_sec=0;
//  tv.tv_usec=0;
//  /* Must be done first to initialize read_fd */
//  FD_ZERO(&read_fd);
//  /* Makes select() ask if input is ready:
//  * 0 is the file descriptor for stdin */
//  FD_SET(0,&read_fd);
//  /* The first parameter is the number of the
//  * largest file descriptor totextEdit = new QPlainTextEdit;
//    setCentralWidget(textEdit); check + 1. */
//  if(select(1, &read_fd, NULL, /*No writes*/NULL, /*No exceptions*/&tv) == -1)
//    return 0; /* An error occured */
//  /* read_fd now holds a bit map of files that are
//  * readable. We test the entry for the standard
//  * input (file 0). */
//  if(FD_ISSET(0,&read_fd))
//    /* Character pending on stdin */
//    return 1;
//  /* no characters were pending */
//  return 0;
//}
//
bool FIELD_IS_INVERTED = false;
//
//
//int startgame(bool usePositiveXSide, Simulator::TeamColor color) {
//  // Set if the side used by us.
//  FIELD_IS_INVERTED = usePositiveXSide;
//  // Set the Team color and id so that the simulator and MyStrategy code are in coherence.
//  switch(color) {
//    case Simulator::BLUE_TEAM:
//      MyStrategy::HomeTeam::COLOR = Simulator::BLUE_TEAM;
//      MyStrategy::AwayTeam::COLOR = Simulator::YELLOW_TEAM;
//      Simulator::BlueTeam::ID   = MyStrategy::HOME_TEAM;
//      Simulator::YellowTeam::ID = MyStrategy::AWAY_TEAM;
//    break;
//    case Simulator::YELLOW_TEAM:
//      MyStrategy::HomeTeam::COLOR = Simulator::YELLOW_TEAM;
//      MyStrategy::AwayTeam::COLOR = Simulator::BLUE_TEAM;
//      Simulator::BlueTeam::ID   = MyStrategy::AWAY_TEAM;
//      Simulator::YellowTeam::ID = MyStrategy::HOME_TEAM;
//    break;
//  }
//  // Loading all the parameters controlled by Databox. Make sure the databox file is correct one.
//  DataBox::loadData();
//  ParamLoaders pl;
//  pl.load_all_params();
//  
//  MyStrategy::BeliefState state;
//  Util::CS writer_mutex;
//  Util::CS writer_preference;
//  MyStrategy::Robot::writer_mutex      = &writer_mutex;
//  MyStrategy::Robot::writer_preference = &writer_preference;
//  if(test) 
//  {
//    Tester t(run, state);
//
//      t.run();
//   
//  } 
//  else 
//  {
//    Worker t(run, state, &writer_mutex, &writer_preference);
//
//      t.run();
//    
//  }
//  DataBox::saveData();
//  return 0;
//}
//
//int main(int argc, char* argv[])
//{
// 
//  return startgame(false, Simulator::BLUE_TEAM);
//} // main
