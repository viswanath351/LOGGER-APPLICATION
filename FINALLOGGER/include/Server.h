#ifndef SERVER_H
#define SERVER_H

#include "Header.h"
#include "Logger.h"
#include "EncDec.h"

/*
 Class Name   : CServer
 Created By   : Viswanath Gupta
 Created Date : 19-June-2017
 Version      : 1.0
 Purpose      : Handles all operation related to Logger, Encryption, Decryption and Client. 
*/
class CServer {

    private:
     int                   m_nServerFileDescriptor;
     int                   m_nNewSocketFileDescriptor;
     struct sockaddr_in    m_stSocketInfo;
     char                  m_szBuf[1024];
     pthread_t             m_arr_Thread[2];
     std::string           m_strCurrentWorkingFileName;
     int                   m_nCheckCPULog;
     int                   m_nCheckMEMLog;
     int                   m_nCheckLogClose;
     int                   m_nRepeat;
     char                  m_szSuccesEncryptMsg[LEN];
     char                  m_szSuccesDecryptMsg[LEN];
     char                  m_szShutdownMsg[LEN];
     char                  m_szCPULogStartMsg[LEN];
     char                  m_szCPULogStopMsg[LEN];
     char		   m_szMEMLogStartMsg[LEN];
     char                  m_szMEMLogStopMsg[LEN]; 

//class objects
     CLogger               OCLogger;
     CEncDec               OCEncDec;




   public:
     CServer();			//default constructor
    ~CServer(){}		//default destructor

/*
 PUR: Socket handles all the connection from client.
      Takes appropriate action accordingly.
 PRE: Bind socket with the appropriate address.
      Open up port to accept connection from any client.
      Takes appropriate action as per the user send commands.
      Accepted commands are START_MEM_LOG, STOP_MEM_LOG, START_CPU_LOG, STOP_CPU_LOG
      ENCRYPT, DECRYPT, SHUTDOWN.
      Infinite loop, if user type anything wrong, nothing will happen. 
 POS: No emplicit returs defined but if some error occured then it calls ERROR macro defined inside Header.h
*/
    void Socket();

/*
 PUR: CheckCase checks all the test cases.
 PRE: Takes reference varaible as an argument.
 POS: Sets the value to the initial argument.  
*/
    void CheckCase(int & );
};
#endif //LOGGER_SOCKET_H
