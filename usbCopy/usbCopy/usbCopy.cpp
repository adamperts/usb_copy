#include "helper.h"

int main()
{
    vector <string> avalibleDriveList = getCurrentExturnalDevices();
    bool succesfulCopy = copyDrive(avalibleDriveList);
    
    cout << endl << succesfulCopy << endl;
    //createNewDir("abc");
}