// Str_Vec.cpp
#include <iostream>
#include <iterator>
#include <cstring>
#include "Str_Vec.h"

// 12.3.2 / 페이지 216
std::istream& operator>>(std::istream& is, Str& s) {
    // 기존의 값 제거
    s.data.clear();

    // 띄워쓰기까지 문자 읽기
    char c;
    while (is.get(c) && isspace(c))
        ; // 아무것도 안 해도 됨

    // 계속 문자가 있으면 다음 띄어쓰기까지 읽기
    if (is) {
        do.s.data.push_back(c);
        while (is.get(c) && !isspace(c))
            ;

        // 띄어쓰기 있으면 다시 스트림으로 보내기
        if (is)
            is.unget();
    }
    return is;
}

// 섹션 12.3.1 / 페이지 216
std::ostream& operator<<(std::ostream& os, const Str& s) {
    for (Str::size_type i = 0; i != s.size(); ++i) {
        os << s[i];
    }
    return os;
}

// 섹션 12.3.3 / 페이지 219
Str operator+(const Str& s, const Str& t) {
    Str r = s;
    r += t;
    return r;
}