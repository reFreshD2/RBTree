#include <iostream>

using namespace std;

enum nodeColor {BLACK, RED};

template< class T>
class RBTree{
    struct node {
        node* left = nullptr;
        node* right = nullptr;
        node* parent = nullptr;
        nodeColor color;
        T data;
        int countOfElem = 0;
    };
    node* root;

protected:
    void rotateLeft(node *x) {
        node *right = x->right;
        x->right = right->left;
        if (right != nullptr) right->parent = x;
        right->parent = x->parent;
        if (x->parent == nullptr) root = right;
        else if (x == x->parent->left) x->parent->left = right;
        else x->parent->right = right;
        right->left = x;
        x->parent = right;
    }
    void rotateRight(node *x) {
       node* left = x->left;
       x->left = left->right;
       if (x->left != nullptr) x->left->parent = x;
       left->parent = x->parent;
       if (x->parent == nullptr) root = left;
       else if (x == x->parent->left) x->parent->left = left;
       else x->parent->right = left;
       left->right = x;
       x->parent = left;
    }
    void fixIns(node* x) {
        node* parent = nullptr;
        node* grand = nullptr;
        while ((x != root) && (x->color!=BLACK) && (x->parent->color == RED)) {
            parent = x->parent;
            grand = x->parent->parent;
            if (parent == grand->left){
                node* uncle = grand->right;
                if (uncle != nullptr && uncle->color == RED){
                    grand->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    x = grand;
                }
                else {
                    if (x == parent->right) {
                        rotateLeft(parent);
                        x = parent;
                        parent = x->parent;
                    }
                    rotateRight(grand);
                    swap(parent->color,grand->color);
                    x = parent;
                }
            }
            else {
               node* uncle = grand->left;
               if ((uncle != nullptr) && (uncle->color == RED)) {
                   grand->color = RED;
                   parent->color = BLACK;
                   uncle->color = BLACK;
                   x = grand;
               }
               rotateLeft(grand);
               swap(parent->color,grand->color);
               x = parent;
            }
        }
        root->color = BLACK;
    }
    void fixDel(node* x) {
            while (x->color == BLACK && x != root) {
                node *brother;
                if (x->parent->left == x) {
                    brother = x->parent->right;
                    if (brother->color == RED) {
                        brother->color = BLACK;
                        x->parent->color = RED;
                        rotateLeft(x->parent);
                    }
                    if (brother->left->color == BLACK && brother->right->color == RED) {
                        brother->color = RED;
                    } else {
                        if (brother->right->color == BLACK) {
                            brother->left->color = BLACK;
                            brother->color = RED;
                            rotateRight(brother);
                        }
                        brother->color = brother->parent->color;
                        brother->parent->color = BLACK;
                        brother->right->color = BLACK;
                        rotateLeft(brother->parent);
                        x = root;
                    }
                } else {
                    brother = x->parent->left;
                    if (brother->color == RED) {
                        brother->color = BLACK;
                        brother->parent->color = RED;
                        rotateRight(x->parent);
                    }
                    if (brother->left->color == BLACK && brother->right->color == RED) {
                        brother->color = RED;
                    } else {
                        if (brother->left->color == BLACK) {
                            brother->right->color = BLACK;
                            brother->color = RED;
                            rotateLeft(brother);
                        }
                        brother->color = brother->parent->color;
                        brother->parent->color = BLACK;
                        brother->left->color = BLACK;
                        rotateRight(brother->parent);
                        x = root;
                    }
                }
            }
            x->color = BLACK;
            root->color = BLACK;
    }
public:
    RBTree(){
        root = nullptr;
    };
    void insert (T key){
    node* t = new node;
    t->data = key;
    t->color = RED;
    t->countOfElem++;
    if (this->root == nullptr) {
        root = t;
        t->parent = nullptr;
    }
    else {
        node* p = root;
        node* q = nullptr;
        while (p != nullptr) {
            q = p;
            if (p->data < t->data) {
                p = p->right;
            } else {
                if (p->data > t->data) {
                    p = p->left;
                } else {
                    p->countOfElem++;
                    return;
                }
            }
        }
        t->parent = q;
        if (q->data < t->data) {
            q->right = t;
        }
        else {
            q->left = t;
        }
    }
    fixIns(t);
    }
    node* search (T key) {
        node* temp = root;
        bool end = false;
        while (end != true) {
            if (key == temp->data){
                end = true;
            } else {
                if (key > temp->data) {
                    if (temp->right == nullptr) {
                        end = true;
                        temp = nullptr;
                    } else temp = temp->right;
                } else {
                    if (temp->left == nullptr) {
                        end = true;
                        temp = nullptr;
                    } else temp = temp->left;
                }
            }
        }
        return temp;
    }
    void deleteByKey (T key) {
        if (root == nullptr) {
            cout << "Tree is empty";
        }
        else {
            node* delNode = search(key);
            if (delNode->countOfElem == 1) {
                deleteNode(delNode);
            }
            else {
                delNode->countOfElem--;
            }
        }
    }
    void deleteNode (node* del) {
        if (del->left == nullptr && del->right == nullptr) {
            if (del == root) {
                root = nullptr;
            }
            else {
                node* parent = del->parent;
                if (parent->left == del) parent->left = nullptr;
                else parent->right = nullptr;
            }
            return;
        }
        node* child;
        if (del->right == nullptr || del->left == nullptr) {
            node* parent = del->parent;
            if (del->left == nullptr) child = del->right;
            else child = del->left;
            if (parent->left == del) parent->left = child;
            else parent->right = child;
            child->parent = parent;
        }
        else {
            node* temp = del->right;
            while (temp->left != nullptr) temp = temp->left;
            del->data = temp->data;
            del->countOfElem = temp->countOfElem;
            deleteNode(temp);
           /* if (temp->right != nullptr) {
                temp->right->parent = temp->parent;
                if (temp->parent->left == temp) temp->parent->left = temp->right;
                temp->parent->right = temp->right;
            }
            else {
                if (temp->parent->left == temp) temp->parent->left = nullptr;
                temp->parent->right = nullptr;
            }*/
            if (del->left == nullptr) child = del->right;
            else child = del->left;
        }
        if (del->color == BLACK) {
            fixDel(child);
            delete del;
        }
    }
    void print(node* x){
        if (x) {
            print(x->left);
            cout << x->data;
            if (x->color == RED) {
                cout << "R|c:" << x->countOfElem << " ";
            }
            else {
                cout << "B|c:" << x->countOfElem << " ";
            }
            print(x->right);
        }
    }
    node* getRoot(){
        return this->root;
    }
};

int main() {
    RBTree<int> tree;
    tree.insert(11);
    tree.insert(1);
    tree.insert(14);
    tree.insert(2);
    tree.insert(7);
    tree.insert(15);
    tree.insert(2);
    tree.print(tree.getRoot());
    cout << endl;
    tree.deleteByKey(14);
    tree.deleteByKey(11);
    tree.deleteByKey(2);
    tree.print(tree.getRoot());
    return 0;
}