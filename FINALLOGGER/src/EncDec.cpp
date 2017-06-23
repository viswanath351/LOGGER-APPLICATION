#include "EncDec.h"

//default constructor assign defaul value to class data members
 CEncDec::CEncDec(){

    m_const_pcNewFilePath = NULL;
    m_const_pcOldFilePath = NULL;
    m_nNewFileDescriptor = -1;
    m_nOldFileDescriptor = -1;
}// end

//------------------------------------------------------------------------------------------------------------------------------------------------------------//
/*
 Method Name : CEncDec::Encrypt  
 Date        : 19-June-2017
 Purpose     : Encrypt all the data from xml file.
               Open up the .xml file in read only mode.
               Creats new .csv file and adds the ENC initial to new file name.
               If ENC file already exist in the directroy(for working date), truncate all the previous data.
               Stores the data inside the .csv created file.
               Closes all the opened files.
 Input       : Takes the current logger file name as an initial argument without extension(.xml). 
 Output      : If successful, created new file in the directory and saves data in it.
 Return      : No explicit returns, but if something does not work than calls ERROR macro defined inside Header.h file.
*/
void CEncDec::Encrypt(const std::string & const_strFileName )
{

   const char * const_cFuncName = "CEncDec::Encrypt()";
   char szOldBuf[BUFSIZ];
   char szNewBuf[BUFSIZ];
   int nCount =0;
   int nReadData;  

    errno =0;									                             //setting errno to its default value

    std::string strNewFileName = "../FILE/ENC" +const_strFileName +".csv";	//creating the new file name
    std::string strOldFileName = "../FILE/" +const_strFileName + ".xml";	//creating the old file name as we are 																				 not taking name with .xml
 
    m_const_pcNewFilePath = strNewFileName.c_str();
    m_const_pcOldFilePath = strOldFileName.c_str();

      if( (m_nOldFileDescriptor = open( m_const_pcOldFilePath, O_RDONLY )) ==FAIL ){
       ERROR( const_cFuncName, __LINE__, errno );
      }

      if( (lseek( m_nOldFileDescriptor, 50, SEEK_SET )) ==FAIL ){		    //setting offset after the xml fil 																				defaul theader 																				
       ERROR( const_cFuncName, __LINE__, errno );
      }

      if( (m_nNewFileDescriptor = open( m_const_pcNewFilePath, O_CREAT | O_EXCL | O_RDWR, S_IROTH | S_IRGRP | S_IRWXU )) ==FAIL ){
 
	//truncate file if above calls fail
        if( (m_nNewFileDescriptor = open( m_const_pcNewFilePath, O_RDWR | O_TRUNC )) ==FAIL ){      
         ERROR( const_cFuncName, __LINE__, errno );
        }
      }

      if( ( nReadData = read(m_nOldFileDescriptor, szOldBuf, BUFSIZ )) ==FAIL ){
       ERROR( const_cFuncName, __LINE__, errno );
      }

        if( nReadData > SUCCESS ){
          char * pcstr = szOldBuf;

            while( *pcstr ){
             szNewBuf[nCount++] =  (*pcstr + ENCRYPT); 
             pcstr++;

              if( nReadData == (nCount-13) )
               break;
            }

           if( ( write( m_nNewFileDescriptor, szNewBuf, (nCount) )) ==FAIL ){		//write until last file tag found
            ERROR( const_cFuncName, __LINE__, errno );
           }

           Close();
 
        }
        else if( nReadData == SUCCESS ){		//no data in file to read
          Close();
        }
         
}// end

//------------------------------------------------------------------------------------------------------------------------------------------------------------//
/*
 Method Name : CEncDec::Decrypt  
 Date        : 19-June-2017
 Purpose     : Decrypt all the data from encrypted csv file.
               Open up the .csv file in read only mode.
               Creats new .xml file and adds the DEC initial to new file name.
               If DEC file already exist in the directroy, truncate all the previous data.
               Stores the data inside the .xml created file.
               Closes all the opened files.
 Input       : Takes the current encrypted file name as an initial argument without extension(.csv). 
 Output      : If successful, created new file in the directory and saves data in it.
 Return      : No explicit returns, but if something does not work than calls ERROR macro defined inside Header.h file.
*/
void CEncDec::Decrypt(const std::string & const_strFileName ){
  
   const char * const_cFuncName = "CEncDec::Decrypt()"; 
   int  nReadData;
   char szOldBuf[BUFSIZ];
   char szNewBuf[BUFSIZ];
   int  nCount =0; 

   std::string strOldFileName = "../FILE/ENC"+ const_strFileName + ".csv";		//generate the file name 
   std::string strNewFileName = "../FILE/DEC"+ const_strFileName + ".xml";

   m_const_pcNewFilePath = strNewFileName.c_str();
   m_const_pcOldFilePath = strOldFileName.c_str();
        
     errno = 0;										                            //setting errno to its default value

     if( (m_nOldFileDescriptor = open( m_const_pcOldFilePath, O_RDONLY )) ==FAIL ){
      ERROR( const_cFuncName, __LINE__, errno );
     }

     if( (m_nNewFileDescriptor = open( m_const_pcNewFilePath, O_CREAT | O_EXCL | O_RDWR, S_IRWXU | S_IROTH | S_IRGRP )) ==FAIL ){
     
 
       if( ( m_nNewFileDescriptor = open( m_const_pcNewFilePath, O_RDWR | O_TRUNC )) ==FAIL ){	//truncate the file 																								if file already exist
        ERROR( const_cFuncName, __LINE__, errno );
       }
     }

       if( ( nReadData = read( m_nOldFileDescriptor, szOldBuf, BUFSIZ )) ==FAIL ){
        ERROR( const_cFuncName, __LINE__, errno );
       }

         if( nReadData == SUCCESS ){		//no data in file to read
          Close();
          return;
         }
         else {

           char * pcstr = szOldBuf;
           char szUpperHeader[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<usage_log>";
           char szBelowHeader[] = "</usage_log>";
  
            while( *pcstr ){
             szNewBuf[nCount++] = (*pcstr + DECRYPT);
             pcstr++;  
            }

             if( (write( m_nNewFileDescriptor, szUpperHeader, strlen(szUpperHeader) )) ==FAIL )
              ERROR( const_cFuncName, __LINE__, errno );
             if( (write( m_nNewFileDescriptor, szNewBuf, nCount )) ==FAIL )
              ERROR( const_cFuncName, __LINE__, errno );
             //if( (write( m_nNewFileDescriptor, szBelowHeader, strlen(szBelowHeader) )) ==FAIL )
             // ERROR( const_cFuncName, __LINE__, errno );
           
         } 
      Close();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------//
/*
 Method Name : CEncDec::Encrypt  
 Date        : 19-June-2017
 Purpose     : Closes all the open files.
 Input       : Works on object data. 
 Output      : If successful, closes all open files.
 Return:     : No explicit returns, but if something does not work than calls ERROR macro from Header.h file.
*/
 void CEncDec::Close(){

    if( close(m_nOldFileDescriptor) ==FAIL || close(m_nNewFileDescriptor) ==FAIL ){
     ERROR( "CEncDec::Close()", __LINE__, errno );
    }
}// end
