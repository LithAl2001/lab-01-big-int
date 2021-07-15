#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

namespace Lab01 {
    enum class CompareResult {
        LESS,
        MORE,
        EQUAL
    };

    class BigInt {
    private:
        std::vector<int> digits;

        unsigned int length() const {
            return digits.size();
        }

        void add(const BigInt &a, int sign) {
            // Сложение и перенос разряда:
            int trans = 0;
            for (unsigned int i = 0; i < a.length(); i++) {
                if (i == digits.size()) {
                    digits.push_back(0);
                }
                digits[i] += sign * a.digits[i] + trans;
                trans = 0;
                if (digits[i] > 9) {
                    digits[i] -= 10;
                    trans++;
                }
                if (digits[i] < 0) {
                    digits[i] += 10;
                    trans--;
                }
            }
            // ---------------------------
            // Прибавление или вычитание оставшегося 1en
            if (trans > 0) {
                unsigned int transvection = 1;
                while (true) {
                    if (digits.size() == a.length() + transvection - 1) {
                        digits.push_back(0);
                    }
                    digits[a.length() + transvection - 1]++;
                    if (digits[a.length() + transvection - 1] > 9) {
                        digits[a.length() + transvection - 1] -= 10;
                        transvection++;
                    } else {
                        break;
                    }
                }
            }
            if (trans < 0) {
                unsigned int transvection = 1;
                while (true) {
                    digits[a.length() + transvection - 1]--;
                    if (digits[a.length() + transvection - 1] < 0) {
                        digits[a.length() + transvection - 1] += 10;
                        transvection++;
                    } else {
                        break;
                    }
                }
            }
            // ---------------------------
            // Срезание ведущих нулей
            while (!digits.empty() && !digits.back()) {
                digits.pop_back();
            }
            // ---------------------------
        }

        friend CompareResult relation(const BigInt &a, const BigInt &b);
        friend bool operator==(const BigInt &a, const BigInt &b);
        friend bool operator!=(const BigInt &a, const BigInt &b);
        friend bool operator<(const BigInt &a, const BigInt &b);
        friend bool operator>(const BigInt &a, const BigInt &b);
        friend bool operator<=(const BigInt &a, const BigInt &b);
        friend bool operator>=(const BigInt &a, const BigInt &b);
        friend BigInt &operator+=(BigInt &a, const BigInt &b);
        friend BigInt &operator-=(BigInt &a, const BigInt &b);
    public:
        explicit BigInt(const std::string &s) {
            unsigned int bound = 0;
            while (bound < s.size() && s[bound] == '0') {
                bound++;
            }
            for (unsigned int i = s.size() - 1; i + 1 - bound; i--) {
                digits.push_back(s[i] - '0');
            }
        }

        BigInt(unsigned int a = 0) {
            while (a) {
                digits.push_back(a % 10);
                a /= 10;
            }
        }

        std::string to_string() const {
            std::string answer;
            if (digits.empty()) {
                answer = "0";
            } else {
                for (unsigned int i = digits.size() - 1; i + 1; i--) {
                    answer += digits[i] + '0';
                }
            }
            return answer;
        }
    };

    CompareResult relation(const BigInt &a, const BigInt &b) {
        if (a.length() < b.length()) {
            return CompareResult::LESS;
        }
        if (a.length() > b.length()) {
            return CompareResult::MORE;
        }
        for (unsigned int i = a.length() - 1; i + 1; i--) {
            if (a.digits[i] < b.digits[i]) {
                return CompareResult::LESS;
            }
            if (a.digits[i] > b.digits[i]) {
                return CompareResult::MORE;
            }
        }
        return CompareResult::EQUAL;
    }

// При необходимости, можете дописать что-то сюда.
// Также по необходимости можно добавлять инклуды.

    bool operator==(const BigInt &a, const BigInt &b) {
        return relation(a, b) == CompareResult::EQUAL;
    }

    bool operator!=(const BigInt &a, const BigInt &b) {
        return relation(a, b) != CompareResult::EQUAL;
    }

    bool operator<(const BigInt &a, const BigInt &b) {
        return relation(a, b) == CompareResult::LESS;
    }

