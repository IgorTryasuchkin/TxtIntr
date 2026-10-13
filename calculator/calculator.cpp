#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cerrno>
#include <cmath>
#include <iomanip>

using namespace std;

const size_t MIN_OPERANDS = 3;
const size_t MAX_OPERANDS = 5;

void print_usage(const char *prog)
{
    cout << "Мультипликативный калькулятор\n\n"
         << "Использование:\n"
         << "  " << prog << " -o <операция> <операнд1> <операнд2> <операнд3> [операнд4] [операнд5]\n"
         << "  " << prog << " --operation <операция> <операнды...>\n\n"
         << "Операции:\n"
         << "  mul, multiply   умножение всех операндов\n"
         << "  div, divide     деление первого операнда на все остальные\n\n"
         << "Параметры:\n"
         << "  -o, --operation <операция>  выполняемая операция (обязательный)\n"
         << "  -h, --help                  показать эту справку\n\n"
         << "Количество операндов: от " << MIN_OPERANDS << " до " << MAX_OPERANDS << ".\n\n"
         << "Примеры:\n"
         << "  " << prog << " -o mul 45 13 -2\n"
         << "  " << prog << " --operation div 100 2 5 2.5\n";
}

bool parse_number(const string &s, double &value)
{
    if (s.empty()) return false;
    char *end = nullptr;
    errno = 0;
    value = strtod(s.c_str(), &end);
    if (*end != '\0' || errno == ERANGE || !isfinite(value)) return false;
    return true;
}

bool looks_like_number(const string &s)
{
    double v;
    return parse_number(s, v);
}

int main(int argc, char *argv[])
{
    if (argc == 1) {
        print_usage(argv[0]);
        return 0;
    }

    string operation;
    bool have_operation = false;
    vector<double> operands;

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            print_usage(argv[0]);
            return 0;
        } else if (arg == "-o" || arg == "--operation") {
            if (i + 1 >= argc) {
                cerr << "Ошибка: у параметра " << arg << " не указано значение\n";
                return 1;
            }
            if (have_operation) {
                cerr << "Ошибка: операция указана более одного раза\n";
                return 1;
            }
            operation = argv[++i];
            have_operation = true;
        } else if (arg.rfind("--operation=", 0) == 0) {
            if (have_operation) {
                cerr << "Ошибка: операция указана более одного раза\n";
                return 1;
            }
            operation = arg.substr(12);
            have_operation = true;
        } else if (looks_like_number(arg)) {
            double v;
            parse_number(arg, v);
            operands.push_back(v);
        } else {
            cerr << "Ошибка: неизвестный параметр или неверное число: " << arg << "\n";
            cerr << "Для справки запустите программу без параметров или с -h\n";
            return 1;
        }
    }

    if (!have_operation) {
        cerr << "Ошибка: не задана операция (параметр -o / --operation)\n";
        return 1;
    }

    if (operands.size() < MIN_OPERANDS || operands.size() > MAX_OPERANDS) {
        cerr << "Ошибка: количество операндов должно быть от " << MIN_OPERANDS
             << " до " << MAX_OPERANDS << ", получено " << operands.size() << "\n";
        return 1;
    }

    double result;
    if (operation == "mul" || operation == "multiply") {
        result = 1.0;
        for (double x : operands) result *= x;
    } else if (operation == "div" || operation == "divide") {
        result = operands[0];
        for (size_t i = 1; i < operands.size(); i++) {
            if (operands[i] == 0.0) {
                cerr << "Ошибка: деление на ноль (операнд №" << i + 1 << ")\n";
                return 1;
            }
            result /= operands[i];
        }
    } else {
        cerr << "Ошибка: неизвестная операция \"" << operation << "\" (допустимы: mul, div)\n";
        return 1;
    }

    if (!isfinite(result)) {
        cerr << "Ошибка: результат выходит за допустимые пределы\n";
        return 1;
    }

    cout << setprecision(12) << result << "\n";
    return 0;
}
