// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
using namespace std;


#include <windows.h>
#include <wininet.h>

//http://blog.sina.com.cn/s/blog_a459dcf50101obn0.html

#define MAXSIZE 1024

#pragma comment(lib, "Wininet.lib")

void urlopen_szzs();

void set_top_most() {
    // GetConsoleWindow() => returns:
    // "handle to the window used by the console
    // associated with the calling process
    // or NULL
    // if there is no such associated console."
    HWND consoleWindowHandle = GetConsoleWindow();

    if (consoleWindowHandle) {
        cout << endl << "Setting up associated console window ON TOP !";
        SetWindowPos(
            consoleWindowHandle, // window handle
            HWND_TOPMOST, // "handle to the window to precede
            // the positioned window in the Z order
            // OR one of the following:"
            // HWND_BOTTOM or HWND_NOTOPMOST or HWND_TOP or HWND_TOPMOST
            0, 0, // X, Y position of the window (in client coordinates)
            0, 0, // cx, cy => width & height of the window in pixels
            SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW // The window sizing and positioning flags.
        );
        // OPTIONAL ! - SET WINDOW'S "SHOW STATE"
        ShowWindow(
            consoleWindowHandle, // window handle
            SW_NORMAL // how the window is to be shown
            // SW_NORMAL => "Activates and displays a window.
            // If the window is minimized or maximized,
            // the system restores it to its original size and position.
            // An application should specify this flag
            // when displaying the window for the first time."
        );
        cout << endl << "Done.";
    }
}


typedef struct StockInfo {
    std::string name;
    std::string today_open_price;
    std::string yesterday_close_price;
    std::string cur_price;
    std::string today_highest_price;
    std::string today_lowest_price;
    std::string buy_1_price;
    std::string sell_1_price;
    std::string executed_shares;
    std::string executed_money;
    std::string buy_1_number;
    std::string buy_1_price_dup;
    std::string buy_2_number;
    std::string buy_2_price;
    std::string buy_3_number;
    std::string buy_3_price;
    std::string buy_4_number;
    std::string buy_4_price;
    std::string buy_5_number;
    std::string buy_5_price;

    std::string sell_1_number;
    std::string sell_1_price_dup;
    std::string sell_2_number;
    std::string sell_2_price;
    std::string sell_3_number;
    std::string sell_3_price;
    std::string sell_4_number;
    std::string sell_4_price;
    std::string sell_5_number;
    std::string sell_5_price;

    std::string date;
    std::string time;
};


void urlopen(_TCHAR*);
std::string GetSubBtFind(std::string&);


int Token(const char* pSep, char* pStr, StockInfo& stock_info);
void print_stock_info(StockInfo& stock_info);


int _tmain(int argc, _TCHAR* argv[]) {
    set_top_most();

    char exit_code = 1;
    while (exit_code != 'q') {
        cout.flush();
        system("cls");
        urlopen(_T("http://hq.sinajs.cn/list=sh600718,sh600895,sz000002"));
        //urlopen(_T("http://hq.sinajs.cn/list=sh600895"));
        //urlopen(_T("http://hq.sinajs.cn/list=sz000002"));
        urlopen_szzs();
        Sleep(7700);
    }

    system("pause");
    return 0;
}



