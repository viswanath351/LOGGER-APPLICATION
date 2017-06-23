#include "Logger.h"

/*
 PUR: ParseLine helps in parsing the data we want from the file.
 PRE: Takes the string as an argument.
 POS: Parse all the numbers from the string.
      Returns that number 
*/
static int ParseLine( char * );

//---------------------------------------------------------------------------------------------------------------------------------------------------------------//
//default constructor to initialize default values to class data members
 CLogger::CLogger(){

  m_strFileInitial   = "log";
  m_RawTime          = 0;
  m_pTimeInfo        = NULL;
  m_const_pcFilePath = NULL; 
  m_nSleepTime       = 0;
  m_nFileDescriptor  = 0;
 
  m_MutexLock        = PTHREAD_MUTEX_INITIALIZER;
  m_WriteLock        = PTHREAD_MUTEX_INITIALIZER; 

  memset( m_szBuf, 0, BUFSIZ);

}// end

//---------------------------------------------------------------------------------------------------------------------------------------------------------------//
/*
 Method Name : CLogger::MEMLogger
 Date        : 19-June-2017
 Purpose     : Opens up the file from the proc directory.
               Fetch out all the memory details we require from opened file.
               Closes the file to see new details if we require to open this file again.
               Calls ParseLine to get all the numbers from the string.
               Fetch out system current time and date.
               If everything happens successfully writes the data in file.  
 Input       : File must be open to perform write action.
 Output      : On success, it makes an entry to the opened xml file. 
 Return      : No explicit returns, but if some error occured it will call the ERROR macro defined inside Header.h
*/
void CLogger::MEMLogger(){

    char         szLine[128];
    int          nTotal;
    int          nFree;
    int          nStatus;
    char         szBuf[100];
    FILE       * pFilePath;
    const char * const_FuncName = "CLogger::MEMLogger()"; 

    errno =0;							//setting errno variable to default

    if( (pFilePath= fopen("/proc/meminfo","r")) ==NULL )
     ERROR( const_FuncName, __LINE__, errno ); 

    while(fgets(szLine, sizeof(szLine), pFilePath) !=NULL){

     if(strncmp(szLine,"MemTotal:",6) == 0){
      nTotal = ParseLine(szLine);
       break;
     }
    }
     while(fgets(szLine, sizeof(szLine), pFilePath) !=NULL){

      if(strncmp(szLine, "MemFree:", 6) == 0){
       nFree = ParseLine(szLine);
        break;
      }
     }

   if( fclose(pFilePath) !=SUCCESS ){
    ERROR( const_FuncName, __LINE__, errno ); 
   }

   time(&m_RawTime);
   m_pTimeInfo = localtime(&m_RawTime);
   strftime(szBuf,sizeof(szBuf),"%d-%m-%Y  %I-%M-%S",m_pTimeInfo);

   snprintf( m_szBuf, BUFSIZ, "<RAM date=\"%s\" used=\"%d\" nFree=\"%d\"/>\n", szBuf, (nTotal-nFree), nFree );

   if( (nStatus = pthread_mutex_lock( &m_WriteLock )) !=SUCCESS ){
    ERROR( const_FuncName, __LINE__, nStatus );
   }

    if( (write( m_nFileDescriptor, m_szBuf, strlen(m_szBuf) )) ==FAIL ){
     ERROR( const_FuncName, __LINE__, errno );
    }

   if( (nStatus = pthread_mutex_unlock( &m_WriteLock )) !=SUCCESS ){
    ERROR( const_FuncName, __LINE__, nStatus );
   }            
}//end

//---------------------------------------------------------------------------------------------------------------------------------------------------------------//
/*
 Method Name  : CLogger::CPULogger
 Date         : 19-June-2017
 Purpose      : Opens up the file from the proc/ directory.
                Fetch out all the CPU details we require.
                Stores those details in static variables for later use.
                Closes the file to get new data if we require to read in again. 
                Fetch out the current system date and time.
                Writes all the details in the xml file.
 Input        : File must be already opened to perform write operations.
 Output       : If success then makes an entry in the xml file.
 Return       : No explicit returns defined, if some error occured then it calls 
                ERROR macro defined inside the Header.h file.
*/

