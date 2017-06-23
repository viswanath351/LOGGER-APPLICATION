#include "Header.h"
#include <cppunit/TestSuite.h>
#include <cppunit/ui/text/TestRunner.h> 

#include "TestCase.h"

int main(int argc, char *argv[]) {


  CppUnit::TextUi::TestRunner runner;
  
  std::cout << "\nCreating Test suites:" << std::endl;
  runner.addTest( CTestCaseSolver::Suite());
  std::cout << "\nRunning unit test." << std::endl;
  runner.run();
  
exit( EXIT_SUCCESS );
}