    bool operator>(const BigInt &a, const BigInt &b) {
        return relation(a, b) == CompareResult::MORE;
    }

    bool operator<=(const BigInt &a, const BigInt &b) {
        return relation(a, b) != CompareResult::MORE;
    }

    bool operator>=(const BigInt &a, const BigInt &b) {
        return relation(a, b) != CompareResult::LESS;
    }

    BigInt &operator+=(BigInt &a, const BigInt &b) {
        a.add(b, 1);
        return a;
    }

    BigInt operator+(const BigInt &a, const BigInt &b) {
        BigInt c = a;
        c += b;
        return c;
    }

    BigInt &operator-=(BigInt &a, const BigInt &b) {
        a.add(b, -1);
        return a;
    }

    BigInt operator-(const BigInt &a, const BigInt &b) {
        BigInt c = a;
        c -= b;
        return c;
    }

    BigInt &operator++(BigInt &a) {
        a += 1;
        return a;
    }

    BigInt operator++(BigInt &a, int) {
        BigInt copy = a;
        ++a;
        return copy;
    }

    BigInt &operator--(BigInt &a) {
        a -= 1;
        return a;
    }

    BigInt operator--(BigInt &a, int) {
        BigInt copy = a;
        --a;
        return copy;
    }

    std::ostream &operator<<(std::ostream &out, const BigInt &a) {
        out << a.to_string();
        return out;
    }

    std::istream &operator>>(std::istream &in, BigInt &a) {
        std::string s;
        in >> s;
        a = BigInt(s);
        return in;
    }
};

// Тесты.
// По умолчанию не запускается ни один тест. По мере готовности задания можно раскомментировать строчки ниже. Если
// раскомментировать строчку, перекомпилировать и запустить программу, запустится тест, проверяющий одну из требуемых
// частей задания. Для удобства справа предоставлен код, который необходимо (но не достаточно) должен компилироваться
// для прохождения теста.
// При сдаче необходимо раскомментировать все строчки ниже.
// РЕШЕНИЯ, В КОТОРЫХ ЧАСТЬ ТЕСТОВ ЗАКОММЕНТИРОВАНА, НЕ ПРОЙДУТ АВТОМАТИЧЕСКОЕ ТЕСТИРОВАНИЕ.

#define TEST_CONSTRUCTOR                // BigInt x("179"); BigInt y(57);
#define TEST_EQUAL_OPERATOR             // BigInt a("179"), b("57"); a == b; 179 == a; a == 179;
#define TEST_NOT_EQUAL_OPERATOR         // BigInt a("179"), b("57"); a != b; 179 != b; b != 179;
#define TEST_LESS_OPERATOR              // BigInt a("179"), b("57"); a < b;  57 < a;   b < 179;
#define TEST_GREATER_OPERATOR           // BigInt a("179"), b("57"); a > b;  179 > b;  a > 57;
#define TEST_LESS_OR_EQUAL_OPERATOR     // BigInt a("179"), b("57"); a <= b; 57 <= a;  b <= 179;
#define TEST_GREATER_OR_EQUAL_OPERATOR  // BigInt a("179"), b("57"); a >= b; 179 >= b; a >= 57;
#define TEST_PLUS_OPERATOR              // BigInt a("179"), b("57"); a + b;  122 + b;  b + 122; a += b; b += 122;
#define TEST_MINUS_OPERATOR             // BigInt a("179"), b("57"); a - b;  a - 122;  200 - a; a -= b; b -= 17;
#define TEST_INCREMENT_OPERATOR         // BigInt a("177"); a++; ++a;
#define TEST_DECREMENT_OPERATOR         // BigInt a("181"); a--; --a;
#define TEST_TO_STRING                  // BigInt a("179"); a.to_string();
#define TEST_INPUT                      // BigInt a; std::cin >> a;
#define TEST_OUTPUT                     // BigInt a("179"); std::cout << a;
#define TEST_ASSIGNMENT_OPERATORS       // BigInt a("179"), b("57"); BigInt c; c = a; c -= b; c += a;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// НИЖЕ НАХОДЯТСЯ АВТОТЕСТЫ
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Изменять любым способом код, находящийся ниже, запрещено. Решения, в которых это правило нарушено, не проверяются.

