//defined all header file, constats and macros used in the task

#define GNU_SOURE

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <string.h>
#include <errno.h>

#include <iostream>
#include <string>
#include <fstream>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <fcntl.h>

#include <pthread.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

//CONSTANTS
 #define FAIL -1
 #define SUCCESS 0
 
 #define TIME 3
 #define PORT 50000
 #define ADDR "127.0.0.1"
 #define BACKLOG 5
 #define LEN 50
 #define ENCRYPT 10
 #define DECRYPT -10

#ifndef BUFSIZ
 #define BUFSIZ 8192
#endif

//MACRO
 #define ERROR( funcname, line, errnum ) do{  \
   fprintf( stderr, "Func: %s. Line: %d. Errtype: %s.\n", funcname, line, strerror(errnum) );  \
  exit( EXIT_FAILURE );  \
  }while(0)




