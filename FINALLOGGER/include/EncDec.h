
#ifndef ENCDEC_H
#define ENCDEC_H

#include "Header.h"


/*
 Class Name   : CEncDec
 Created By   : Viswanath Gupta
 Created Date : 19-June-2012
 Version      : 1.0
 Purpose      : It is used for encrypting and decrypting the data of the pre existing XML file.
*/

class CEncDec {

private:
    const char * m_const_pcNewFilePath;			//holds the new file path
    const char * m_const_pcOldFilePath;			//holds the old file path

    int m_nNewFileDescriptor;				//file descriptor for handling operations on new file 
    int m_nOldFileDescriptor;				//file descriptor for handling read operation on old file

public:

    CEncDec();						//default constructor
   ~CEncDec(){}						//default destructor

/*
 PUR: Encrypt is used for doing encyprion on pre existing xml file data.
 PRE: Takes the filename as the initial argument
 POS: Reads all the data from the pre existing xml file name created by Logger.
      Encrypt the data according to our encryption algorithm.
      Creates and Open up the new file with adding ENC as the initial in the const_strFileName.
      Adds extension .csv by removing .xml in new file.
      Saves the data in the new file and closed the file accrodingly.  
*/
    void Encrypt(const std::string & const_strFileName );

/*
 PUR: Decrypt is used to decrypt the encrypted file data.
 PRE: Takes the filename of the existing file as the default argument
 POS: Reads all the data from the previous file.
      Decrypt the data as per our algorithm.
      Created new file and removes ENC initial and adds DEC as an initials.
      Removes extension .csv and adds the new extension .xml in new file.
      Saves the data in the new file and closes the file accrodingly.    
*/
    void Decrypt(const std::string & const_strFileName );

/*
 PUR: Closes all the open file descriptor.
 PRE: 
 POS: Close both the file , the new one and the old one.
      Returns error if not succesfully closed the file.
      Or, else return back to the calling function.
*/
    void Close();
}; //end

#endif /* enc_dec */
