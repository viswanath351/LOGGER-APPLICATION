#include "Server.h"

//default constructor 
 CServer::CServer (){

   strcpy(m_szSuccesEncryptMsg, "File encrypted successfully.\n");
   strcpy(m_szSuccesDecryptMsg, "File decrypted successfully.\n");
   strcpy(m_szShutdownMsg, "Logger has successfully shutdown.\n");
   strcpy(m_szCPULogStartMsg,"CPU Logger started successfully.\n");
   strcpy(m_szCPULogStopMsg, "CPU Logger stopped successfully.\n"); 
   strcpy(m_szMEMLogStartMsg, "MEM Logger started successfully.\n");
   strcpy(m_szMEMLogStopMsg, "MEM Logger stopped successfully.\n");
   m_nRepeat = 0;
   m_nCheckCPULog = 0;
   m_nCheckMEMLog = 0;
   m_nCheckLogClose = 0;
}//end

//--------------------------------------------------------------------------------------------------------------------------------------------------------------//
/*
 Method Name : CServer::CheckCase(int & )
 Date        : 19-June-2017
 Purpose     : Sets the flag accordingly.
 Input       : Takes an r_nFlag variable as a reference
 Output      : Sets the r_nFlag variable after calling Socket call 
*/
void CServer::CheckCase( int & r_nFlag ){

  Socket();  
  r_nFlag = m_nRepeat;
}// end

