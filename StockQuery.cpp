// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <ostream>

#include <windows.h>
#include <wininet.h>
#pragma comment(lib, "Wininet.lib")

#include "IniParser.h"

using namespace std;

std::unordered_map<std::string, std::string> g_index_string_map;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAXSIZE 1024*10

typedef struct StockInfo {
    std::string index_id;
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


//void urlopen_szzs();
//void urlopen_hs300();
void urlopen_zs(std::string& code, std::string& name);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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


void set_top_most()
{
    // GetConsoleWindow() => returns:
    // "handle to the window used by the console
    // associated with the calling process
    // or NULL
    // if there is no such associated console."
    HWND consoleWindowHandle = GetConsoleWindow();

    if (consoleWindowHandle){
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


void LoadStockToQuery(/*out*/std::vector<std::string>& stock_id_list)
{
    IniParser myparser;
    char str[100] = { 0 };
    try {
        myparser.Load("config.ini");
        std::string strValue;
        for (int i = 0; i < 100; ++i)
        {
            sprintf_s(str, "id%d", i);
            myparser.QueryKeyValue<std::string>("STOCK_ID", str, strValue);
            stock_id_list.push_back(strValue);
        }
    }
    catch (IniParseException &e) {
        //std::cout << e.what();
    }
}


void LoadStockIndexStringMap(const std::vector<std::string>& stock_id_list)
{
    IniParser myparser;
    char str[100] = { 0 };
    try {
        myparser.Load("config.ini");
        std::string strValue;
        for (auto i: stock_id_list)
        {
            myparser.QueryKeyValue<std::string>("STOCK_MAP", i, strValue);
            g_index_string_map.insert(std::make_pair(i, strValue));
        }
    }
    catch (IniParseException &e) {
        //std::cout << e.what();
    }
}



void urlopen(_TCHAR*);
std::string GetSubBtFind(std::string&, std::string&);


int Token(const char* pSep, char* pStr, StockInfo& stock_info);
void print_stock_info(StockInfo& stock_info);


int _tmain(int argc, _TCHAR* argv[]) {
    //set_top_most();

    // change console window size

    std::vector<std::string> stock_id_list;
    LoadStockToQuery(stock_id_list);
    LoadStockIndexStringMap(stock_id_list);
	char cmd[1024] = { 0 };
	sprintf_s(cmd, "mode CON: COLS=120 LINES=%d", stock_id_list.size()+7);
	system(cmd);

    std::string query_str = "http://hq.sinajs.cn/list=";
    for (auto i : stock_id_list)
    {
        query_str += i;
        query_str += ",";
    }

    query_str.erase(query_str.end() - 1);

    char exit_code = 1;
    while (exit_code != 'q') {
        cout.flush();
        system("cls");

        urlopen(const_cast<TCHAR*>(s2ws(query_str).c_str()));
        //urlopen(_T("http://hq.sinajs.cn/list=sh600718,sh600895,sz000002"));
        //urlopen(_T("http://hq.sinajs.cn/list=sh600895"));
        //urlopen(_T("http://hq.sinajs.cn/list=sz000002"));
        //urlopen_szzs();
		//urlopen_hs300();
		urlopen_zs(std::string("sh000300"), std::string("hs300"));
		urlopen_zs(std::string("sh000001"), std::string("szzs"));
		urlopen_zs(std::string("sz399001"), std::string("szcz"));
		urlopen_zs(std::string("sz399005"), std::string("zxbz"));
		urlopen_zs(std::string("sz399006"), std::string("cybz"));
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

                std::string index_id;
                std::string str = GetSubBtFind(strAll, index_id);
                char szpstr[1024];
                strcpy_s(szpstr, str.c_str());

                StockInfo info;
                info.index_id = index_id;
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
void urlopen_zs(std::string& code, std::string& name) {
	HINTERNET hSession = InternetOpen(_T("UrlTest"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hSession != NULL) {
		std::string url = "http://hq.sinajs.cn/list=s_";
		url += code;
		HINTERNET hHttp = InternetOpenUrlA(hSession, url.c_str(), NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
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
			//std::string name = outer; // s2ws(outer);
			std::string cur_index = strtok_s(NULL, ",", &context);
			std::string cur_price = strtok_s(NULL, ",", &context);
			std::string cur_zdl = strtok_s(NULL, ",", &context);
			std::string cur_cjs = strtok_s(NULL, ",", &context);
			std::string cur_cje = strtok_s(NULL, ",", &context);
			
			// init
			HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
			WORD wOldColorAttrs;
			CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
			// Save the current color  
			GetConsoleScreenBufferInfo(h, &csbiInfo);
			wOldColorAttrs = csbiInfo.wAttributes;

			// Set the new color 
			if (atof(cur_zdl.c_str()) < 0.00) {
				SetConsoleTextAttribute(h, FOREGROUND_GREEN| FOREGROUND_INTENSITY);
			}
			else {
				SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
			}

			//std::cout << std::left << setw(10) << std::string(name);
			std::cout << std::left << setw(8) << name;
			float f = std::stof(cur_zdl);

			if (f >= 0.00) {
				cur_zdl = std::string("+") + cur_zdl;
			}
			cur_zdl += "%";
			std::cout << std::left << setw(10) << cur_zdl;
			std::cout << std::left << setw(10) << cur_index;
			std::cout << std::left << setw(10) << cur_price;
			std::cout << std::left << setw(10) << cur_cjs;
			std::cout << std::endl;
			// Restore the original color  
			SetConsoleTextAttribute(h, wOldColorAttrs);

			InternetCloseHandle(hHttp);
			hHttp = NULL;

		}
		InternetCloseHandle(hSession);
		hSession = NULL;
	}
}

//void urlopen_hs300() {
//	HINTERNET hSession = InternetOpen(_T("UrlTest"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
//	if (hSession != NULL) {
//		HINTERNET hHttp = InternetOpenUrl(hSession, _T("http://hq.sinajs.cn/list=s_sh000300"), NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
//		if (hHttp != NULL) {
//			//wprintf_s(_T("%s\n"), url);
//			char Temp[MAXSIZE];
//			ULONG Number = 1;
//			InternetReadFile(hHttp, Temp, MAXSIZE - 1, &Number);
//			Temp[Number] = '\0';
//
//			std::string str = Temp;
//			char* context = NULL;
//			char* outer = strtok_s(Temp, ",", &context);
//			//std::cout << outer << std::endl;
//			std::string name = outer; // s2ws(outer);
//			std::string cur_index = strtok_s(NULL, ",", &context);
//			std::string cur_price = strtok_s(NULL, ",", &context);
//			std::string cur_zdl = strtok_s(NULL, ",", &context);
//			std::string cur_cjs = strtok_s(NULL, ",", &context);
//			std::string cur_cje = strtok_s(NULL, ",", &context);
//
//			std::cout << std::left << setw(10) << "hs300";
//			std::cout << std::left << setw(10) << cur_index;
//			std::cout << std::left << setw(10) << cur_price;
//			std::cout << std::left << setw(10) << cur_zdl;
//			std::cout << std::left << setw(10) << cur_cjs;
//			std::cout << std::endl;
//
//			InternetCloseHandle(hHttp);
//			hHttp = NULL;
//
//		}
//		InternetCloseHandle(hSession);
//		hSession = NULL;
//	}
//}
//
//void urlopen_szzs() {
//    HINTERNET hSession = InternetOpen(_T("UrlTest"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
//    if (hSession != NULL) {
//        HINTERNET hHttp = InternetOpenUrl(hSession, _T("http://hq.sinajs.cn/list=s_sh000001"), NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
//        if (hHttp != NULL) {
//            //wprintf_s(_T("%s\n"), url);
//            char Temp[MAXSIZE];
//            ULONG Number = 1;
//            InternetReadFile(hHttp, Temp, MAXSIZE - 1, &Number);
//            Temp[Number] = '\0';
//
//            std::string str = Temp;
//            char* context = NULL;
//            char* outer = strtok_s(Temp, ",", &context);
//            //std::cout << outer << std::endl;
//            std::string name = outer; // s2ws(outer);
//            std::string cur_index = strtok_s(NULL, ",", &context);
//            std::string cur_price = strtok_s(NULL, ",", &context);
//            std::string cur_zdl = strtok_s(NULL, ",", &context);
//            std::string cur_cjs = strtok_s(NULL, ",", &context);
//            std::string cur_cje = strtok_s(NULL, ",", &context);
//
//            std::cout << std::left << setw(10) << cur_index;
//            std::cout << std::left << setw(10) << cur_price;
//            std::cout << std::left << setw(10) << cur_zdl;
//            std::cout << std::left << setw(10) << cur_cjs;
//			std::cout << std::endl;
//
//            InternetCloseHandle(hHttp);
//            hHttp = NULL;
//
//        }
//        InternetCloseHandle(hSession);
//        hSession = NULL;
//    }
//}


std::string GetSubBtFind(std::string& scrStr, std::string& index_id) {
                
    std::string str_temp = "";
    int len = scrStr.length();
    int beginPos = scrStr.find("\"");
    int index_pos = beginPos - 9;
    index_id = scrStr.substr(index_pos, 8);
    scrStr = scrStr.substr(beginPos + 1, len);
    int endPos = scrStr.find("\"");

    str_temp = scrStr.substr(0, endPos);
    scrStr.erase(0, endPos + 1);
    return str_temp;
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
    auto it = g_index_string_map.find(stock_info.index_id);
    std::string name_str = it->second;
	double f = 0.00;
	if (stock_info.cur_price.compare("0.00") != 0) {
		f = 100 * (atof(stock_info.cur_price.c_str()) - atof(stock_info.yesterday_close_price.c_str())) / atof(stock_info.yesterday_close_price.c_str());
	}
	// init
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD wOldColorAttrs;
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

	// Save the current color  
	GetConsoleScreenBufferInfo(h, &csbiInfo);
	wOldColorAttrs = csbiInfo.wAttributes;

	// Set the new color 
	if (f < 0.00) {
		SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	}
	else {
		SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
	}
	std::cout << std::left << setw(8) << name_str;
	std::ostringstream out;
	out << std::setprecision(2) << f;
	std::string percent = out.str();
	if (f >= 0.00) {
		percent = std::string("+") + percent;
	}
	percent = percent + "%";
	std::cout << std::left << setw(10) << percent;
    std::cout << std::left << setw(10) << stock_info.cur_price;
    std::cout << std::left << setw(10) << "C-" + stock_info.yesterday_close_price;
    std::cout << std::left << setw(10) << "O-" + stock_info.today_open_price;

    std::ostringstream os;
    os << stock_info.today_highest_price << "/" << stock_info.today_lowest_price << " ";
    std::cout << std::left << setw(16) << os.str();
    //std::cout << std::left << setw(8) << stock_info.buy_1_price;
    //std::cout << std::left << setw(8) << stock_info.sell_1_price;
    //std::cout << std::left << setw(8) << stock_info.executed_shares;
    //std::cout << std::left << setw(8) << stock_info.executed_money;

    std::ostringstream os1;
    os1 << "B-" + stock_info.buy_1_number << "/" + stock_info.buy_1_price_dup << " ";
    std::cout << std::left << setw(16) << os1.str();

    std::ostringstream os2;
    os2 << "S-" + stock_info.sell_1_number << "/" + stock_info.sell_1_price_dup << " ";
    std::cout << std::left << setw(16) << os2.str();

    //std::cout << std::left << setw(8) << stock_info.buy_2_number;
    //std::cout << std::left << setw(8) << stock_info.buy_2_price;
    //std::cout << std::left << setw(8) << stock_info.buy_3_number;
    //std::cout << std::left << setw(8) << stock_info.buy_3_price;
    //std::cout << std::left << setw(8) << stock_info.buy_4_number;
    //std::cout << std::left << setw(8) << stock_info.buy_4_price;
    //std::cout << std::left << setw(8) << stock_info.buy_5_number;
    //std::cout << std::left << setw(8) << stock_info.buy_5_price;

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


    cout << endl;
	// Restore the original color  
	SetConsoleTextAttribute(h, wOldColorAttrs);

}