 #logger

A) Package required to run Logger program
 
1) CppUnit
   INSTALL:  sudo apt-get install libcppunit-doc libcppunit-dev

2) cmake
   INSTALL:  sudo apt-get install cmake

B) How to make program run from terminal

Type these command in terminal

 1) mkdir Build ( it will create the directory with name Build. You can give any other name)

 2) cd Build ( You will get inside the directory , you can give your directory name which you have specified in above step)

 3) cmake .. ( it will create all the make files and dependencies inside Build folder)

 4) make ( creates the binary of name Logger if succesfully able to execute this command)

 C) How to run the binary created through above steps

 Type this command inside the terminal

 1) ./Logger & ( This will start the server as a background process ) 

 D) How to communicate with the Logger process

 1) ctrl+alt+t ( This will open up the new terminal )

 Type this command inside the terminal

 nc localhost 50000 ( this will establish the connetion with Logger )

 now use following commands -

 START_MEM_LOG - starts the memory logger
 STOP_MEM_LOG  - stops the memory logger
 START_CPU_LOG - starts the cpu logger
 STOP_CPU_LOG  - stops the cpu logger
 ENCRYPT       - encrypt the file
 DECRYPT       - decrypt the encrypted file
 SHUTDOWN      - shutdown the logger

 note:-
	1. all encrypted and decrypted files store in FILE folder which is already there

	2. stored data for memery is in KB and cpu usage is in %. 