//--------------------------------------------------------------------------------------------------------------------------------------------------------------//
/*
 Method Name : CServer::Socket 
 Date        : 19-June-2017
 Purpose     : Handles client server connections.
               Initialize the logger
               Bind socket to the localhost
               Assign the Port number for communication between client and server
               Accepts all the commands from the client.
               START_CPU_LOG : Starts the CPU Logger
               STAR_MEM_LOG  : Starts the MEM Logger
               STOP_CPU_LOG  : Stops the CPU Logger
               STOP_MEM_LOG  : Stops the MEM Logger
               ENCRYPT       : Encrypt the logger file  
               DECRYPT       : Decrypt the encrypted file
               SHUTDOWN      : Shutdown the logger and disconnects from client
 Input       : Client commands 
 Output      : Success or failure messages to the client's screen
 Reurn       : No explicit return. If calls fail then calls ERROR macro defined insdie Header.h.
               Return the succesful or any other messages to the user screen.
*/
void CServer::Socket(){

   int nOption =1;
   int nMEMStartedFirst =0;
   int nCPUStartedFirst =0;
   char szCommand[128] = "COM";
   const char * const_cFuncName = "CServer::Socket()";

   errno =0;
   memset( &m_stSocketInfo, 0, sizeof( struct sockaddr_in ));
   memset( m_szBuf, 0, sizeof(m_szBuf) );

    m_stSocketInfo.sin_family = AF_INET;
    m_stSocketInfo.sin_port   = htons( PORT );

     if( (OCLogger.Initializer( m_strCurrentWorkingFileName )) ==FAIL ){
      ERROR( const_cFuncName, __LINE__, errno );
     }

     if( (m_nServerFileDescriptor = socket(AF_INET, SOCK_STREAM, 0)) == FAIL){
      ERROR( const_cFuncName, __LINE__, errno ); 
     }
    
     if( (inet_pton( AF_INET, ADDR, &(m_stSocketInfo.sin_addr) )) ==FAIL ){
      ERROR( const_cFuncName, __LINE__, errno );
     }
 
     if( (setsockopt(m_nServerFileDescriptor, SOL_SOCKET, SO_REUSEADDR , &nOption, sizeof(nOption) )) ==FAIL ){
      ERROR( const_cFuncName, __LINE__, errno );
     }
    
     if( (bind(m_nServerFileDescriptor, (struct sockaddr *)&m_stSocketInfo, (socklen_t)sizeof(m_stSocketInfo))) ==FAIL ){
      ERROR( const_cFuncName, __LINE__, errno );
     }
    
     if( (listen(m_nServerFileDescriptor, BACKLOG)) ==FAIL ){
      ERROR( const_cFuncName, __LINE__, errno ); 
     }
 
     if( (m_nNewSocketFileDescriptor = accept(m_nServerFileDescriptor, NULL, 0)) ==FAIL ){
      ERROR( const_cFuncName, __LINE__, errno );
     }
      
      while(1){
        
       int nValRead = read(m_nNewSocketFileDescriptor, m_szBuf, 1024);

        if( nValRead ==FAIL ){
         send( m_nNewSocketFileDescriptor, m_szShutdownMsg, strlen( m_szShutdownMsg ), 0 );
          ERROR( const_cFuncName, __LINE__, errno );
        }
        else{
         sscanf(m_szBuf, "%s", szCommand);
        }

        if( strcmp(szCommand, "START_MEM_LOG") ==SUCCESS ){

          if( m_nCheckMEMLog ==0 ){
            int nStatus;

             if( (nStatus = pthread_create(&m_arr_Thread[0], NULL, MEMThread, &(OCLogger))) !=SUCCESS ){
              ERROR( const_cFuncName, __LINE__, nStatus );
             }
 
            send(m_nNewSocketFileDescriptor , m_szMEMLogStartMsg , strlen(m_szMEMLogStartMsg) , 0 );
            m_nCheckMEMLog =1;
     
             if( nCPUStartedFirst ==0 ){
               nMEMStartedFirst =1;
             }

          }
          else {
            const char const_szMsg[] = "Memory log is already up and running.\n"; 
            send( m_nNewSocketFileDescriptor, const_szMsg, strlen(const_szMsg), 0 ); 
          }
          m_nRepeat +=1;
        }

        else if( strcmp(szCommand, "STOP_MEM_LOG") ==SUCCESS ){

          if( m_nCheckMEMLog ==1 ){
            pthread_detach(m_arr_Thread[0]);
            send(m_nNewSocketFileDescriptor , m_szMEMLogStopMsg , strlen(m_szMEMLogStopMsg) , 0 );
            m_nCheckMEMLog =0;
            nMEMStartedFirst =0;
          }
          else {
            const char const_szMsg[] = "Memory log is not running.\n";
            send( m_nNewSocketFileDescriptor, const_szMsg, strlen(const_szMsg), 0 );
           
          }
          m_nRepeat +=1;
        }

        else if( strcmp(szCommand, "START_CPU_LOG") ==SUCCESS ){

          if( m_nCheckCPULog ==0 ){
            int nStatus;

             if( (nStatus = pthread_create(&m_arr_Thread[1], NULL, CPUThread, &(OCLogger) )) !=SUCCESS ){
              ERROR( const_cFuncName, __LINE__, nStatus );
             }

            send(m_nNewSocketFileDescriptor , m_szCPULogStartMsg , strlen(m_szCPULogStartMsg) , 0 );
            m_nCheckCPULog =1;

             if( nMEMStartedFirst ==0 ){
               nCPUStartedFirst =1;
             }
           
          }
          else {
            const char const_szMsg[] = "CPU log is already up and running.\n";
            send( m_nNewSocketFileDescriptor, const_szMsg, strlen(const_szMsg), 0 );
           
          }
          m_nRepeat +=1;
        }

        else if( strcmp(szCommand, "STOP_CPU_LOG") ==SUCCESS ){

          if( m_nCheckCPULog ==1 ){ 
            pthread_detach(m_arr_Thread[1]);
            send(m_nNewSocketFileDescriptor , m_szCPULogStopMsg , strlen(m_szCPULogStopMsg) , 0 );
            m_nCheckCPULog =0;
            nCPUStartedFirst =0;
          }
          else {
            const char const_szMsg[] = "CPU log is not running.\n";
            send( m_nNewSocketFileDescriptor, const_szMsg, strlen(const_szMsg), 0 );
          }
          m_nRepeat +=1;
        }

        else if( strcmp(szCommand, "ENCRYPT") ==SUCCESS ){

          if( m_nCheckCPULog ==1 && nCPUStartedFirst ==1){
	
            if( m_nCheckMEMLog ==1 ){
             pthread_detach(m_arr_Thread[0]);
             send(m_nNewSocketFileDescriptor , m_szMEMLogStopMsg , strlen(m_szMEMLogStopMsg) , 0 );
             m_nCheckMEMLog =0;
             nMEMStartedFirst =0; 
            }

            //stop CPU First and then MEM if started
            pthread_detach(m_arr_Thread[1]);
            send(m_nNewSocketFileDescriptor , m_szCPULogStopMsg , strlen(m_szCPULogStopMsg) , 0 );
            m_nCheckCPULog =0;
            nCPUStartedFirst =0;
            }   
  
          if( m_nCheckMEMLog ==1 && nMEMStartedFirst ==1 ){

            if( m_nCheckCPULog ==1 ){
             pthread_detach(m_arr_Thread[1]);
             send(m_nNewSocketFileDescriptor , m_szCPULogStopMsg , strlen(m_szCPULogStopMsg) , 0 );
             m_nCheckCPULog =0;
             nCPUStartedFirst =0;
            }

	        //stop mem first and then CPU if started
            pthread_detach(m_arr_Thread[0]);
            send(m_nNewSocketFileDescriptor , m_szMEMLogStopMsg , strlen(m_szMEMLogStopMsg) , 0 );
            m_nCheckMEMLog =0; 
            nMEMStartedFirst =0;
           }
          
          if( m_nCheckLogClose == 0){ 
           OCLogger.End();
           m_nCheckLogClose =1;
          }
           
           OCEncDec.Encrypt(m_strCurrentWorkingFileName);
           send(m_nNewSocketFileDescriptor , m_szSuccesEncryptMsg , strlen(m_szSuccesEncryptMsg) , 0 );

           const char szAsk[] = "Do you want to Decrypt the encrypted file.[y/n]\n";
           send( m_nNewSocketFileDescriptor, szAsk, strlen( szAsk ), 0);
           nValRead = read( m_nNewSocketFileDescriptor, m_szBuf, 1024 );

            if( (strncmp(m_szBuf, "Y",1)  ==SUCCESS) || (strncmp(m_szBuf, "y",1)) ==SUCCESS ){
             OCEncDec.Decrypt(m_strCurrentWorkingFileName);
             send(m_nNewSocketFileDescriptor , m_szSuccesDecryptMsg , strlen(m_szSuccesDecryptMsg) , 0);
           }
       
           send(m_nNewSocketFileDescriptor, m_szShutdownMsg, strlen( m_szShutdownMsg ), 0);
           m_nRepeat +=1;
           break;
           }

           else if( strcmp(szCommand, "DECRYPT") ==SUCCESS ){

           OCEncDec.Decrypt(m_strCurrentWorkingFileName);
           send(m_nNewSocketFileDescriptor , m_szSuccesDecryptMsg , strlen(m_szSuccesDecryptMsg) , 0 );
           m_nRepeat +=1;
           }

           else if( (strcmp(szCommand,"SHUTDOWN") ==SUCCESS) ){

          if( m_nCheckMEMLog ==1 ){
           pthread_detach(m_arr_Thread[0]); 
          }

          if( m_nCheckCPULog ==1 ){
           pthread_detach(m_arr_Thread[1]);
          }       
      
          if( m_nCheckLogClose  == 0 ){
           OCLogger.End();
          }
          send( m_nNewSocketFileDescriptor, m_szShutdownMsg, strlen( m_szShutdownMsg ), 0 );
          break;
        }

     }

  if( close( m_nServerFileDescriptor ) ==FAIL && close( m_nNewSocketFileDescriptor ) ==FAIL ){
   ERROR( const_cFuncName, __LINE__, errno );
  }
}// end
