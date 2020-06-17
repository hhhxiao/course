//
// Created by agno3 on 10/17/19.
//

#ifndef PHONE_BOOK_PHONEBOOK_H
#define PHONE_BOOK_PHONEBOOK_H


#include <iostream>
#include <string>
#include <ostream>
#include <utility>

const int MAX_LEN = 64;
const int MAX_USER = 1024;

class phone_book {
public:
    enum class INFO_TYPE {
        NAME, TEL, ADDRESS
    };

    struct user {
        std::string name;//姓名是主键
        std::string tel;
        std::string address;

        bool operator==(const user &rhs) const {
            return name == rhs.name;
        }

        friend std::ostream &operator<<(std::ostream &os, const user &user) {
            os << user.name << "," << user.tel << "," << user.address;
            return os;
        }

        friend std::istream &operator>>(std::istream &is, user &u) {
            is >> u.name >> u.tel >> u.address;
            return is;
        }

        user() = default;

        user(std::string n, std::string tel, std::string addr) :
                name(std::move(n)), tel(std::move(tel)), address(std::move(addr)) {}
    };

private:
    user users[MAX_USER];
    int length = 0;


public:
    phone_book() = default;

    int add_user(const user &u) {
        if (length == MAX_USER) {
            std::cerr << "the phone book is full!\n";
            return -1;
        }
        int index = find_user(u);
        if (index != -1) {
            std::cerr << "user has existed!!\n";
            return 1;
        }
        users[length] = u;
        length++;
        return 1;
    }

    int delete_user(const user &u) {
        int index = find_user(u);
        if (index == -1) {
            std::cerr << "user does not exist!\n";
            return -1;
        }
        for (int i = index; i < length - 1; ++i)
            users[i] = users[i + 1];
        --length;
        return 0;
    }


    int find_user(const user &u) {
        for (int i = 0; i < length; ++i)
            if (u == users[i])return i;
        return -1;
    }

    int modify_user(const user &u) {
        int index = find_user(u);
        if (index == -1)return -1;
        users[index].name = u.tel;
        users[index].address = u.address;
        return 0;
    }

    friend std::ostream &operator<<(std::ostream &os, const phone_book &book) {
        os << "length: " << book.length << "\n";
        for (int i = 0; i < book.length - 1; ++i)
            os << book.users[i]<<"\n";
        os<<book.users[book.length-1];
        
        return os;
    }

    user index(int i) {
        if (i >= 0 && i <= length)
            return users[i];
        return user("null", "null", "null");
    }

};


#endif //PHONE_BOOK_PHONEBOOK_H
