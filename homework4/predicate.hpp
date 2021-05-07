#ifndef C___PREDICATE_HPP
#define C___PREDICATE_HPP

#include <iostream>

//FIXED S P A C E S

template<class TInputIterator, class TPredicate>
bool allOf(TInputIterator begin, TInputIterator end, TPredicate func) {
    for (; begin != end; begin++) {
        if (!func(*begin)) {
            return false;
        }

    }

    return true;
}

template<class TInputIterator, class TPredicate>
bool anyOf(TInputIterator begin, TInputIterator end, TPredicate func) {
    for (; begin != end; begin++) {
        if (func(*begin)) {
            return true;
        }

    }

    return false;
}


template<class TInputIterator, class TPredicate>
bool noneOf(TInputIterator begin, TInputIterator end, TPredicate func) {
    for (; begin != end; begin++) {
        if (func(*begin)) {
            return false;
        }

    }

    return true;
}

template<class TInputIterator, class TPredicate>
bool oneOf(TInputIterator begin, TInputIterator end, TPredicate func) {
    bool flag = false;
    for (; begin != end; begin++) {
        if (func(*begin)) {
            if (flag)
                return false;
            else
                flag = true;
        }

    }

    return true;
}

//FIXED use default argument parameter
template<class TForwardIterator, class TCompare = std::less<>>
bool isSorted(TForwardIterator begin, TForwardIterator end, TCompare comp = TCompare()) {


    if (begin != end) {
        TForwardIterator second = begin;
        while (++second != end) {
            if (comp(*second, *begin))
                return false;
            begin = second;
        }


    }
    return true;


}




template<class TInputIterator, class TPredicate>
bool isPartitioned(TInputIterator begin, TInputIterator end, TPredicate func) {
    bool flag = false;

    for (; begin != end; begin++) {
        if (!func(*begin) && !flag)
            flag = true;
        if (flag && func(*begin))
            return false;
    }

    return true;
}

template<class TInputIterator, class T>
TInputIterator findNot(TInputIterator begin, TInputIterator end, const T &value) {
    for (; begin != end; begin++) {
        if (*begin != value)
            return begin;
    }
    return end;
}


template<class TForwardIterator, class T>
TForwardIterator findBackward(TForwardIterator begin, TForwardIterator end, const T &value) {
    TForwardIterator result = end;
    for (; begin != end; begin++) {
        if (*begin == value)
            result = begin;
    }

    return result;
}

template<class TForwardIterator, class TPredicate>
bool isPalindrome(TForwardIterator begin, TForwardIterator end, TPredicate func) {
    TForwardIterator first = begin;
    end--;
    while (first != end) {
        if (!func(*begin, *end))
            return false;
        begin++;
        end--;
    }
    return true;
}


#endif //C___PREDICATE_HPP