void urlopen(_TCHAR* url) {
    HINTERNET hSession = InternetOpen(_T("UrlTest"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (hSession != NULL) {
        HINTERNET hHttp = InternetOpenUrl(hSession, url, NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
        if (hHttp != NULL) {
            //wprintf_s(_T("%s\n"), url);
            char Temp[MAXSIZE];
            ULONG Number = 1;
            InternetReadFile(hHttp, Temp, MAXSIZE - 1, &Number);
            Temp[Number] = '\0';

            //printf("%s", Temp);


            std::string strAll = Temp;
            while (strAll.length() >= 10) {
                std::string str = GetSubBtFind(strAll);
                char szpstr[1024];
                strcpy_s(szpstr, str.c_str());
                std::vector<char*> vec;

                StockInfo info;
                if (0 == Token(",", szpstr, info)) {
                    print_stock_info(info);
                }
                //break;
            }

            InternetCloseHandle(hHttp);
            hHttp = NULL;

        }
        InternetCloseHandle(hSession);
        hSession = NULL;
    }
}



void urlopen_szzs() {
    HINTERNET hSession = InternetOpen(_T("UrlTest"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (hSession != NULL) {
        HINTERNET hHttp = InternetOpenUrl(hSession, _T("http://hq.sinajs.cn/list=s_sh000001"), NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
        if (hHttp != NULL) {
            //wprintf_s(_T("%s\n"), url);
            char Temp[MAXSIZE];
            ULONG Number = 1;
            InternetReadFile(hHttp, Temp, MAXSIZE - 1, &Number);
            Temp[Number] = '\0';

            std::string str = Temp;
            char* context = NULL;
            char* outer = strtok_s(Temp, ",", &context);
            //std::cout << outer << std::endl;
            std::string name = outer; // s2ws(outer);
            std::string cur_index = strtok_s(NULL, ",", &context);
            std::string cur_price = strtok_s(NULL, ",", &context);
            std::string cur_zdl = strtok_s(NULL, ",", &context);
            std::string cur_cjs = strtok_s(NULL, ",", &context);
            std::string cur_cje = strtok_s(NULL, ",", &context);

            std::cout << std::left << setw(10) << cur_index;
            std::cout << std::left << setw(10) << cur_price;
            std::cout << std::left << setw(10) << cur_zdl;
            std::cout << std::left << setw(10) << cur_cjs;

            InternetCloseHandle(hHttp);
            hHttp = NULL;

        }
        InternetCloseHandle(hSession);
        hSession = NULL;
    }
}


std::string GetSubBtFind(std::string& scrStr) {
    std::string str_temp = "";
    int len = scrStr.length();
    int beginPos = scrStr.find("\"");
    scrStr = scrStr.substr(beginPos + 1, len);
    int endPos = scrStr.find("\"");

    str_temp = scrStr.substr(0, endPos);
    scrStr.erase(0, endPos + 1);
    return str_temp;
}


std::wstring s2ws(const std::string& str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

std::string ws2s(const std::wstring& wstr) {
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], -1, 0, 0, 0, 0);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

int Token(const char* pSep, char* pStr, StockInfo& stock_info) {
    if (strlen(pStr) == 0) {
        return -1;
    }

    char* context = NULL;
    char* outer = strtok_s(pStr, pSep, &context);
    //std::cout << outer << std::endl;
    stock_info.name = outer; // s2ws(outer);
    stock_info.today_open_price = strtok_s(NULL, pSep, &context);
    stock_info.yesterday_close_price = strtok_s(NULL, pSep, &context);
    stock_info.cur_price = strtok_s(NULL, pSep, &context);
    stock_info.today_highest_price = strtok_s(NULL, pSep, &context);
    stock_info.today_lowest_price = strtok_s(NULL, pSep, &context);
    stock_info.buy_1_price = strtok_s(NULL, pSep, &context);
    stock_info.sell_1_price = strtok_s(NULL, pSep, &context);
    stock_info.executed_shares = strtok_s(NULL, pSep, &context);
    stock_info.executed_money = strtok_s(NULL, pSep, &context);
    stock_info.buy_1_number = strtok_s(NULL, pSep, &context);
    stock_info.buy_1_price_dup = strtok_s(NULL, pSep, &context);
    stock_info.buy_2_number = strtok_s(NULL, pSep, &context);
    stock_info.buy_2_price = strtok_s(NULL, pSep, &context);
    stock_info.buy_3_number = strtok_s(NULL, pSep, &context);
    stock_info.buy_3_price = strtok_s(NULL, pSep, &context);
    stock_info.buy_4_number = strtok_s(NULL, pSep, &context);
    stock_info.buy_4_price = strtok_s(NULL, pSep, &context);
    stock_info.buy_5_number = strtok_s(NULL, pSep, &context);
    stock_info.buy_5_price = strtok_s(NULL, pSep, &context);
    stock_info.sell_1_number = strtok_s(NULL, pSep, &context);
    stock_info.sell_1_price_dup = strtok_s(NULL, pSep, &context);
    stock_info.sell_2_number = strtok_s(NULL, pSep, &context);
    stock_info.sell_2_price = strtok_s(NULL, pSep, &context);
    stock_info.sell_3_number = strtok_s(NULL, pSep, &context);
    stock_info.sell_3_price = strtok_s(NULL, pSep, &context);
    stock_info.sell_4_number = strtok_s(NULL, pSep, &context);
    stock_info.sell_4_price = strtok_s(NULL, pSep, &context);
    stock_info.sell_5_number = strtok_s(NULL, pSep, &context);
    stock_info.sell_5_price = strtok_s(NULL, pSep, &context);
    stock_info.date = strtok_s(NULL, pSep, &context);
    stock_info.time = strtok_s(NULL, pSep, &context);

    return 0;
}


void print_stock_info(StockInfo& stock_info) {
    if (0 == stock_info.name.compare("东软集团"))
        std::cout << std::left << setw(8) << "drjt ";
    else if (0 == stock_info.name.compare("张江高科"))
        std::cout << std::left << setw(8) << "zjgk ";
    else
        std::cout << std::left << setw(8) << "wk   ";

    std::cout << std::left << setw(10) <<  stock_info.cur_price;

    std::cout << std::left << setw(20) << stock_info.today_open_price + "/"
              + stock_info.today_highest_price
              + "/" + stock_info.today_lowest_price;

    //std::cout << std::left << setw(8) << "C-" + stock_info.yesterday_close_price;

    //std::cout << std::left << setw(8) << stock_info.buy_1_price;
    //std::cout << std::left << setw(8) << stock_info.sell_1_price;
    //std::cout << std::left << setw(8) << stock_info.executed_shares;
    //std::cout << std::left << setw(8) << stock_info.executed_money;

    std::cout << std::left << setw(15) << "B-" + stock_info.buy_1_price_dup + "/" + stock_info.buy_1_number ;

    //std::cout << std::left << setw(8) << stock_info.buy_2_number;
    //std::cout << std::left << setw(8) << stock_info.buy_2_price;
    //std::cout << std::left << setw(8) << stock_info.buy_3_number;
    //std::cout << std::left << setw(8) << stock_info.buy_3_price;
    //std::cout << std::left << setw(8) << stock_info.buy_4_number;
    //std::cout << std::left << setw(8) << stock_info.buy_4_price;
    //std::cout << std::left << setw(8) << stock_info.buy_5_number;
    //std::cout << std::left << setw(8) << stock_info.buy_5_price;

    std::cout << std::left << setw(15) << "S-" + stock_info.sell_1_price_dup + "/" + stock_info.sell_1_number ;

    //std::cout << std::left << setw(8) << stock_info.sell_2_number;
    //std::cout << std::left << setw(8) << stock_info.sell_2_price;
    //std::cout << std::left << setw(8) << stock_info.sell_3_number;
    //std::cout << std::left << setw(8) << stock_info.sell_3_price;
    //std::cout << std::left << setw(8) << stock_info.sell_4_number;
    //std::cout << std::left << setw(8) << stock_info.sell_4_price;
    //std::cout << std::left << setw(8) << stock_info.sell_5_number;
    //std::cout << std::left << setw(8) << stock_info.sell_5_price;
    //std::cout << std::left << setw(8) << stock_info.date;
    //std::cout << std::left << setw(8) << stock_info.time;

    double f = 100 * (atof(stock_info.cur_price.c_str()) - atof(stock_info.today_open_price.c_str())) / atof(stock_info.today_open_price.c_str());
    std::cout << std::left << setw(10) << f;
    cout << endl;
}