#ifdef TEST_CONSTRUCTOR

void test_constructor() {
    Lab01::BigInt x("179");
    Lab01::BigInt y(57);
    std::cout << "constructor test passed" << std::endl;
}

#endif // TEST_CONSTRUCTOR

#ifdef TEST_EQUAL_OPERATOR

void test_equal_operator() {
    assert(!(Lab01::BigInt("0") == Lab01::BigInt("1")));
    assert(!(Lab01::BigInt("111111111111111999999999999999") == Lab01::BigInt("999999999999999111111111111111")));
    assert(!(Lab01::BigInt("999999999999999999999999999999") == Lab01::BigInt("1000000000000000000000000000000")));
    assert(!(100 == Lab01::BigInt("101")));
    assert(!(Lab01::BigInt("100") == 101));

    assert(!(Lab01::BigInt("1") == Lab01::BigInt("0")));
    assert(!(Lab01::BigInt("999999999999999111111111111111") == Lab01::BigInt("111111111111111999999999999999")));
    assert(!(Lab01::BigInt("1000000000000000000000000000000") == Lab01::BigInt("999999999999999999999999999999")));
    assert(!(Lab01::BigInt("101") == 100));
    assert(!(101 == Lab01::BigInt("100")));

    assert(Lab01::BigInt("0") == Lab01::BigInt("0"));
    assert(Lab01::BigInt("111111111111111111111111111111111") == Lab01::BigInt("111111111111111111111111111111111"));
    assert(100 == Lab01::BigInt("100"));
    assert(Lab01::BigInt("100") == 100);

    std::cout << "equal_operator test passed" << std::endl;
}

#endif // TEST_EQUAL_OPERATOR

#ifdef TEST_NOT_EQUAL_OPERATOR

void test_not_equal_operator() {
    assert(Lab01::BigInt("0") != Lab01::BigInt("1"));
    assert(Lab01::BigInt("111111111111111999999999999999") != Lab01::BigInt("999999999999999111111111111111"));
    assert(Lab01::BigInt("999999999999999999999999999999") != Lab01::BigInt("1000000000000000000000000000000"));
    assert(100 != Lab01::BigInt("101"));
    assert(Lab01::BigInt("100") != 101);

    assert(Lab01::BigInt("1") != Lab01::BigInt("0"));
    assert(Lab01::BigInt("999999999999999111111111111111") != Lab01::BigInt("111111111111111999999999999999"));
    assert(Lab01::BigInt("1000000000000000000000000000000") != Lab01::BigInt("999999999999999999999999999999"));
    assert(Lab01::BigInt("101") != 100);
    assert(101 != Lab01::BigInt("100"));

    assert(!(Lab01::BigInt("0") != Lab01::BigInt("0")));
    assert(!(Lab01::BigInt("111111111111111111111111111111111") != Lab01::BigInt("111111111111111111111111111111111")));
    assert(!(100 != Lab01::BigInt("100")));
    assert(!(Lab01::BigInt("100") != 100));

    std::cout << "not_equal_operator test passed" << std::endl;
}

#endif // TEST_NOT_EQUAL_OPERATOR

#ifdef TEST_LESS_OPERATOR

void test_less_operator() {
    assert(Lab01::BigInt("0") < Lab01::BigInt("1"));
    assert(Lab01::BigInt("111111111111111999999999999999") < Lab01::BigInt("999999999999999111111111111111"));
    assert(Lab01::BigInt("999999999999999999999999999999") < Lab01::BigInt("1000000000000000000000000000000"));
    assert(100 < Lab01::BigInt("101"));
    assert(Lab01::BigInt("100") < 101);

    assert(!(Lab01::BigInt("1") < Lab01::BigInt("0")));
    assert(!(Lab01::BigInt("999999999999999111111111111111") < Lab01::BigInt("111111111111111999999999999999")));
    assert(!(Lab01::BigInt("1000000000000000000000000000000") < Lab01::BigInt("999999999999999999999999999999")));
    assert(!(Lab01::BigInt("101") < 100));
    assert(!(101 < Lab01::BigInt("100")));

    assert(!(Lab01::BigInt("0") < Lab01::BigInt("0")));
    assert(!(Lab01::BigInt("111111111111111111111111111111111") < Lab01::BigInt("111111111111111111111111111111111")));
    assert(!(100 < Lab01::BigInt("100")));
    assert(!(Lab01::BigInt("100") < 100));

    std::cout << "less_operator test passed" << std::endl;
}

