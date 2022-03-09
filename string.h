#include <iostream>
#include <cstring>

class String {
private:
    char *body = nullptr;
    size_t len = 0;
    size_t real_len = 0;

    void make_copy(const String &s, const size_t len, const size_t real_len, const size_t begin = 0) {
        body = new char [real_len];
        memcpy(body, s.body + begin, len);
    }
    String(const String &s, const size_t begin, const size_t end): len(end - begin), real_len(len * 2) {
        make_copy(s, len, real_len, begin);
    }
    void delete_body() {
        delete [] body;
    }
    void expand_or_squeeze(const size_t new_len, const bool expand = true, const bool replace = true) {
        if (new_len == real_len) return;
        char *new_body = new char [new_len];
        if (replace)
            memcpy(new_body, body, expand ? real_len : new_len);
        delete_body();
        body = new_body;
        real_len = new_len;
    }
    void expand(const size_t new_len) {
        expand_or_squeeze(new_len, true);
    }
    void squeeze(const size_t new_len) {
        expand_or_squeeze(new_len, false);
    }

public:
    String(const char c = 0) : len((bool)c), real_len(len + 1){
        body = new char [real_len];
        body[0] = c;
    }
    String(const char *s) {
        for (; s[len] != '\0'; len++);
        real_len = len * 2;
        body = new char [real_len];
        strcpy(body, s);
    }
    String(const size_t n, const char c): len(n), real_len(n * 2) {
        body = new char [real_len];
        memset(body, c, len);
    }
    String(const String &s): len(s.len), real_len(len * 2) {
        make_copy(s, len, real_len);
    }
    String &operator= (const String &right) {
        expand_or_squeeze(right.real_len, true, false);
        memcpy(body, right.body, right.real_len);
        len = right.len;
        real_len = right.real_len;
        return *this;
    }
    ~String() {
        delete_body();
    }
    size_t length() const {
        return len;
    }
    char &front() {
        return body[0];
    }
    const char &front() const {
        return body[0];
    }
    char &back() {
        return body[len - 1];
    }
    const char &back() const {
        return body[len - 1];
    }
    bool empty() const {
        return len == 0;
    }
    char &operator[] (const size_t ind){
        return body[ind];
    }
    const char &operator[] (const size_t ind) const {
        return body[ind];
    }
    void clear() {
        squeeze(1);
        len = 0;
    }
    String &operator+= (const String &right) {
        while (len + right.len >= real_len)
            expand(real_len * 2);
        memcpy(body + len, right.body, right.len);
        len += right.len;
        return *this;
    }
    void push_back(const char c) {
        body[len++] = c;
        if (len == real_len)
            expand(len * 2);
    }
    char pop_back() {
        char res = body[--len - 1];
        if (len * 4 <= real_len)
            squeeze(real_len / 2);
        return res;
    }
    String substr(const size_t start, const size_t count) const {
        return String(*this, start, start + count);
    }
    size_t find(const String &s) const {
        if (len < s.len || s.len == 0) return len;
        for (size_t i = 0; i <= len - s.len; ++i) {
            String ss = substr(i, s.len);
            if (ss == s) return i;
        }
        return len;
    }
    size_t rfind(const String &s) const {
        if (len < s.len || s.len == 0) return len;
        for (size_t i = len; i >= s.len; --i) {
            String ss = substr(i - s.len, s.len);
            if (ss == s) return i - s.len;
        }
        return len;
    }
    friend bool operator== (const String &left, const String &right);
    friend std::ostream &operator<< (std::ostream &out, const String &s);
    friend std::ostream &operator>> (std::ostream &out, String &s);
};

bool operator== (const String &left, const String &right) {
    if (left.len != right.len) return false;
    for (size_t i = 0; i < left.len; ++i) {
        if (left.body[i] != right.body[i]) return false;
    }
    return true;
}

String operator+ (const String &left, const String &right) {
    return (String(left) += right);
}

std::ostream &operator<< (std::ostream &out, const String &s) {
    for (size_t i =0; i < s.len; ++i) {
        out << s.body[i];
    }
    return out;
}

std::istream &operator>> (std::istream &in, String &s) {
    char c;
    in >> std::noskipws;
    while (in >> c && c != '\n') {
        s.push_back(c);
    }
    in >> std::skipws;
    return in;
}
