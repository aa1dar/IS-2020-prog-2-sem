#ifndef C___CIRCULARBUFFER_HPP
#define C___CIRCULARBUFFER_HPP

#include <iostream>
#include <exception>
#include <iterator>


using namespace std;


template<class T>
class Iterator : std::iterator<random_access_iterator_tag, T> {
private:
    T *data_;
    int size_;
    int cur_;
    int id_;
    int capacity_;

public:
    using typename std::iterator<random_access_iterator_tag, T>::iterator_category;
    using typename std::iterator<random_access_iterator_tag, T>::value_type;
    using typename std::iterator<random_access_iterator_tag, T>::difference_type;
    using typename std::iterator<random_access_iterator_tag, T>::pointer;
    using typename std::iterator<random_access_iterator_tag, T>::reference;

    Iterator() {};

    Iterator(T *data, int size, int cur, int id, int capacity) :
            data_(data), size_(size), cur_(cur), id_(id), capacity_(capacity) {};

    Iterator(const Iterator&) = default;

    Iterator &operator=(const Iterator &iter) {
        if (this == &iter) {
            return *this;
        }
        data_ = iter.data_;
        size_ = iter.size_;
        cur_ = iter.cur_;
        id_ = iter.id_;
        capacity_ = iter.capacity_;
        return *this;
    };

    ~Iterator() = default;

    Iterator &operator++() {
        cur_ = (cur_ + 1) % capacity_;
        if (id_ != size_)
            id_++;
        return *this;
    }

    Iterator operator++(T) {
        auto old = *this;
        ++(*this);
        return old;
    }

    reference operator*() const {
        return data_[cur_];
    }

    pointer operator->() const;

    Iterator &operator--() {
        cur_ = (capacity_ + cur_ - 1) % capacity_;
        if (id_ != 0)
            id_--;
        return *this;
    }

    Iterator operator--(int) {
        auto old = *this;
        --(*this);
        return old;
    }

    reference operator[](difference_type n) const {
        auto tmp = *this;
        tmp += n;
        return *tmp;
    }

    Iterator &operator+=(difference_type n) {
        cur_ = (cur_ + capacity_ + n) % capacity_;
        id_ += n;
        return *this;
    }

    Iterator &operator-=(difference_type n) {
        return (*this += -n);
    }

    int current() const {
        return cur_;
    }

    int checkId() const {
        return id_;
    }

    void changeId(int val) {
        id_ = val;
    }

    void changeCur(int val) {
        cur_ = val;
    }

    void changeSize(int val) {
        size_ = val;
    }


};

template<class T>
void swap(Iterator<T> &a, Iterator<T> &b) {
    int tmp1 = b.current();
    int tmp2 = b.checkId();
    b.changeCur(a.current());
    b.changeId(a.checkId());
    a.changeCur(tmp1);
    a.changeId(tmp2);
}

template<class T>
bool operator==(const Iterator<T> &lhs, const Iterator<T> &rhs) {
    return (lhs.current() == rhs.current()) && (lhs.checkId() == rhs.checkId());
}

template<class T>
bool operator!=(const Iterator<T> &lhs, const Iterator<T> &rhs) {
    return !(lhs == rhs);
}

template<class T>
bool operator<(const Iterator<T> &lhs, const Iterator<T> &rhs) {
    return (rhs.checkId() - lhs.checkId()) > 0;
}

template<class T>
bool operator>(const Iterator<T> &lhs, const Iterator<T> &rhs) {
    return rhs < lhs;
}

template<class T>
bool operator<=(const Iterator<T> &lhs, const Iterator<T> &rhs) {
    return !(rhs > lhs);
}


template<class T>
bool operator>=(const Iterator<T> &lhs, const Iterator<T> &rhs) {
    return !(rhs < lhs);
}

template<class T>
Iterator<T> operator+(Iterator<T> it, typename Iterator<T>::difference_type n) {
    it += n;
    return it;
}

template<class T>
Iterator<T> operator+(typename Iterator<T>::difference_type n, Iterator<T> it) {
    return it + n;
}

template<class T>
Iterator<T> operator-(Iterator<T> it, typename Iterator<T>::difference_type n) {
    it -= n;
    return it;
}

template<class T>
typename Iterator<T>::difference_type operator-(const Iterator<T> &lhs, const Iterator<T> &rhs) {
    return lhs.checkId() - rhs.checkId();
}