#endif // TEST_LESS_OPERATOR

#ifdef TEST_GREATER_OPERATOR

void test_greater_operator() {
    assert(!(Lab01::BigInt("0") > Lab01::BigInt("1")));
    assert(!(Lab01::BigInt("111111111111111999999999999999") > Lab01::BigInt("999999999999999111111111111111")));
    assert(!(Lab01::BigInt("999999999999999999999999999999") > Lab01::BigInt("1000000000000000000000000000000")));
    assert(!(100 > Lab01::BigInt("101")));
    assert(!(Lab01::BigInt("100") > 101));

    assert(Lab01::BigInt("1") > Lab01::BigInt("0"));
    assert(Lab01::BigInt("999999999999999111111111111111") > Lab01::BigInt("111111111111111999999999999999"));
    assert(Lab01::BigInt("1000000000000000000000000000000") > Lab01::BigInt("999999999999999999999999999999"));
    assert(Lab01::BigInt("101") > 100);
    assert(101 > Lab01::BigInt("100"));

    assert(!(Lab01::BigInt("0") > Lab01::BigInt("0")));
    assert(!(Lab01::BigInt("111111111111111111111111111111111") > Lab01::BigInt("111111111111111111111111111111111")));
    assert(!(100 > Lab01::BigInt("100")));
    assert(!(Lab01::BigInt("100") > 100));

    std::cout << "greater_operator test passed" << std::endl;
}

#endif // TEST_GREATER_OPERATOR

#ifdef TEST_LESS_OR_EQUAL_OPERATOR

void test_less_or_equal_operator() {
    assert(Lab01::BigInt("0") <= Lab01::BigInt("1"));
    assert(Lab01::BigInt("111111111111111999999999999999") <= Lab01::BigInt("999999999999999111111111111111"));
    assert(Lab01::BigInt("999999999999999999999999999999") <= Lab01::BigInt("1000000000000000000000000000000"));
    assert(100 <= Lab01::BigInt("101"));
    assert(Lab01::BigInt("100") <= 101);

    assert(!(Lab01::BigInt("1") <= Lab01::BigInt("0")));
    assert(!(Lab01::BigInt("999999999999999111111111111111") <= Lab01::BigInt("111111111111111999999999999999")));
    assert(!(Lab01::BigInt("1000000000000000000000000000000") <= Lab01::BigInt("999999999999999999999999999999")));
    assert(!(Lab01::BigInt("101") <= 100));
    assert(!(101 <= Lab01::BigInt("100")));

    assert(Lab01::BigInt("0") <= Lab01::BigInt("0"));
    assert(Lab01::BigInt("111111111111111111111111111111111") <= Lab01::BigInt("111111111111111111111111111111111"));
    assert(100 <= Lab01::BigInt("100"));
    assert(Lab01::BigInt("100") <= 100);

    std::cout << "less_or_equal_operator test passed" << std::endl;
}

#endif // TEST_LESS_OR_EQUAL_OPERATOR

#ifdef TEST_GREATER_OR_EQUAL_OPERATOR

