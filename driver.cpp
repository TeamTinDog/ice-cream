#include <iostream>
#include "truckupload.h"
#include "maintenanceUpload.h"
#include "truckRouteUpload.h"

using namespace std;

int main()
{

	truckUpload();
	maintenanceUpload();
	truckRouteUpload();

	getchar();

	return 0;
}