template<class T>
class CircularBuffer {
public:
    CircularBuffer();

    CircularBuffer(int capacity = 16) {
        capacity_ = capacity;
        data = new T[capacity];
        size_ = 0;
        begin_ = Iterator<T>(data, size_, 0, 0, capacity_);
        end_ = Iterator<T>(data, size_, 0, 0, capacity_);


    }

    ~CircularBuffer() = default;

    void addLast(T val) {
        if (!size_) {
            data[0] = val;
            start = 0;
            first_id = 0;
            last_id = 0;
            size_++;
            end_.changeId(1);
            end_.changeCur(1);
            begin_.changeId(0);
            begin_.changeCur(0);
            begin_.changeSize(size_);
            end_.changeSize(size_);
            return;
        }
        if (size_ != capacity_)
            size_++;

        begin_.changeSize(size_);
        end_.changeSize(size_);
        ++end_;
        last_id = (last_id + 1) % capacity_;
        data[last_id] = val;

    }

    void addFirst(T val) {
        if (!size_) {
            data[0] = val;
            start = 0;
            first_id = 0;
            last_id = 0;
            size_++;
            begin_.changeId(0);
            begin_.changeCur(0);
            end_.changeId(1);
            end_.changeCur(1);
            begin_.changeSize(size_);
            end_.changeSize(size_);

            return;
        }
        if (size_ != capacity_)
            size_++;
        start = (start + capacity_ - 1) % capacity_;
        first_id = (capacity_ + first_id - 1) % capacity_;
        if (first_id == last_id && size_ == capacity_) {
            end_--;
            last_id = (last_id - 1 + capacity_) % capacity_;
            end_.changeId(size_);
        } else {
            int tmp = end_.checkId();
            if (tmp != capacity_)
                end_.changeId(++tmp);
        }
        --begin_;
        begin_.changeSize(size_);
        end_.changeSize(size_);

        data[first_id] = val;


    }

    void delFirst() {
        if (!size_)
            return;
        if (size_ != 1) {
            ++begin_;
            start = (start + 1) % capacity_;
            first_id = (first_id + 1) % capacity_;
        }
        size_--;
        begin_.changeSize(size_);
        end_.changeSize(size_);

        int tmp = end_.checkId();
        end_.changeId(--tmp);


    }

    void delLast() {
        if (!size_)
            return;
        if (size_ != 1) {
            last_id = (last_id + capacity_ - 1) % capacity_;
            --end_;
        }
        size_--;
        begin_.changeSize(size_);
        end_.changeSize(size_);


    }


    const T &first() const {
        return data[first_id];
    }

    const T &last() const {
        return data[last_id];
    }

    T &operator[](int id) const {
        if (id >= size_ || id < 0)
            throw runtime_error("Error: index out of the range, index = " + std::to_string(id) + ", size = " +
                                std::to_string(size_));
        int new_id = (start + id) % capacity_;
        return data[new_id];
    }

    T &operator[](int id) {
        if (id >= size_ || id < 0)
            throw runtime_error("Error: index out of the range, index = " + std::to_string(id) + ", size = " +
                                std::to_string(size_));
        int new_id = (start + id) % capacity_;
        return data[new_id];
    }




    Iterator<T> &begin() {
        return begin_;
    }

    Iterator<T> begin() const {
        return begin_;
    }

    Iterator<T> &end() {
        return end_;
    }

    Iterator<T> end() const {
        return end_;
    }

    void changeCapacity(int new_capacity) {
        T *new_data = new T[new_capacity];
        int i = 0;
        for (auto iter = begin(); iter != end(); iter++) {
            if (i < new_capacity)
                new_data[i++] = *iter;
        }
        start = 0;
        first_id = 0;
        size_ = std::min(size_, new_capacity);
        last_id = size_ - 1;
        capacity_ = new_capacity;
        delete[] data;
        data = new_data;
        begin_ = Iterator<T>(data, size_, 0, 0, capacity_);
        int current = (size_ % capacity_);
        end_ = Iterator<T>(data, size_, current, size_, capacity_);


    }


private:
    int size_;
    int capacity_;
    T *data;
    Iterator<T> begin_;
    Iterator<T> end_;
    int first_id;
    int last_id;
    int start;


};

#endif //C___CIRCULARBUFFER_HPP