void test_greater_or_equal_operator() {
    assert(!(Lab01::BigInt("0") >= Lab01::BigInt("1")));
    assert(!(Lab01::BigInt("111111111111111999999999999999") >= Lab01::BigInt("999999999999999111111111111111")));
    assert(!(Lab01::BigInt("999999999999999999999999999999") >= Lab01::BigInt("1000000000000000000000000000000")));
    assert(!(100 >= Lab01::BigInt("101")));
    assert(!(Lab01::BigInt("100") >= 101));

    assert(Lab01::BigInt("1") >= Lab01::BigInt("0"));
    assert(Lab01::BigInt("999999999999999111111111111111") >= Lab01::BigInt("111111111111111999999999999999"));
    assert(Lab01::BigInt("1000000000000000000000000000000") >= Lab01::BigInt("999999999999999999999999999999"));
    assert(Lab01::BigInt("101") >= 100);
    assert(101 >= Lab01::BigInt("100"));

    assert(Lab01::BigInt("0") >= Lab01::BigInt("0"));
    assert(Lab01::BigInt("111111111111111111111111111111111") >= Lab01::BigInt("111111111111111111111111111111111"));
    assert(100 >= Lab01::BigInt("100"));
    assert(Lab01::BigInt("100") >= 100);

    std::cout << "greater_or_equal_operator test passed" << std::endl;
}

#endif // TEST_GREATER_OR_EQUAL_OPERATOR

#ifdef TEST_PLUS_OPERATOR

void test_plus_operator() {
    Lab01::BigInt a;
    Lab01::BigInt b = a += 10000;
    assert("10000" == a.to_string());
    assert("10000" == b.to_string());
    a += 0;
    assert("10000" == a.to_string());
    a += Lab01::BigInt("999999999999999999999999999999");
    assert("1000000000000000000000000009999" == a.to_string());
    a += 1;
    assert("1000000000000000000000000010000" == a.to_string());

    assert("1000000000000000000000000009999" == (Lab01::BigInt("999999999999999999999999999999") + 10000).to_string());
    assert("1000000000000000000000000009999" == (10000 + Lab01::BigInt("999999999999999999999999999999")).to_string());

    std::cout << "plus_operator test passed" << std::endl;
}

#endif // TEST_PLUS_OPERATOR

#ifdef TEST_MINUS_OPERATOR

void test_minus_operator() {
    Lab01::BigInt a("1000000000000000000000000010000");
    Lab01::BigInt b = a -= 1;
    assert("1000000000000000000000000009999" == a.to_string());
    assert("1000000000000000000000000009999" == b.to_string());
    a -= 0;
    assert("1000000000000000000000000009999" == a.to_string());
    a -= Lab01::BigInt("999999999999999999999999999999");
    assert("10000" == a.to_string());
    a -= 10000;
    assert("0" == a.to_string());

    assert("999999999999999999999999999999" == (Lab01::BigInt("1000000000000000000000000009999") - 10000).to_string());
    assert("9999" == (10000 - Lab01::BigInt("1")).to_string());

    std::cout << "minus_operator test passed" << std::endl;
}

#endif // TEST_MINUS_OPERATOR

#ifdef TEST_INCREMENT_OPERATOR

void test_increment_operator() {
    Lab01::BigInt a("999999999999998");
    Lab01::BigInt b = ++a;
    assert("999999999999999" == a.to_string());
    assert("999999999999999" == b.to_string());
    b = a++;
    assert("999999999999999" == b.to_string());
    assert("1000000000000000" == a.to_string());
    (a++) = 10;
    assert("1000000000000001" == a.to_string());

    std::cout << "increment_operator test passed" << std::endl;
}

#endif // TEST_INCREMENT_OPERATOR

#ifdef TEST_DECREMENT_OPERATOR

void test_decrement_operator() {
    Lab01::BigInt a("1000000000000001");
    Lab01::BigInt b = --a;
    assert("1000000000000000" == a.to_string());
    assert("1000000000000000" == b.to_string());
    b = a--;
    assert("1000000000000000" == b.to_string());
    assert("999999999999999" == a.to_string());
    (a--) = 10;
    assert("999999999999998" == a.to_string());

    std::cout << "decrement_operator test passed" << std::endl;
}

#endif // TEST_DECREMENT_OPERATOR

#ifdef TEST_TO_STRING

bool check_to_string(const std::string& value) {
    return value == Lab01::BigInt(value).to_string();
}

