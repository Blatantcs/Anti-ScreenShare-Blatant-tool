#include <Windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <set>

bool P(const std::string& processName) {
    std::set<std::string> blockedProcesses = { "notepad.exe", "AnyDesk.exe", "SystemInformer.exe", "AppCrashView.exe", "ProcessHacker.exe", "JournalTrace.exe", "WinPrefetchView.exe", "PreviousFilesRecovery.exe", "Everything.exe", "Echo.exe" };
    return blockedProcesses.find(processName) != blockedProcesses.end();
}

void B() {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return;
    }

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe)) {
        do {
            std::wstring processNameW(pe.szExeFile);
            std::string processName(processNameW.begin(), processNameW.end());

            if (P(processName)) {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE | PROCESS_QUERY_INFORMATION, FALSE, pe.th32ProcessID);

                if (hProcess != NULL) {
                    if (TerminateProcess(hProcess, 0)) {
                    }
                    CloseHandle(hProcess);
                }
            }
        } while (Process32Next(hSnapshot, &pe));
    }

    CloseHandle(hSnapshot);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    FreeConsole();

    while (true) {
        B();
        Sleep(1);
    }

    return 0;
}
