// William Do
// CIS 375
// Team Tin Dog

#ifndef TRUCK_UPLOAD
#define TRUCK_UPLOAD

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;

void truckUpload()
{
	// Read from truckUpload and write to errorLog
	ifstream truckFileIn("truckUpload.txt");
	ofstream errorFileOut("errorLog.txt", ios::app);

	int lineCount = 0;
	int checkDigit;
	int rowTotal = 0;
	char lineIn[256];
	bool validRowTotal = true;
	bool validTruckID = true;
	string date;
	string seqNum;
	bool noErrors = true;

	if (truckFileIn)
	{
		/*
		TO BE IMPLEMENTED: clear master list of trucks in database HERE
		*/

		truckFileIn.getline(lineIn, 256, '\n'); // read in first line

		// Record sequence number and date
		seqNum = lineIn;
		seqNum = seqNum.substr(3, 4);
		date = lineIn;
		date = date.substr(13, 10);

		// Label error log
		cout << "Truck upload file " << seqNum << ", " << date << endl;
		errorFileOut << "Truck upload file " << seqNum << ", " << date << endl;


		truckFileIn.getline(lineIn, 256, '\n'); // read in second line

		while (lineIn[0] != 'T' && !truckFileIn.eof()) // Repeat until Trailer or end of file
		{
			lineCount++;
			validTruckID = true;

			// Make sure first 4 characters are integers
			for (int i = 0; i < 4; i++)
			{
				checkDigit = lineIn[i];
				if (checkDigit < 48 || checkDigit > 57)
				{
					cout << "LINE" << lineCount << ": Invalid truck ID" << endl;
					errorFileOut << "LINE" << lineCount << ": Invalid truck ID" << endl;
					validTruckID = false;
					noErrors = false;
					break;
				}
			}

			// Make sure line does not contain any other characters besides whitespace
			if (validTruckID)
			{
				for (int i = 4; i < 256; i++)
				{
					if (lineIn[i] == '\n' || lineIn[i] == '\0')
						break;
					if (lineIn[i] != ' ')
					{
						cout << "LINE " << lineCount << ": Invalid truck ID" << endl;
						errorFileOut << "LINE " << lineCount << ": Invalid truck ID" << endl;
						noErrors = false;
						break;
					}
				}
			}

			/*
			TO BE IMPLEMENTED: Add truck ID to database
			*/

			truckFileIn.getline(lineIn, 256, '\n'); // get next line
		}

		if (lineIn[0] == 'T')
		{
			// Make sure row total is four integers
			for (int i = 2; i < 6; i++)
			{
				checkDigit = lineIn[i];
				if (checkDigit < 48 || checkDigit > 57)
				{
					cout << "TRAILER: Invalid row total" << endl;
					errorFileOut << "TRAILER: Invalid row total" << endl;
					validRowTotal = false;
					noErrors = false;
					break;
				}
			}

			// Calculate row total and compare to lineCount
			if (validRowTotal)
			{
				for (int i = 2; i < 6; i++)
				{
					checkDigit = lineIn[i];
					checkDigit -= 48;
					rowTotal += checkDigit * pow(10, 5 - i);
				}
				if (rowTotal != lineCount)
				{
					cout << "TRAILER: Inaccurate row total" << endl;
					errorFileOut << "TRAILER: Inaccurate row total" << endl;
					noErrors = false;
				}
			}
		}
		else
		{
			cout << "NO TRAILER/TRAILER FORMATTED INCORRECTLY" << endl;
			errorFileOut << "NO TRAILER/TRAILER FORMATTED INCORRECTLY" << endl;
			noErrors = false;
		}
	}
	else
	{
		cout << "Truck upload file: ERROR - truckUpload.txt missing" << endl;
		errorFileOut << "Truck upload file: ERROR - truckUpload.txt missing" << endl;
	}

	if (noErrors)
	{
		cout << "There were no errors" << endl;
		errorFileOut << "There were no errors" << endl;
	}


	truckFileIn.close();
	errorFileOut.close();
}

#endif
