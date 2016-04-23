// William Do
// CIS 375
// Team Tin Dog

#ifndef TRUCK_ROUTE_UPLOAD
#define TRUCK_ROUTE_UPLOAD

#include <iostream>
#include <string>
#include <fstream>

using namespace std;


void truckRouteUpload()
{
	// Open maintenance and error files to read from and write to
	ifstream truckRouteFileIn("truckRouteUpload.txt");
	ofstream errorFileOut("errorLog.txt", ios::app);

	char lineIn[256];			// To get lines from truck route file
	int checkDigit;				// To verify IDs are composed of integers
	int lineCount = 0;			// Keep track of line count in case errors need to be reported
	string date;				// Store date to label error file
	string seqNum;				// Store sequence number to label error file
	bool noErrors = true;		// Keep track of whether errors occured
	bool validTruckRoute = true;

	if (truckRouteFileIn)
	{
		// Get first line, store sequence number and date
		truckRouteFileIn.getline(lineIn, 256, '\n');
		seqNum = lineIn;
		seqNum = seqNum.substr(3, 4);
		date = lineIn;
		date = date.substr(13, 10);

		// Label error log
		cout << "Truck Route upload file " << seqNum << ", " << date << endl;
		errorFileOut << "Truck Route upload file " << seqNum << ", " << date << endl;

		truckRouteFileIn.getline(lineIn, 256, '\n'); // Get second line

		while (lineIn[0] != 'T' && !truckRouteFileIn.eof()) // Process lines until trailer or end of file
		{
			lineCount++;
			validTruckRoute = true;

			for (int i = 0; i < 8; i++) // Check validity of truck and route IDs
			{
				checkDigit = lineIn[i];
				if (checkDigit < 48 || checkDigit > 57)
				{
					if (i < 4) // If truck ID found to be invalid, print message and move to route ID
					{
						cout << "LINE " << lineCount << ": Invalid Truck ID" << endl;
						errorFileOut << "LINE " << lineCount << ": Invalid Truck ID" << endl;
						validTruckRoute = false;
						noErrors = false;
						i = 4;
					}
					else // If route ID found to be invalid, print message and break
					{
						cout << "LINE " << lineCount << ": Invalid Route ID" << endl;
						errorFileOut << "LINE " << lineCount << ": Invalid Route ID" << endl;
						validTruckRoute = false;
						noErrors = false;
						break;
					}
				}
			}

			if (validTruckRoute) // If truck and route IDs are valid, make sure rest of line is whitespace or nothing
			{
				for (int i = 8; i < 256; i++)
				{
					if (lineIn[i] == '\n' || lineIn[i] == '\0')
						break;
					if (lineIn[i] != ' ')
					{
						cout << "LINE " << lineCount << ": Invalid Truck or Route ID" << endl;
						errorFileOut << "LINE " << lineCount << ": Invalid Truck or Route ID" << endl;
						noErrors = false;
						break;
					}
				}
			}

			/*
			TO BE IMPLEMENTED:
			Make sure truck ID is in database
			Make sure route ID is in database
			Make sure route and truck are still unassigned
			Add truck/route pair to database
			*/

			truckRouteFileIn.getline(lineIn, 256, '\n'); // Get next line
		}
	}
	else // Truck route upload file missing
	{
		cout << "Truck Route upload file: ERROR - truckRouteUpload.txt missing" << endl;
		errorFileOut << "Truck Route upload file: ERROR - truckRouteUpload.txt missing" << endl;
		noErrors = false;
	}

	if (noErrors)
	{
		cout << "There were no errors" << endl;
		errorFileOut << "There were no errors" << endl;
	}

	truckRouteFileIn.close();
	errorFileOut.close();

}


#endif
