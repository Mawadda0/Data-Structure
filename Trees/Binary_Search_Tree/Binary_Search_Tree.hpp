#ifndef BINARYSEARCHTREE_HPP
#define BINARYSEARCHTREE_HPP

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

// Binary Search Tree: for every node, the values in its left subtree are smaller
// than the node, and the values in its right subtree are greater than or equal to it.
// Equal values go to the right, so duplicates are allowed.
template <typename T>
class BST
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

    Node<T> *insertNode(Node<T> *node, T item)
    {
        if(node == nullptr) return new Node<T>(item);
        if(item < node->Data)
        {
            node->left = insertNode(node->left, item);
        }
        else
        {
            node->right = insertNode(node->right, item);
        }
        return node;
    }

    // The smallest value is the leftmost node.
    T minValue(Node<T> *node) const
    {
        if(node->left == nullptr) return node->Data;
        return minValue(node->left);
    }

    // The largest value is the rightmost node.
    T maxValue(Node<T> *node) const
    {
        if(node->right == nullptr) return node->Data;
        return maxValue(node->right);
    }

    // Removes one node holding `item` and returns the new root of this subtree.
    Node<T> *deleteNode(Node<T> *node, T item)
    {
        if(node == nullptr) return node;
        else if(item < node->Data) node->left = deleteNode(node->left, item);
        else if(node->Data < item) node->right = deleteNode(node->right, item);
        else
        {
            if(node->right == nullptr)
            {
                Node<T> *n = node->left;
                delete node;
                return n;
            }
            else if(node->left == nullptr)
            {
                Node<T> *n = node->right;
                delete node;
                return n;
            }
            else
            {
                // two children: take the smallest value of the right subtree
                T successor = minValue(node->right);
                node->Data = successor;
                node->right = deleteNode(node->right, successor);
            }
        }
        return node;
    }

    bool searchValue(Node<T> *node, T item) const
    {
        if(node == nullptr) return false;
        if(item < node->Data) return searchValue(node->left, item);
        if(node->Data < item) return searchValue(node->right, item);
        return true;
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

public:
    // ---------- Constructors and destructor ----------
    BST()
    {
        root = nullptr;
    }

    BST(const BST &other)
    {
        root = copy(other.root);
    }

    ~BST()
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

    T TreeMin() const
    {
        if(root == nullptr)
        {
            cout << "Tree Is Empty." << nl;
            return T();
        }
        return minValue(root);
    }

    T TreeMax() const
    {
        if(root == nullptr)
        {
            cout << "Tree Is Empty." << nl;
            return T();
        }
        return maxValue(root);
    }

    // ---------- Adding and removing ----------
    void insert(T item)
    {
        root = insertNode(root, item);
    }

    // Removes one node whose value is `item`.
    void Delete(T item)
    {
        if(root == nullptr)
        {
            cout << "Tree Is Empty." << nl;
            return;
        }
        if(!searchValue(root, item))
        {
            cout << "Value Isn't Found." << nl;
            return;
        }
        root = deleteNode(root, item);
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
    BST & operator=(const BST &other)
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