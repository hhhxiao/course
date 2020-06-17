//
// Created by agno3 on 10/20/19.
//

#ifndef BAG_BAG_H
#define BAG_BAG_H

#include <iostream>

struct item {
public:
    int size;
    int num;
    item *left;
    item *right;

    item(int val, int num) : size(val), num(num), left(nullptr), right(nullptr) {}

    explicit item(int val) : size(val), num(1), left(nullptr), right(nullptr) {}


};

class bag {
private:
    item *root;
    int item_num;
public:
    bag() : root(nullptr), item_num(0) {}


private:
    static void add_item(item *&i, int size, int num) {
        if (i == nullptr) {
            i = new item(size, num);
            return;
        } else {
            if (size == i->size) {
                i->num += num;
                return;
            } else if (size < i->size) {
                add_item(i->left, size, num);
            } else if (size > i->size) {
                add_item(i->right, size, num);
            }
        }
    }

    static int remove_item(item *&i, int size, int num) {
        if (i == nullptr)return 0;
        if (size < i->size)
            //delete from left child tree
            return remove_item(i->left, size, num);
        else if (size > i->size)
            //delete from right tree
            return remove_item(i->right, size, num);
        else {
            //delete from current node
            if (num < i->num) {
                //decrease the size
                i->num -= num;
                return num;
            } else {
                //remove the node totally
                if (i->left != nullptr && i->right != nullptr) {
                    //left tree and right tree are not empty
                    i->size = find_min(i->right)->size;
                    i->num = find_min(i->right)->num;
                    return remove_item(i->right, i->size, i->num);
                } else if (i->left == nullptr && i->right == nullptr) {
                    //this is a leaf node
                    i = nullptr;
                    return num;
                } else {//only one node
                    item *node = i->right == nullptr ? i->left : i->right;
                    i->num = node->num;
                    i->size = node->size;
                    i->left = node->left;
                    i->right = node->right;
                    node = nullptr;
                    return num;
                }
            }
        }

    }

    static item *find_max(item *i) {
        if (i == nullptr)return nullptr;
        if (i->right == nullptr)return i;
        return find_max(i->right);
    }

    static item *find_min(item *i) {
        if (i == nullptr)return nullptr;
        if (i->left == nullptr)return i;
        return find_min(i->left);
    }

    static void traversal(item *i, int &current, int max) {
        if (i != nullptr) {
            traversal(i->left, current, max);
            if (i->size <= max && i->size > current)current = i->size;
            traversal(i->right, current, max);
        }
    }


public:
//    static int find(item *i, int max) {
//        if (i == nullptr)return -1;
//        if (i->left == nullptr) {
//            if (max >= i->size)
//                return i->size;
//            return -1;
//        } else {
//            if (max < i->size)return find(i->left, max);
//            else if (max == i->size)return i->size;
//            else {
//                return std::max(i->size, find(i->right, max));
//            }
//        }
//    }

public:
    static void print(item *item) {
        if (item == nullptr)return;
        if (item->num > 0)
            std::cout << "(size:" << item->size << ",num:" << item->num << ") ";
        print(item->left);
        print(item->right);
    }


    int find_better(int max) {
        int current = this->min()->size;
        traversal(this->root, current, max);
        return current;
    }


    void add_item(int size, int num = 1) {
        add_item(this->root, size, num);
        item_num += num;
    }

    int remove_item(int size, int num = 1) {
        int del = remove_item(this->root, size, num);
        this->item_num -= del;
        return del;
    }

    void print() {
        print(this->root);
        std::cout << " size = " << item_num << "\n";
    }

    item *max() {
        return find_max(this->root);
    }

    item *min() {
        return find_min(this->root);
    }

//    int find(int max) {
//        return find(this->root, max);
//    }


    int size() const {
        return item_num;
    }
};

#endif //BAG_BAG_H
