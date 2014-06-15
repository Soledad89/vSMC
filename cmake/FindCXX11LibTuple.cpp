#include <tuple>

template <typename... Types>
std::tuple<Types...> get_tuple () {return std::tuple<Types...>();}

int main ()
{
    std::tuple<double, int, char> tp = get_tuple<double, int, char>();

    std::get<0>(tp) = 0;
    std::get<1>(tp) = 1;
    std::get<2>(tp) = 2;

    return 0;
}