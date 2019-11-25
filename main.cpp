#include <iostream>

using namespace std;

enum nodeColor {
    BLACK, RED
};

template<class T>
class RBTree {
    struct Node {
        Node *left;
        Node *right;
        Node *parent;
        nodeColor color;
        T data;
        int countOfElem = 0;
    };
    Node *root;

#define NIL &leaf
    Node leaf = {nullptr, nullptr, nullptr, BLACK, 0};

    Node *createNode(Node *parent, T key) {
        Node *node = new Node;
        node->data = key;
        node->parent = parent;
        node->left = NIL;
        node->right = NIL;
        node->color = RED;
        node->countOfElem = 1;
        return node;
    }

protected:
    void Transplant(Node *u, Node *v) {
        if (u->parent == nullptr) {
            root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    void rotateLeft(Node *x) {
        Node *right = x->right;
        x->right = right->left;
        if (right != NIL) right->parent = x;
        right->parent = x->parent;
        if (x->parent == NIL) root = right;
        else if (x == x->parent->left) x->parent->left = right;
        else x->parent->right = right;
        right->left = x;
        x->parent = right;
    }

    void rotateRight(Node *x) {
        Node *left = x->left;
        x->left = left->right;
        if (x->left != NIL) x->left->parent = x;
        left->parent = x->parent;
        if (x->parent == NIL) root = left;
        else if (x == x->parent->left) x->parent->left = left;
        else x->parent->right = left;
        left->right = x;
        x->parent = left;
    }

    void fixIns(Node *x) {
        Node *parent;
        Node *grand;
        while ((x != root) && (x->color != BLACK) && (x->parent->color == RED)) {
            parent = x->parent;
            grand = x->parent->parent;
            if (parent == grand->left) {
                Node *uncle = grand->right;
                if (uncle != NIL && uncle->color == RED) {
                    grand->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    x = grand;
                } else {
                    if (x == parent->right) {
                        rotateLeft(parent);
                        x = parent;
                        parent = x->parent;
                    }
                    rotateRight(grand);
                    swap(parent->color, grand->color);
                    x = parent;
                }
            } else {
                Node *uncle = grand->left;
                if ((uncle != NIL) && (uncle->color == RED)) {
                    grand->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    x = grand;
                }
                rotateLeft(grand);
                swap(parent->color, grand->color);
                x = parent;
            }
        }
        root->color = BLACK;
    }

    void fixDel(Node *x) {
        while (x->color == BLACK && x != root) {
            Node *brother;
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
    RBTree() {
        root = NIL;
    };

    void destroy(Node *node) {
        if (node != NIL) {
            destroy(node->left);
            destroy(node->right);
            delete node;
        }
    }

    ~RBTree() {
        destroy(root);
    }

    void insert(T key) {
        Node *r = root;
        Node *curr = NIL;
        while (r != NIL) {
            curr = r;
            if (key < r->data) {
                r = r->left;
            } else if (key > r->data) {
                r = r->right;
            } else {
                r->countOfElem++;
                return;
            }
        }
        Node *newNode = createNode(curr, key);
        if (curr == NIL) {
            root = newNode;
        } else if (newNode->data < curr->data) {
            curr->left = newNode;
        } else {
            curr->right = newNode;
        }
        fixIns(newNode);
    }

    Node *search(T key) {
        Node *temp = root;
        bool end = false;
        while (end != true) {
            if (key == temp->data) {
                end = true;
            } else {
                if (key > temp->data) {
                    if (temp->right == NIL) {
                        end = true;
                        temp = nullptr;
                    } else temp = temp->right;
                } else {
                    if (temp->left == NIL) {
                        end = true;
                        temp = nullptr;
                    } else temp = temp->left;
                }
            }
        }
        return temp;
    }

    void deleteByKey(T key) {
        if (root == NIL) {
            cout << "Tree is empty";
        } else {
            Node *delNode = search(key);
            if (delNode != nullptr) {
                if (delNode->countOfElem == 1) {
                    deleteNode(delNode);
                } else {
                    delNode->countOfElem--;
                }
            } else {
                cout << "Key didn't find";
            }
        }
    }

    void deleteNode(Node *del) {
        if (del->left == NIL && del->right == NIL) {
            if (del == root) {
                root = NIL;
            } else {
                Node *parent = del->parent;
                if (parent->left == del) parent->left = NIL;
                else parent->right = NIL;
            }
            return;
        }
        Node *child; //x
        Node *current = del; // y
        nodeColor color = current->color; //c
        if (del->right == NIL || del->left == NIL) {
            if (del->right == NIL) {
                child = del->left;
                Transplant(del, del->left);
            } else {
                child = del->right;
                Transplant(del, del->right);
            }
        } else {
            Node *temp = del->right;
            while (temp->left != NIL) temp = temp->left;
            current = temp;
            color = current->color;
            child = current->right;
            if (current->parent == del) {
                child->parent = current;
            } else {
                Transplant(current, current->right);
                current->right = del->right;
                current->right->parent = current;
            }
            Transplant(del, current);
            current->left = del->left;
            current->left->parent = current;
            current->color = del->color;
        }
        if (color == BLACK) {
            fixDel(child);
            delete del;
        }
    }

    void print(Node *x) {
        if (x != NIL) {
            print(x->left);
            cout << x->data;
            if (x->color == RED) {
                cout << "R|c:" << x->countOfElem << " ";
            } else {
                cout << "B|c:" << x->countOfElem << " ";
            }
            print(x->right);
        }
    }

    Node *getRoot() {
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