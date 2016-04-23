// William Do
// CIS 375
// Team Tin Dog

#ifndef MAINTENANCE_UPLOAD
#define MAINTENANCE UPLOAD

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

void maintenanceUpload()
{
	// Open maintenance and error files to read from and write to
	ifstream maintenanceFileIn("maintenanceUpload.txt");
	ofstream errorFileOut("errorLog.txt", ios::app);

	char lineIn[256];			// To get lines from maintenance file
	int checkDigit;				// To verify truck IDs are composed of integers
	int lineCount = 0;			// Keep track of line count in case errors need to be reported
	string date;				// Store date to label error file
	string seqNum;				// Store sequence number to label error file
	string maintenanceReason;	// Store reason for maintenance to send to DB
	bool noErrors = true;		// Keep track of whether errors occured
	bool validTruckID = true;	// Allows us to stop checking truck ID once error is found


	if (maintenanceFileIn) // Does not execute if maintenanceUpload.txt is missing
	{
		// Get first line, store date, sequence number
		maintenanceFileIn.getline(lineIn, 256, '\n');
		seqNum = lineIn;
		seqNum = seqNum.substr(3, 4);
		date = lineIn;
		date = date.substr(13, 10);

		// Label error log
		cout << "Maintenance upload file " << seqNum << ", " << date << endl;
		errorFileOut << "Maintenance upload file " << seqNum << ", " << date << endl;

		maintenanceFileIn.getline(lineIn, 256, '\n'); // Get second line

		while (lineIn[0] != 'T' && !maintenanceFileIn.eof()) // Loop until trailer or end of file
		{
			lineCount++;
			validTruckID = true;

			// Check that ID is valid
			for (int i = 0; i < 4; i++)
			{
				checkDigit = lineIn[i];
				if (checkDigit < 48 || checkDigit > 57) // Truck ID contains a non-integer character
				{
					cout << "LINE " << to_string(lineCount) << ": Invalid truck ID" << endl;
					errorFileOut << "LINE " << to_string(lineCount) << ": Invalid truck ID" << endl;
					validTruckID = false;
					noErrors = false;
					break;
				}
			}

			// Get reason truck is out for maintenance
			maintenanceReason = lineIn;
			if (maintenanceReason.length() > 4)
			{
				maintenanceReason = maintenanceReason.substr(5, 250);
			}
			else
			{
				maintenanceReason = "No reason given";
			}
			

			/*
			TO BE IMPLEMENTED:
			Make sure ID is in Trucks table
				If not, error
			Make sure ID is NOT in Maintenance table
				If not, error
			Remove ID from Trucks table, add to Maintenance table
			Add reason for unavailability to Maintenance table
			*/

			maintenanceFileIn.getline(lineIn, 256, '\n'); // Get next line
		}

	}
	else // Maintenance file missing
	{
		cout << "Maintenance upload file: ERROR - maintenanceUpload.txt missing" << endl;
		errorFileOut << "Maintenance upload file: ERROR - maintenanceUpload.txt missing" << endl;
		noErrors = false;
	}

	if (noErrors) // No errors occurred
	{
		cout << "There were no errors" << endl;
		errorFileOut << "There were no errors" << endl;
	}

	maintenanceFileIn.close();
	errorFileOut.close();
}

#endif
