#pragma
// Vec.h

#ifndef  GUARD_Vec_H
#define  GUARD_Vec_H

#include <algorithm> // max 함수
#include <cstddef> // size_t, ptrdiff_t
#include <memory> // 메모리 allocator(할당자)

using namespace std;

// Vec 클래스 추상 데이터 타입
template<class T> class Vec {
public:
    // 인터페이스

    // 타입 정의문
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef size_t size_type;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T& reference;
    typedef const T& const_reference;

    // 생성자
    Vec() { create(); }
    explicit Vec(size_type n, const T& val = T())
    {
        create(n, val);
    }

    // 복사 생성자
    Vec(const Vec& v) { create(v.begin(), v.end()); }
    Vec& operator=(const Vec&); // 선언문(나중에 작성)
    ~Vec() { uncreate(); } // 소멸자

    // 벡터 크기와 인덱스에 관한 새로운 함수들
    size_type size() const { return avail - data; }
    T& operator[](size_type i) { return data[i]; }
    const T& operator[](size_type i) const { return data[i]; }

    template<class T> Vec(In b, In e) {
        create(b, e);
    } // Q10

    // 반복자를 반환하는 새로운 함수
    iterator begin() { return data; }
    const_iterator begin() const { return data; }

    iterator end() { return avail; }
    const_iterator end() const { return avail; }

    template<class In> iterator insert(iterator, In, In); //Q12

    void assign(iterator, iterator); // Q13

    // 요소 추가하는 함수(메모리를 채우면, 확보합니다)
    void push_back(const T& val) {
        if (avail == limit) { // 필요한다면 저장 공간 확보
            grow();
        }
        unchecked_append(val); // 새로운 요소 추가
    }

    // 추가 함수 (Student_info 클래스 vector~~에서 이 두 함수 사용)
    void clear() { uncreate(); }
    bool empty() const{ return data == avail; }

    // erase() X2
    iterator erase(iterator);
    iterator erase(iterator, iterator);

private:
    iterator data; // Vec의 첫 번째 요소를 가리키는 포인터
    iterator avail; // 마지막으로 만든 요소의 다음을 가리키는 포인터
    iterator limit; // 사용할 수 있는 마지막 요소의 다음을 가리키는 포인터

    allocator<T> alloc; // 메모리를 할당하는 객체

    // 멤버 함수 선언문
    void create();
    void create(size_type, const T&);
    void create(const_iterator, const_iterator);
    template<class In> void create(In, In); // Q14

    void uncreate();

    void grow();
    void unchecked_append(const T&);
};

// =피연산자 정의문
template<class T> Vec<T>& Vec<T>::operator=(const Vec& rhs) {
    // 자가 할당 여부 확인
    if (&rhs != this) {
        // 좌변이 지닌 배열이 점유하는 메모리를 해제
        uncreate();

        //우변이 지닌 요소를 좌변으로 복사
        create(rhs.begin(), rhs.end());
    }
    return *this;
}

// 생성자 멤버 함수들 (3개)
// 1. 메모리를 할당하고 요소를 초기화합니다.
template<class T> void Vec<T>::create() {
    data = avail = limit = 0;
}

// 2. 다양한 버전이 존재하며, 각 함수는 
// 할당된 메모리를 초기화하고
// 포인터를 적절히 설정합니다.
template<class T> void Vec<T>::create(size_type n, const T& val) {
    data = alloc.allocate(n);
    limit = avail = data + n;
    uninitialized_fill(data, limit, val);
}

// 3. limit와 avail은 항상 같은 메모리 주소를 
// 가리키게 됩니다.
template<class T> void Vec<T>::create(const_iterator i, const_iterator j) {
    data = alloc.allocate(j - i);
    limit = avail = uninitialized_copy(i, j, data);
}

// Q14
template<class T>
template<class In>
void Vec<T>::create(In i, In j) {
    data = alloc.allocate(j - i);
    limit = avail = uninitialized_copy(i, j, data);
}

// 소멸자 멤버 함수
template<class T> void Vec<T>::uncreate() {
    if (data) {
        // 생성된 요소들을 역순으로 소멸
        iterator it = avail; // 마지막 요소의 가리키는 포인터
        while (it != data) {
            alloc.destroy(--it);
        }

        // 할당된 모든 저장 공간을 반환
        alloc.deallocate(data, limit - data);
    }
    // 비어 있는 Vec을 가리키도록 포인터를 재설정
    data = avail = limit = 0;
}

// 메모리 공간을 늘리는 함수
template<class T> void Vec<T>::grow() {
    // 저장공간을 늘릴 때는 현재 사용하는
    // 저장공간의 2배로 할당
    size_type new_size = max(2 * (limit - data), ptrdiff_t(1));

    // 새로운 저장 공간을 할당하고 기존 요소들을 새로운 저장 공간에 복사
    iterator new_data = alloc.allocate(new_size); // 메모리 확보하기
    iterator new_avail = uninitialized_copy(data, avail, new_data);

    // 기존의 저장 공간을 해제
    uncreate();

    // 새롭게 할당한 저장 공간을 가리키려고 포인터 재설정
    data = new_data;
    avail = new_avail;
    limit = data + new_size;
}

// 새로운 요소를 추가하기
template<class T> void Vec<T>::unchecked_append(const T& val) {
    alloc.construct(avail++, val);
}

// Q12
template<class T>
template<class In>
typename Vec<T>::iterator Vec<T>::insert(iterator d, In b, In e) {
    // 끝 부분과 차이 저장
    Vec<T> end(d, avail);
    ptrdiff_t diff = d - data;

    // 끝 부분 소멸하기
    if (d) {
        // 역방향으로 모든 요소를 제거
        while (avail != d)
            alloc.destroy(--avail);
    }

    // b부터 e까지 모든 요소를 추가하기
    while (b != e)
        push + back(*b++);

    // 다시 끝 포인터 추가하기
    for (Vec<T>::const_iterator it = end.begin();
        it != end.end(); ++it)
        push_back(*it);

    return data + diff;
}

// assign()
template<class T>
void Vec<T>::assign(iterator b, iterator e) {
    // Vec 소멸
    uncreate();

    // 모든 반복자의 요소를 추가하기
    while (b != e)
        push_back(*b++);
}

// erase() X2
template<class T>
typename Vec<T>::iterator Vec<T>::erase(iterator pos) {
    // 새로운 Vec 생성
    Vec<T> new_vec;
    iterator ret;

    // 첫 요소 외 모든 요소를 추가
    for (iterator it = begin(); it != end(); ++it) {
        if (it != pos)
            new_vec.push_back(*it);
        else
            ret = new_vec.end();
    }

    // 삭제 후에 가리키는 포인터 반환
    return ret;
}

template<class T>
typename Vec<T>::iterator Vec<T>::erase(iterator b, iterator e) {
    // 새로운 Vec 생성
    Vec<T> new_vec;
    iterator ret;

    // 첫 요소 외 모든 요소를 추가
    for (iterator it = begin(); it != end(); ++it) {
        if (it < b && it >= e)
            new_vec.push_back(*it);
        else
            ret = new_vec.end();
    }

    // 삭제 후에 가리키는 포인터 반환
    return ret;
}

#endif