#ifndef BINARYTREE_HPP
#define BINARYTREE_HPP

#include "Template.hpp"

template <typename T>
class Node
{
public:
    T Data;
    Node<T> *left;
    Node<T> *right;

    Node(T val)
    {
        Data = val;
        left = nullptr;
        right = nullptr;
    }
};

// Binary tree (there is no ordering between the values).
// Nodes are placed by a path, for example {'L', 'R'} means: go left, then right.
template <typename T>
class Binary_Tree
{
private:
    Node<T> *root;

    // ---------- Private helpers ----------
    void clear(Node<T> *node)
    {
        if(node == nullptr) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

    Node<T> *copy(Node<T> *node) const
    {
        if(node == nullptr) return nullptr;
        Node<T> *n = new Node<T>(node->Data);
        n->left = copy(node->left);
        n->right = copy(node->right);
        return n;
    }

    // Depth-First traversals
    void inOrder(Node<T> *node) const // left, root, right
    {
        if(node == nullptr) return;
        inOrder(node->left);
        cout << node->Data << " ";
        inOrder(node->right);
    }

    void preOrder(Node<T> *node) const // root, left, right
    {
        if(node == nullptr) return;
        cout << node->Data << " ";
        preOrder(node->left);
        preOrder(node->right);
    }

    void postOrder(Node<T> *node) const // left, right, root
    {
        if(node == nullptr) return;
        postOrder(node->left);
        postOrder(node->right);
        cout << node->Data << " ";
    }

    // Breadth-First traversal
    void levelOrder(Node<T> *node) const
    {
        queue<Node<T> *> nodes;
        nodes.push(node);
        while(!nodes.empty())
        {
            Node<T> *curr = nodes.front();
            nodes.pop();
            cout << curr->Data << " ";

            if(curr->left != nullptr) nodes.push(curr->left);
            if(curr->right != nullptr) nodes.push(curr->right);
        }
    }

    // Measurements
    int height(Node<T> *node) const
    {
        if(node == nullptr) return 0;
        return 1 + max(height(node->left), height(node->right));
    }

    int countNodes(Node<T> *node) const
    {
        if(node == nullptr) return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }

    int countLeaves(Node<T> *node) const
    {
        if(node == nullptr) return 0;
        if(node->left == nullptr && node->right == nullptr) return 1;
        return countLeaves(node->left) + countLeaves(node->right);
    }

    bool searchValue(Node<T> *node, T item) const
    {
        if(node == nullptr) return false;
        if(node->Data == item) return true;
        return searchValue(node->left, item) || searchValue(node->right, item);
    }

public:
    // ---------- Constructors and destructor ----------
    Binary_Tree()
    {
        root = nullptr;
    }

    Binary_Tree(T rootVal)
    {
        root = new Node<T>(rootVal);
    }

    Binary_Tree(const Binary_Tree & other)
    {
        root = copy(other.root);
    }

    ~Binary_Tree()
    {
        clear(root);
        root = nullptr;
    }

    // ---------- Utilities ----------
    bool IsEmpty() const
    {
        return root == nullptr;
    }

    void Clear()
    {
        clear(root);
        root = nullptr;
    }

    // Number of levels (an empty tree has height 0, a single node has height 1).
    int TreeHeight() const
    {
        return height(root);
    }

    int TreeCount() const
    {
        return countNodes(root);
    }

    int LeavesCount() const
    {
        return countLeaves(root);
    }

    bool TreeSearch(T item) const
    {
        return searchValue(root, item);
    }

    // ---------- Building the tree ----------
    // Creates the root, or overwrites its value if it already exists.
    void setRoot(T item)
    {
        if(root == nullptr) root = new Node<T>(item);
        else root->Data = item;
    }

    // Follows the path from the root and sets the value of the last node on it.
    // Every node before the last one must already exist.
    // The last node is created if it is missing, or overwritten if it exists.
    void addByPath(const vector<char> & path, T item)
    {
        if(root == nullptr)
        {
            cout << "Tree Is Empty." << nl;
            return;
        }
        if(path.empty())
        {
            cout << "Invalid Path." << nl;
            return;
        }
        Node<T> *curr = root;
        for(int i = 0; i < sz(path); i++)
        {
            char direction = tolower(path[i]);
            if(direction != 'l' && direction != 'r')
            {
                cout << "Invalid Path." << nl;
                return;
            }
            Node<T> *&next = (direction == 'l') ? curr->left : curr->right;
            if(i == sz(path) - 1)
            {
                if(next == nullptr) next = new Node<T>(item);
                else next->Data = item;
                return;
            }
            if(next == nullptr)
            {
                cout << "Path Doesn't Exist." << nl;
                return;
            }
            curr = next;
        }
    }

    // ---------- Display ----------
    void display_inOrder() const
    {
        if(root == nullptr)
        {
            cout << "Tree Is Empty." << nl;
            return;
        }
        inOrder(root);
        cout << nl;
    }

    void display_preOrder() const
    {
        if(root == nullptr)
        {
            cout << "Tree Is Empty." << nl;
            return;
        }
        preOrder(root);
        cout << nl;
    }

    void display_postOrder() const
    {
        if(root == nullptr)
        {
            cout << "Tree Is Empty." << nl;
            return;
        }
        postOrder(root);
        cout << nl;
    }

    void display_LevelByLevel() const
    {
        if(root == nullptr)
        {
            cout << "Tree Is Empty." << nl;
            return;
        }
        levelOrder(root);
        cout << nl;
    }

    // ---------- Operators ----------
    Binary_Tree & operator=(const Binary_Tree &other)
    {
        if(this != &other)
        {
            Node<T> *newRoot = copy(other.root);
            clear(root);
            root = newRoot;
        }
        return *this;
    }
};

#endif