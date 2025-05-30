// Str_Vec.h
#pragma once
#ifndef GUARD_Str
#define GUARD_Str

#include <algorithm>
#include <iterator>
#include <cstring>

#include "Vec.h"

class Str {
    // 입력 연산자
    friend std::istream& operator>>(std::istream&, Str&);

private:
    Vec<char> data;

public:
    typedef Vec<char>::size_type size_type;
    typedef char* iterator; // Q12
    typedef const char* const_iterator; // Q12

    // 비어 있는 Str 객체를 만드는 기본 생성자
    Str() { } //초기값 추가
 
    // c의 복사본 n개가 있는 Str 객체를 생성
    Str(size_type n, char c) : data(n, c) { }

    // null로 끝나는 char 타입의 배열에서 Str 객체를 생성
    Str(const char* cp) {
        std::copy(cp, cp + std::strlen(cp), std::back_inserter(data));
    }

    // 반복자 b와 e가 가리키는 범위에서 Str 객체를 생성
    template<class In> Str(In b, In e) {
        std::copy(b, e, std::back_inserter(data));
    }

    // 인덱스 연산자
    char& operator[](size_type i) { return data[i]; }
    const char& operator[](size_type i) const { return data[i]; }

    // 길이(크기) 함수
    size_type size() const { return data.size(); }

    // 결합 연산자
    Str& operator+=(const Str& s) {
        std::copy(s.data.begin(), s.data.end(),
            std::back_inserter(data));
        return *this;
    }

     // Q12 begin(), end(), insert()
    iterator begin() { return data.begin(); }
    iterator end() { return data.end(); }
    iterator insert(iterator d, iterator b, iterator e) {
        return data.insert(d, b, e);
    }
};

std::ostream& operator<<(std::ostream&, const Str&);
Str operator+(const Str&, const Str&);

#endif 
