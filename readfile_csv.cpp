#include <iostream>
#include <vector>
#include <string> // подключаем строки
#include <fstream> // подключаем файлы
#include <sstream>
#include <map>
#include <algorithm>
#include <ctime>
#include <locale>
#include <iomanip>


using namespace std; // используем стандартное пространство имен
//***********************************************************************
std::vector<std::string> split(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}
//**************************************************************
//static std::time_t to_time_t(const std::string& str, bool is_dst = false, const std::string& format = "%d.%m.%Y %H:%M:%S")
std::tm to_time_t(const std::string& str, bool is_dst = false, const std::string& format = "%d.%m.%Y %H:%M:%S")
{
    std::tm t = { 0 };
    t.tm_isdst = is_dst ? 1 : 0;
    std::istringstream ss(str);
    ss >> std::get_time(&t, "%d.%m.%Y %H:%M:%S");
    mktime(&t);
    return t;
}
//*************************************************
struct Call {
    tm data{};
    string number{};
    int dt{};
    bool ish{};
};
//*************************************************************
int main() {
    setlocale(LC_ALL, "ru_ru");
    ifstream inf("list_112020.csv");
    inf.imbue(std::locale("ru_RU.UTF-8"));
    std::vector<vector<std::string>> arStr;
    std::map<string, int> list;
    const char* weekday[] = { "Воскресение", "Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота" };
    std::map<string, int> listCall;
    vector<Call> arCall;
    Call call;
    string s; // сюда будем класть считанные строки
//    ifstream file("list_112020.csv"); // файл из которого читаем (для линукс путь будет выглядеть по другому)
//******************************************************------------
    {
        std::tm t = {};
        std::istringstream ss("01.11.2020 10:51:51");
        ss.imbue(std::locale("de_DE.utf-8"));
        ss >> std::get_time(&t, "%d.%m.%Y %H:%M:%S");
        if (ss.fail()) {
            std::cout << "Parse failed\n";
        }
        else {
            std::cout << std::put_time(&t, "%c") << '\n';
        }
    }
//-----------------------------------------------------------------*/
    while (getline(inf, s)) { // пока не достигнут конец файла класть очередную строку в переменную (s)
//        cout << s << endl; // выводим на экран
        arStr.push_back(split(s, ','));
//        cout << s.substr(0, s.find(',')) << endl;
    }
    for (vector<std::string> ar : arStr) {
        if (ar.size() > 3 && ar[2][0] == '+') {
            if (list.find(ar[2]) == list.end()) {
                list.insert(make_pair(ar[2], stoi(ar[3])));
                listCall.insert(make_pair(ar[2], 1));
            }
            else {
                list[ar[2]] += stoi(ar[3]);
                listCall[ar[2]]++;
            }
            call.number = ar[2];
            call.data = to_time_t(ar[0]);
            call.dt = stoi(ar[3]);
            call.ish = ar[1][3] == -47 ? true : false;
//            cout << "c = " << ar[1][3] << " / " << ar[1][4] << " =>" << call.ish << endl;
            arCall.push_back(call);

        }
    }

    vector<pair<string, int>> vList;

    for (pair<string, int> item : list) {
//        cout << item.first << ": " << item.second << endl;
        vList.push_back(item);
    }
    std::sort(vList.begin(), vList.end(), [](pair<string, int> a, pair<string, int> b) {return a.second > b.second; });

    cout << endl << "**********************************" << endl;
    for (pair<string, int> item : vList) {
        cout << item.first << ": " << item.second << endl;
    }
    
    vList.clear();

    for (pair<string, int> item : listCall) {
        vList.push_back(item);
    }

    std::sort(vList.begin(), vList.end(), [](pair<string, int> a, pair<string, int> b) {return a.second > b.second; });

    cout << endl << "**********************************" << endl;
    for (pair<string, int> item : vList) {
        cout << item.first << ": " << item.second << endl;
    }
    cout << endl;

    vList.clear();

    string str;
    tm timeCall{};
    char* p{};
    cout << "Введите номер: ";
    cin >> str;
    cout << endl;

    struct OneDay {
        int dt;     //длительность
        int col;    //число звонков
        vector<pair<string, bool>> timeVal;
    };
    vector<OneDay> arDn{};
    OneDay oneDay{};
//    string s;
    for (int k = 0; k < 7; k++)
        arDn.push_back(oneDay);
    int prev_date = 0;
    int iSum{}, vSum{};
    for (Call i : arCall) {
        if (i.number.find(str.c_str()) != -1) {
 //           timeCall = to_time_t(i.data);
            arDn[i.data.tm_wday].dt += i.dt;
            arDn[i.data.tm_wday].col++;
            s = i.data.tm_hour > 9 ? "" : "0";
            s += to_string(i.data.tm_hour) + ":" + to_string(i.data.tm_min);
            arDn[i.data.tm_wday].timeVal.push_back(make_pair(s, i.ish));
            if (i.data.tm_mday != prev_date) {
                cout << "-------------" << std::put_time(&(i.data), "%d.%m.%Y") << " " << weekday[i.data.tm_wday]<< " -------------" << endl;
                prev_date = i.data.tm_mday;
            }
            cout << std::put_time(&(i.data), "%H:%M") << (i.ish ? "\t\t" : "") << " " << i.dt/60. << " мин. (" << (i.ish? "И":"В") << ")" <<endl;
            if (i.ish)
                iSum += i.dt;
            else
                vSum += i.dt;
        }
    }
    cout << endl << "Входящих: "<< vSum << ", Исходящих: " << iSum << "." << endl;;
    cout << endl << "*********** по дням недели *************" << endl;;
    int n{};
    for (OneDay o : arDn) {
        cout << weekday[n++] << ") общее время: " << o.dt/60 << " мин, число звонков: " << o.col << endl;
        sort(o.timeVal.begin(), o.timeVal.end(), [](pair<string, bool> a, pair<string, bool> b) {return a.first < b.first; });
        for (pair<string, bool> t : o.timeVal) {
            cout << t.first << "(" << (t.second?"И":"В") << ") ";
        }
        cout << endl;
    }
    //for (Call i : arCall) {
    //    if (i.number.find(str.c_str()) != -1) {
    //        cout << 
    //    }
    //}
    
    inf.close(); // обязательно закрываем файл что бы не повредить его

    return 0;
}