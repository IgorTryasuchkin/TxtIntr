#!/bin/bash
# Простой набор тестов: сравнение вывода и кода возврата
BIN=./calculator
pass=0; fail=0
check() { # описание, ожидаемый код, ожидаемый stdout (или "-" не проверять), аргументы...
    local desc="$1" code="$2" exp="$3"; shift 3
    out=$($BIN "$@" 2>/dev/null); rc=$?
    if [ "$rc" = "$code" ] && { [ "$exp" = "-" ] || [ "$out" = "$exp" ]; }; then
        echo "OK   $desc"; pass=$((pass+1))
    else
        echo "FAIL $desc (код=$rc, вывод='$out')"; fail=$((fail+1))
    fi
}
check "умножение 3 операндов"        0 -1170 -o mul 45 13 -2
check "умножение 5 операндов"        0 120   --operation mul 1 2 3 4 5
check "умножение с нулём"            0 0     -o mul 5 0 7
check "умножение дробных"            0 3     -o mul 0.5 2 3
check "деление 3 операндов"          0 5     -o div 100 4 5
check "деление 4 операндов"          0 2     -o div 100 2 5 5
check "деление отрицательных"        0 -4    -o div 40 -2 5
check "деление, форма --operation="  0 2     --operation=div 20 2 5
check "деление на ноль"              1 -     -o div 10 0 5
check "мало операндов (2)"           1 -     -o mul 1 2
check "много операндов (6)"          1 -     -o mul 1 2 3 4 5 6
check "нет операндов"                1 -     -o mul
check "нет операции"                 1 -     1 2 3
check "неизвестная операция"         1 -     -o sum 1 2 3
check "операнд не число"             1 -     -o mul 1 abc 3
check "параметр без значения"        1 -     -o
check "справка без параметров"       0 -
check "справка -h"                   0 -     -h
echo "Пройдено: $pass, провалено: $fail"
[ $fail -eq 0 ]
