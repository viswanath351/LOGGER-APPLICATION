#ifndef LOGGER_H
#define LOGGER_H

#include "Header.h"

/*
 PUR: MEMThread is a thread function which calls when thread has been created.
      It has call from the CServer class.
      It is also the friend with CLogger class.
      It handles all the operation of Memory logger
 PRE: Takes the CLogger class object address as pointer variable.
 POS: Calls the Memory logger if successfully initialized.
      Make an entry in xml file and sleeps for the given time period.
*/
    void *MEMThread( void * );

/*
 PUR: CPUThread is a thread function which runs when the thread has been created.
      It has call from the Cserver class.
      It handles all the operation of CPU logger.
 PRE: Takes the CLogger class object address as pointer variable.
 POS: Calls the CPU logger if successfully initialized
      Make an entry in xml file and sleeps for the desired time frame.  
*/
    void* CPUThread( void * );

/*
 Class Name    : CLogger
 Created By    : Viswanath Gupta
 Created Date  : 19-June-2017
 Version       : 1.0
 Purpose       : This is used to initialize the logger and makes an entry in the xml file.
*/
class CLogger {

    private:

     std::string      m_strFileInitial;				//initial of the file name, in our case "log"
     char             m_szBuf[BUFSIZ];			
     time_t           m_RawTime;					
     tm             * m_pTimeInfo;
     const char     * m_const_pcFilePath;			//stores the  complete file path name
     int              m_nSleepTime;				    //no. of seconds we want our thread to sleep
     int              m_nFileDescriptor;			//file descriptor to do some action on the xml file		
     pthread_mutex_t  m_MutexLock;				    //lock to provide synchronization
     pthread_mutex_t  m_WriteLock;				    //lock to provide special synchronization to write process in 														 xmlfile

   public:
    CLogger();							            //default constructor
   ~CLogger(){}							            //default destructor

/*
 PUR: It initializes the logger by opening the xml file.
 PRE: Takes reference variable.
 POS: Returns the current working file name in reference varaible.
      Returns -1 on error.
      Returns 0 if successfully initialized the logger.  
*/
    int Initializer(std::string & );				

/*
 PUR: It handles all the activities related to Memory logger.
 PRE: File must be open to write Memory entry in file.
 POS: If any error occurs then calls ERROR macro defined in Header.h
      Else, make an entry in the xml file.
*/
    void MEMLogger();
/*
 PUR: It handles all the activities related to CPU logger.
 PRE: File must be opened to write CPU entry in file.
 POS: If any error occurs then calls ERROR macro defined in Header.h
      Or else, make an entry in the .xml file.
*/
    void CPULogger();

/*
 PUR: It adds the last xml file tag and closes the file.
 PRE: File must be already opened. 
 POS: Adds last xml tag in the opened file.
      If some error occured, calls the ERROR macro defined in Header.h
*/
    void End();

/*
 PUR: It works on Memory part synchronously.
 PRE: Takes the address of the CLogger object.
 POS: Provides synchronization.
      Calls MEMLogger class method to make an entry in the file.
      Sleeps for the desired time period. 
*/
    friend void * MEMThread(void * );

/*
 PUR: It works on CPU part synchronously.
 PRE: Takes the address of CLogger object.
 POS: Provides synchronization.
      Calls CPULogger method to make an entry in the file.
      Sleeps for the desired time period.
*/
    friend void * CPUThread(void * );
};

#endif //~LOGGER_LOG_H
