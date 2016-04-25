/*
	This is a test file of how to implement the code.
	My code checks the headers/trailers of all files (except the line count of Catering, which i can add if necessary).
	I used a switch case to check each file type for their individual errors.  Each time a CITY_FILE is checked,
	previous city and route information is erased by using clearErrors().  Currently my program is Case Sensitive,
	I can change that if necessary.  
*/


#include "filecheck.h"
#include <iostream>

using namespace std;

int main()
{
	bool status;

	fileCheck myFileCheck;
	status = myFileCheck.checkFile("city.txt", CITY_FILE);		// CITY CHECK
	if (status == false)
	{
		cout << "File was Rejected" << endl;
	}
	cout << myFileCheck.getNumErrors() << " Errors found in City File" << endl;

	myFileCheck.writeErrorfile("Errors.txt");
	myFileCheck.clearErrors();					// Must be called to clear errors.




	status = myFileCheck.checkFile("route.txt", ROUTE_FILE);  // ROUTE CHECK
	if (status == false)
	{
		cout << "File was Rejected" << endl;
	}
	cout << myFileCheck.getNumErrors() << " Errors found in Route File" << endl;

	myFileCheck.writeErrorfile("Errors.txt");




	status = myFileCheck.checkFile("load.txt", LOAD_FILES);		// Other files. This just checks the Header/Trailer of them.
	if (status == false)
	{
		cout << "File was Rejected" << endl;
	}
	cout << myFileCheck.getNumErrors() << " Errors found in Load Files" << endl;

	myFileCheck.writeErrorfile("Errors.txt");

	cin.get();
	return 0;
}