void test_to_string() {
    assert(check_to_string("0"));
    assert(check_to_string("1"));
    assert(check_to_string("10"));
    assert(check_to_string("999"));
    assert(check_to_string("1234567890123456"));
    assert(check_to_string("100000000000000000000000000000000"));
    assert(check_to_string("14759328346542391834675032346958210394540"));

    std::cout << "to_string test passed" << std::endl;
}

#endif // TEST_TO_STRING

#ifdef TEST_INPUT

void test_input() {
    Lab01::BigInt a;

    std::stringstream("1234") >> a;
    assert(1234 == a);
    std::stringstream("438758234729356000000000000001230000000000000000000") >> a;
    assert(Lab01::BigInt("438758234729356000000000000001230000000000000000000") == a);

    std::cout << "input test passed" << std::endl;
}

#endif // TEST_INPUT

#ifdef TEST_OUTPUT

bool check_output(const std::string& value) {
    std::stringstream ss;
    ss << Lab01::BigInt(value);
    return value == ss.str();
}

void test_output() {
    assert(check_output("0"));
    assert(check_output("1"));
    assert(check_output("10"));
    assert(check_output("999"));
    assert(check_output("1234567890123456"));
    assert(check_output("100000000000000000000000000000000"));

    std::cout << "output test passed" << std::endl;
}

#endif // TEST_OUTPUT

#ifdef TEST_ASSIGNMENT_OPERATORS

void test_assignment_operators() {
    const Lab01::BigInt a("1234");
    const Lab01::BigInt b("56");

    assert(a == Lab01::BigInt(a));
    assert(1290 == a + b);
    assert(1178 == a - b);
    assert("1234" == a.to_string());
    assert(!(a == b));
    assert(a != b);
    assert(b < a);
    assert(b <= a);
    assert(a > b);
    assert(a >= b);

    Lab01::BigInt c;
    c += a;
    assert(1234 == c);
    c = a;
    assert(1234 == c);
    c -= b;
    assert(1178 == c);

    std::stringstream ss;
    ss << a << " " << b;
    assert("1234 56" == ss.str());

    std::cout << "assignment_operators test passed" << std::endl;
}

#endif // TEST_ASSIGNMENT_OPERATORS

int main() {
#ifdef TEST_CONSTRUCTOR
    test_constructor();
#endif // TEST_CONSTRUCTOR

#ifdef TEST_EQUAL_OPERATOR
    test_equal_operator();
#endif // TEST_EQUAL_OPERATOR

#ifdef TEST_NOT_EQUAL_OPERATOR
    test_not_equal_operator();
#endif // TEST_NOT_EQUAL_OPERATOR

#ifdef TEST_LESS_OPERATOR
    test_less_operator();
#endif // TEST_LESS_OPERATOR

#ifdef TEST_GREATER_OPERATOR
    test_greater_operator();
#endif // TEST_GREATER_OPERATOR

#ifdef TEST_LESS_OR_EQUAL_OPERATOR
    test_less_or_equal_operator();
#endif // TEST_LESS_OR_EQUAL_OPERATOR

#ifdef TEST_GREATER_OR_EQUAL_OPERATOR
    test_greater_or_equal_operator();
#endif // TEST_GREATER_OR_EQUAL_OPERATOR

#ifdef TEST_PLUS_OPERATOR
    test_plus_operator();
#endif // TEST_PLUS_OPERATOR

#ifdef TEST_MINUS_OPERATOR
    test_minus_operator();
#endif // TEST_MINUS_OPERATOR

#ifdef TEST_INCREMENT_OPERATOR
    test_increment_operator();
#endif // TEST_INCREMENT_OPERATOR

#ifdef TEST_DECREMENT_OPERATOR
    test_decrement_operator();
#endif // TEST_DECREMENT_OPERATOR

#ifdef TEST_TO_STRING
    test_to_string();
#endif // TEST_TO_STRING

#ifdef TEST_INPUT
    test_input();
#endif // TEST_INPUT

#ifdef TEST_OUTPUT
    test_output();
#endif // TEST_OUTPUT

#ifdef TEST_ASSIGNMENT_OPERATORS
    test_assignment_operators();
#endif // TEST_ASSIGNMENT_OPERATORS

    return 0;
}