void CLogger::CPULogger(){

    static unsigned long long int stat_ullnLastTotalUser;
    static unsigned long long int stat_ullnLastTotalUserLow;
    static unsigned long long int stat_ullnLastTotalSys;
    static unsigned long long int stat_ullnLastTotalIdle;

    unsigned long long int        ullnTotalUser;
    unsigned long long int        ullnTotalUserLow;
    unsigned long long int        ullnTotalSys;
    unsigned long long int        ullnTotalIdle;
    unsigned long long int        nTotal;

    const char * const_FuncName = "CLogger::CPULogger()";
    double       dPercent;
    char         szBuf[300];
    FILE       * pFilePath;
    int          nStatus;
    int          nWriteData;
  
   errno =0;						//setting errno to its default value

   if( (pFilePath = fopen("/proc/stat", "r")) ==NULL ){
    ERROR( const_FuncName, __LINE__, errno );
   }

     fscanf(pFilePath, "cpu %llu %llu %llu %llu", &ullnTotalUser, &ullnTotalUserLow, &ullnTotalSys, &ullnTotalIdle);
       
   if( fclose(pFilePath) !=SUCCESS ){
    ERROR( const_FuncName, __LINE__, errno ); 
   }

   if( (ullnTotalUser < stat_ullnLastTotalUser) || (ullnTotalUserLow < stat_ullnLastTotalUserLow) || 
        (ullnTotalSys < stat_ullnLastTotalSys) || (ullnTotalIdle < stat_ullnLastTotalIdle) ){
     dPercent = -1.0;
   }
   else{
     nTotal = (ullnTotalUser - stat_ullnLastTotalUser) + (ullnTotalUserLow - stat_ullnLastTotalUserLow) + (ullnTotalSys - stat_ullnLastTotalSys);
     dPercent = nTotal;
     nTotal += (ullnTotalIdle - stat_ullnLastTotalIdle);
     dPercent /= nTotal;
     dPercent *= 100;
   }

     stat_ullnLastTotalUser = ullnTotalUser;
     stat_ullnLastTotalUserLow = ullnTotalUserLow;
     stat_ullnLastTotalSys = ullnTotalSys;
     stat_ullnLastTotalIdle = ullnTotalIdle;

      time(&m_RawTime);
      m_pTimeInfo = localtime(&m_RawTime);
      strftime(szBuf,sizeof(szBuf),"%d-%m-%Y  %I-%M-%S",m_pTimeInfo);

      snprintf( m_szBuf, BUFSIZ,"<CPU date=\"%s\" used=\"%lf\" nFree=\"%lf\" />\n", szBuf, dPercent, (100-dPercent));
   
      if( (nStatus = pthread_mutex_lock( &m_WriteLock )) !=SUCCESS ){
       ERROR( const_FuncName, __LINE__, nStatus );
      }

       if( (nWriteData = write( m_nFileDescriptor, m_szBuf, strlen(m_szBuf))) ==FAIL ){
        ERROR( const_FuncName, __LINE__, errno );
       }

      if( (nStatus = pthread_mutex_unlock( &m_WriteLock )) !=SUCCESS ){
       ERROR( const_FuncName, __LINE__, nStatus );
      }
      
}// end

