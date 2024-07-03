#include <iostream>
#include <map>
#include <vector>
#include <tuple>
#include <sstream>
#include <fstream>
#include "CSVParser.h"

using namespace std;

template<int I, int Max, typename Ch, typename Tr, typename... Args> struct output {
    static void out(basic_ostream<Ch, Tr>& os, tuple<Args...>const& t) {
        os << get<I>(t) << ' ';
        output<I+1, Max, Ch, Tr, Args...>::out(os, t);
    }
};

template<int Max, typename Ch, typename Tr, typename... Args> struct output<Max, Max, Ch, Tr, Args...> {
    static void out(basic_ostream<Ch, Tr>& os, tuple<Args...>const& t) {};
};

template<typename Ch, typename Tr, typename... Args>
auto operator<<(std::basic_ostream<Ch, Tr>& os, tuple<Args...> const& t) {
    output<0, sizeof...(Args), Ch, Tr, Args...>::out(os, t);
};


int main()
{
    ifstream file("input.txt");
    CSVParser<int, string, double> parser(file, ',', '"', 0);

    for (tuple<int, string, double> rs : (vector<tuple<int, string, double>>)parser) {
        cout << rs;
        cout << endl;
    }
}
