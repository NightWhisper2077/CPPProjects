#ifndef CSV_CSVPARSER_H
#define CSV_CSVPARSER_H

#include <vector>
#include <tuple>
#include <fstream>
#include <iostream>
#include <sstream>
#include <charconv>
#include <algorithm>

using namespace std;

template<typename... Tail> struct Counter;
template<typename Head, typename... Tail> struct Counter<Head, Tail...>:Counter<Tail...> {
    static int count () {
        return 1 + Counter<Tail...>::count();
    }
};

template<> struct Counter<> {
    static int count () {
        return 0;
    }
};

template<int I, int Max, typename... Args> struct B;
template<int I, int Max, typename... Args> struct B:B<I+1, Max, Args...> {
    static void a(vector<string> vec, tuple<Args...> t, tuple<Args...>* b, vector<int>& errors) {
        typename tuple_element<I, decltype(t)>::type type;

        stringstream ss(vec[I]);
        if constexpr (is_same_v<typename tuple_element<I, decltype(t)>::type, string>) {
            get<I>(*b) = vec[I];
        } else {
            if (!(ss >> std::noskipws >> type)) {
                errors.push_back(I);
            }
            else {
                get<I>(*b) = type;
            }
        }

        B<I+1, Max, Args...>::a(vec, t, b, errors);
    }
};

template<int Max, typename... Args> struct B<Max, Max, Args...>{
    static void a(vector<string> vec, tuple<Args...> t, tuple<Args...>* b, vector<int>& errors) {};
};

template<int Count, typename... Args>
void out (vector<string> vec, tuple<Args...> t, tuple<Args...>* b, vector<int>& errors) {
    B<0, Count, Args...>::a(vec, t, b, errors);
}

template <typename... Args>
class CSVParser {
public:
    CSVParser(ifstream& in, char del, char scr, int countLines) {
        string a;

        for (int i = 0; getline(in, a); i++) {
            if (i >= countLines) {
                strings.push_back(Parser(a, del, scr,i));
            }
        }
    }

    tuple<Args...> Parser(string str, char del, char scr, int row) {
        str = del + str + del;
        string resStr = "";
        string token;
        vector<string> tokens;
        tuple<Args...> res;
        vector<int> errors;
        bool flag = false;

        for (int i = 1; i < str.size() - 1; i++) {
            if (str[i] == scr && !flag) {
                if (str[i - 1] == del) {
                    flag = true;
                }
                else throw runtime_error("Incorrect format");
            }
            else if (str[i] == scr && flag) {
                if (str[i + 1] == del) {
                    flag = false;
                }
                else throw runtime_error("Incorrect format");
            }
            else if (str[i] == del && flag) {
                resStr.push_back(scr);
            }
            else resStr.push_back(str[i]);
        }

        stringstream ss(resStr);
        while (getline(ss, token, del)) {
            replace(token.begin(), token.end(), scr, del);
            tokens.push_back(token);
        }

        out<sizeof...(Args), Args...>(tokens, res, &res, errors);

        if (!errors.empty()) {
            for (int i = 0; i < errors.size(); i++) {
                cout << "Incorrect type of value in row: " << row + 1 << " column: " << errors[i] + 1<< endl;
            }
        }

        return res;
    }

    operator vector<tuple<Args...>> () const {
        return strings;
    }
private:
    vector<tuple<Args...>> strings;
};


#endif //CSV_CSVPARSER_H
