#include <iostream>
#include <cstring>

#include "phone_book.h"

using namespace std;

void test() {
    phone_book phoneBook;
    phone_book::user user;
    std::string name;
    int index;
    std::cout << "phone book test:\nto add a user: a [name] [phone] [address]\n"
                 "to delete a user: d [name]\n"
                 "to find a user: f [name]\n"
                 "to modify a user: m [name] [phone] [address]\n"
                 "to print the phone book: p\n"
                 "to quit: q\n";

    char command;
    do {
        cin >> command;
        switch (command) {
            case 'p':
                std::cout << phoneBook << "\n";
                break;
            case 'a':
                cin >> user;
                phoneBook.add_user(user);
                break;
            case 'd':
                cin >> name;
                user.name = name;
                phoneBook.delete_user(user);
                break;
            case 'f':
                cin >> name;
                user.name = name;
                index = phoneBook.find_user(user);
                cout << phoneBook.index(index) << "\n";
                break;
            case 'm':
                cin >> user;
                phoneBook.modify_user(user);
            default:
                break;
        }
    } while (command != 'q');
}


int main() {
    test();
    return 0;
}