#pragma
// Vec.h

#ifndef  GUARD_Vec_H
#define  GUARD_Vec_H

#include <algorithm> // max �Լ�
#include <cstddef> // size_t, ptrdiff_t
#include <memory> // �޸� allocator(�Ҵ���)

using namespace std;

// Vec Ŭ���� �߻� ������ Ÿ��
template<class T> class Vec {
public:
    // �������̽�

    // Ÿ�� ���ǹ�
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef size_t size_type;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T& reference;
    typedef const T& const_reference;

    // ������
    Vec() { create(); }
    explicit Vec(size_type n, const T& val = T())
    {
        create(n, val);
    }

    // ���� ������
    Vec(const Vec& v) { create(v.begin(), v.end()); }
    Vec& operator=(const Vec&); // ����(���߿� �ۼ�)
    ~Vec() { uncreate(); } // �Ҹ���

    // ���� ũ��� �ε����� ���� ���ο� �Լ���
    size_type size() const { return avail - data; }
    T& operator[](size_type i) { return data[i]; }
    const T& operator[](size_type i) const { return data[i]; }

    template<class T> Vec(In b, In e) {
        create(b, e);
    } // Q10

    // �ݺ��ڸ� ��ȯ�ϴ� ���ο� �Լ�
    iterator begin() { return data; }
    const_iterator begin() const { return data; }

    iterator end() { return avail; }
    const_iterator end() const { return avail; }

    template<class In> iterator insert(iterator, In, In); //Q12

    void assign(iterator, iterator); // Q13

    // ��� �߰��ϴ� �Լ�(�޸𸮸� ä���, Ȯ���մϴ�)
    void push_back(const T& val) {
        if (avail == limit) { // �ʿ��Ѵٸ� ���� ���� Ȯ��
            grow();
        }
        unchecked_append(val); // ���ο� ��� �߰�
    }

    // �߰� �Լ� (Student_info Ŭ���� vector~~���� �� �� �Լ� ���)
    void clear() { uncreate(); }
    bool empty() const{ return data == avail; }

    // erase() X2
    iterator erase(iterator);
    iterator erase(iterator, iterator);

private:
    iterator data; // Vec�� ù ��° ��Ҹ� ����Ű�� ������
    iterator avail; // ���������� ���� ����� ������ ����Ű�� ������
    iterator limit; // ����� �� �ִ� ������ ����� ������ ����Ű�� ������

    allocator<T> alloc; // �޸𸮸� �Ҵ��ϴ� ��ü

    // ��� �Լ� ����
    void create();
    void create(size_type, const T&);
    void create(const_iterator, const_iterator);
    template<class In> void create(In, In); // Q14

    void uncreate();

    void grow();
    void unchecked_append(const T&);
};

// =�ǿ����� ���ǹ�
template<class T> Vec<T>& Vec<T>::operator=(const Vec& rhs) {
    // �ڰ� �Ҵ� ���� Ȯ��
    if (&rhs != this) {
        // �º��� ���� �迭�� �����ϴ� �޸𸮸� ����
        uncreate();

        //�캯�� ���� ��Ҹ� �º����� ����
        create(rhs.begin(), rhs.end());
    }
    return *this;
}

// ������ ��� �Լ��� (3��)
// 1. �޸𸮸� �Ҵ��ϰ� ��Ҹ� �ʱ�ȭ�մϴ�.
template<class T> void Vec<T>::create() {
    data = avail = limit = 0;
}

// 2. �پ��� ������ �����ϸ�, �� �Լ��� 
// �Ҵ�� �޸𸮸� �ʱ�ȭ�ϰ�
// �����͸� ������ �����մϴ�.
template<class T> void Vec<T>::create(size_type n, const T& val) {
    data = alloc.allocate(n);
    limit = avail = data + n;
    uninitialized_fill(data, limit, val);
}

// 3. limit�� avail�� �׻� ���� �޸� �ּҸ� 
// ����Ű�� �˴ϴ�.
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

// �Ҹ��� ��� �Լ�
template<class T> void Vec<T>::uncreate() {
    if (data) {
        // ������ ��ҵ��� �������� �Ҹ�
        iterator it = avail; // ������ ����� ����Ű�� ������
        while (it != data) {
            alloc.destroy(--it);
        }

        // �Ҵ�� ��� ���� ������ ��ȯ
        alloc.deallocate(data, limit - data);
    }
    // ��� �ִ� Vec�� ����Ű���� �����͸� �缳��
    data = avail = limit = 0;
}

// �޸� ������ �ø��� �Լ�
template<class T> void Vec<T>::grow() {
    // ��������� �ø� ���� ���� ����ϴ�
    // ��������� 2��� �Ҵ�
    size_type new_size = max(2 * (limit - data), ptrdiff_t(1));

    // ���ο� ���� ������ �Ҵ��ϰ� ���� ��ҵ��� ���ο� ���� ������ ����
    iterator new_data = alloc.allocate(new_size); // �޸� Ȯ���ϱ�
    iterator new_avail = uninitialized_copy(data, avail, new_data);

    // ������ ���� ������ ����
    uncreate();

    // ���Ӱ� �Ҵ��� ���� ������ ����Ű���� ������ �缳��
    data = new_data;
    avail = new_avail;
    limit = data + new_size;
}

// ���ο� ��Ҹ� �߰��ϱ�
template<class T> void Vec<T>::unchecked_append(const T& val) {
    alloc.construct(avail++, val);
}

// Q12
template<class T>
template<class In>
typename Vec<T>::iterator Vec<T>::insert(iterator d, In b, In e) {
    // �� �κа� ���� ����
    Vec<T> end(d, avail);
    ptrdiff_t diff = d - data;

    // �� �κ� �Ҹ��ϱ�
    if (d) {
        // ���������� ��� ��Ҹ� ����
        while (avail != d)
            alloc.destroy(--avail);
    }

    // b���� e���� ��� ��Ҹ� �߰��ϱ�
    while (b != e)
        push + back(*b++);

    // �ٽ� �� ������ �߰��ϱ�
    for (Vec<T>::const_iterator it = end.begin();
        it != end.end(); ++it)
        push_back(*it);

    return data + diff;
}

// assign()
template<class T>
void Vec<T>::assign(iterator b, iterator e) {
    // Vec �Ҹ�
    uncreate();

    // ��� �ݺ����� ��Ҹ� �߰��ϱ�
    while (b != e)
        push_back(*b++);
}

// erase() X2
template<class T>
typename Vec<T>::iterator Vec<T>::erase(iterator pos) {
    // ���ο� Vec ����
    Vec<T> new_vec;
    iterator ret;

    // ù ��� �� ��� ��Ҹ� �߰�
    for (iterator it = begin(); it != end(); ++it) {
        if (it != pos)
            new_vec.push_back(*it);
        else
            ret = new_vec.end();
    }

    // ���� �Ŀ� ����Ű�� ������ ��ȯ
    return ret;
}

template<class T>
typename Vec<T>::iterator Vec<T>::erase(iterator b, iterator e) {
    // ���ο� Vec ����
    Vec<T> new_vec;
    iterator ret;

    // ù ��� �� ��� ��Ҹ� �߰�
    for (iterator it = begin(); it != end(); ++it) {
        if (it < b && it >= e)
            new_vec.push_back(*it);
        else
            ret = new_vec.end();
    }

    // ���� �Ŀ� ����Ű�� ������ ��ȯ
    return ret;
}

#endif