//---------------------------------------------------------------------------------------------------------------------------------------------------------------//
/*
 Method Name  : CLogger::Initializer(std::string & )
 Date         : 19-June-2017
 Purpose      : It initializes the logger to perform operations on logger.
                Created new file for the current date on which client has started the logger.
                If file is already there then truncate all the previous data from file.
                Adds up the XML header to the file.
                Keeps it open for other operations. 
 Input        : Takes the r_strFilePath as the method argument.
 Output       : Opens up the logger file for performing the write operations on it. 
 Return       : Returns the current file name without adding the .xml extension.
              : If succesfully opens the xml file then returns 0. 
                Else return -1 
                If some other error occurs then calls ERROR macro defined in Header.h file.
*/
int CLogger::Initializer(std::string & r_strFilePath){

   int nHead  =0;
   int nStatus;
      
   errno =0;

   time(&m_RawTime);
   m_pTimeInfo = localtime(&m_RawTime);
   strftime(m_szBuf, BUFSIZ,"%d-%m-%Y",m_pTimeInfo);

   std::string strFileName(m_szBuf);
   std::string strNewFileName(m_szBuf);

   strFileName = "../FILE/"+ m_strFileInitial +"_" + strFileName + ".xml";
   r_strFilePath = m_strFileInitial +"_" + strNewFileName;

   m_const_pcFilePath = strFileName.c_str();

    if( (m_nFileDescriptor =open( m_const_pcFilePath, O_CREAT| O_EXCL| O_RDWR|O_CLOEXEC, S_IROTH| S_IRGRP| S_IRWXU )) ==FAIL ){

      if( (m_nFileDescriptor = open( m_const_pcFilePath, O_RDWR| O_CLOEXEC| O_TRUNC )) ==FAIL ){
       return -1;      
      }
    }
        
    char szTopHeader[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<usage_log>\n";
  
     if( (nStatus = pthread_mutex_lock( &m_WriteLock )) !=SUCCESS ){
      ERROR( "CLogger::logger()", __LINE__, nStatus );
     }
                  
       if( write( m_nFileDescriptor, szTopHeader, strlen( szTopHeader )) ==FAIL ){
        return -1;
       }

     if( (nStatus = pthread_mutex_unlock( &m_WriteLock )) !=SUCCESS ){
      ERROR( "CLogger::logger()", __LINE__, errno );
     }

 return 0;
}// end

//--------------------------------------------------------------------------------------------------------------------------------------------------------------//
/*
 Method Name : CLogger::End
 Date        : 19-June-2017
 Purpose     : Adds the file xml last header in the file.
               Closes the file.
 Input       : File must be already opened to perform write operations on it.
 Output      : Writes on to the file and closes the file for further use.
 Return      : No explicit returns defined, but if some error occured then calls
               ERROR macro defined inside the Header.h file.
*/
void CLogger::End() {

   const char const_EndHeader[] = "</usage_log>\n\n";
   int nStatus;

   errno =0;

    if( (nStatus = pthread_mutex_lock( &m_MutexLock )) ==FAIL ){
     ERROR( "CLogger::End()", __LINE__, nStatus ); 
    }

    if( (nStatus = pthread_mutex_lock( &m_WriteLock )) ==FAIL ){
     ERROR( "CLogger::End()", __LINE__, nStatus );
    }

     if( (lseek( m_nFileDescriptor, 0, SEEK_END )) ==FAIL ){
      ERROR( "CLogger::End()", __LINE__, errno ); 
     } 

     if( write( m_nFileDescriptor, const_EndHeader, strlen( const_EndHeader )) ==FAIL ){
      ERROR( "CLogger::End()", __LINE__, errno );
     }

    if( (nStatus = pthread_mutex_unlock( &m_WriteLock )) ==FAIL ){
     ERROR( "CLogger::End()", __LINE__, nStatus );
    }

    if( (nStatus = pthread_mutex_unlock( &m_MutexLock )) ==FAIL ){
     ERROR( "CLogger::End()", __LINE__, nStatus );
    }
 
}// end

//---------------------------------------------------------------------------------------------------------------------------------------------------------------//
/*
 Method Name : MEMThread( void * )
 Date        : 19-June-2017
 Purpose     : MEMThread is a friend function with CLogger class.
               Directly calls the CLogger::MEMLogger class method
               to perform Memory related operations.
               Wait for the specified time frame( in our case 3600 seconds )
 Input       : Takes the CLogger class object address as an argument
               which we have defined inside the CServer.h class.
 Output      : Calls the CLogger::MEMLogger class method.
               If succesfully happened then sleeps for the given time period
               When sleeping time frame expires then again calls the class method.
               Runs until STOP_MEM_LOG, ENCRYPT or SHUTDOWN operation occurs. 
 Return      : No explicit returns, but if something bad happens calls the 
               ERROR macro defined inside the Header.h file.
*/
void * MEMThread( void * pArg ){

   CLogger oCLoggerAddr = *( (CLogger*)pArg ) ;
   int nStatus;

   while(1){

    if( (nStatus = pthread_mutex_lock( &(oCLoggerAddr.m_MutexLock))) !=SUCCESS )
     ERROR( __func__, __LINE__, nStatus );

      oCLoggerAddr.MEMLogger();

    if( (nStatus = pthread_mutex_unlock( &(oCLoggerAddr.m_MutexLock))) !=SUCCESS )
     ERROR( __func__, __LINE__, nStatus );

      oCLoggerAddr.m_nSleepTime= TIME;
      sleep( oCLoggerAddr.m_nSleepTime );  //3600, 1 hour
   }
  return (NULL);
}// end

//--------------------------------------------------------------------------------------------------------------------------------------------------------------//
/*
 Method Name : CPUThread( void * )
 Date        : 19-June-2017
 Purpose     : CPUThread is a friend function with CLogger class.
               Directly calls the CLogger::CPULogger class method
               to perform Memory related operations.
               Wait for the specified time frame( in our case 3600 seconds )
 Input       : Takes the CLogger class object address as an argument
               which we have defined inside the CServer.h class.
 Output      : Calls the CLogger::CPULogger class method.
               If succesfully happened then sleeps for the given time period
               When sleeping time frame expires then again calls the class method.
               Runs until STOP_CPU_LOG, ENCRYPT or SHUTDOWN operation occurs. 
 Return      : No explicit returns, but if something bad happens calls the 
               ERROR macro defined inside the Header.h file.
*/
void * CPUThread( void * pArg){

   CLogger oCLoggerAddr = *( (CLogger*)pArg );
   int nStatus;

   while(1){

      if( (nStatus = pthread_mutex_lock( &(oCLoggerAddr.m_MutexLock))) !=SUCCESS ){
       ERROR( __func__, __LINE__, nStatus );
      }

        oCLoggerAddr.CPULogger();

      if( (nStatus = pthread_mutex_unlock( &(oCLoggerAddr.m_MutexLock))) !=SUCCESS ){
       ERROR( __func__, __LINE__, nStatus );
      }

        oCLoggerAddr.m_nSleepTime= TIME;
        sleep(oCLoggerAddr.m_nSleepTime);  //3600, 1 hour

   }
  return (NULL);
}// end

//---------------------------------------------------------------------------------------------------------------------------------------------------------------//
/*
 Method Name : ParseLine( char * )
 Date        : 19-June-2017
 Purpose     : ParseLine parse the integer value from the argument.
 Input       : char string which contains the data in character format
 Ouput       : Parse all the integers from the line
 Return      : Returns the parse integer from string.
*/
static int ParseLine ( char * pcLine ){

   int nLen = strlen( pcLine);
   const char *const_pcstr = pcLine;

    while(*const_pcstr< '0' || *const_pcstr >'9')
     const_pcstr++;

      pcLine[nLen-3]='\0';
     nLen=atoi(const_pcstr);
  return nLen;
}// end


