#ifndef TESTCASE_H
#define TESTCASE_H

#include "Header.h"

#include <cppunit/TestFixture.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCase.h>

#include "Server.h"

/*
 Class Name   : CTestCaseSolver
 Created By   : Viswanath Gupta
 Created Date : 19-Jun-2017
 Version      : 1.0
 Purpose      : This class has the soul purpose to provide test case for our Server class.
                If test is successful, prints the success message onto the screen
                If test is a failure, prints Failure message onto the screen
*/
  class CTestCaseSolver : public CppUnit::TestFixture {

    private:
      CServer * pCServer;			//CServer class pointer object

    public: 
/*
 PUR: Default destructor
 POS: deletes the dynamic memory allocated for CServer object.
*/
      virtual ~CTestCaseSolver(){
       delete pCServer;
      }

/*
 PUR: Suite method defines the test cases which are going to perform on CServer pointer object.
 PRE: CTestCaseSolver must be inherited to CppUnit::TestFixture
 POS: Returns the result of test case
*/
      static CppUnit :: Test * Suite(){

       CppUnit :: TestSuite * pSuiteOfTest = new CppUnit :: TestSuite( "TestLogger" );
       pSuiteOfTest->addTest( new CppUnit :: TestCaller<CTestCaseSolver>( "Test1 - Test case to check if commands    		   are working.",&CTestCaseSolver :: TestCaseOne ));
       return pSuiteOfTest;   
    }

    protected:  
/*
 PUR: Explicit test case defined
 PRE: Must declare the test case declaration in Suite method
 POS: Defines the CServer class object
      Calls CServer class method with the nFlag argument
      If the Server runs and any command executes then
      gets the nFlag value from the CServer class
      If value still remains 0, test returns fails 
*/
       void TestCaseOne(){
       int nFlag =0;
       CServer oCServer;
       oCServer.CheckCase(nFlag );
       CPPUNIT_ASSERT( 0 != nFlag ); 
      }

}; //end

#endif /* ~TestCase.h */
