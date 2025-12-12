#include <bits/stdc++.h>
using namespace std;

#define nl '\n'
#define ll long long
#define fixed(n) fixed << setprecision(n)
#define sz(x) (int)(x.size())
#define all(v) v.begin(), v.end()

// "سُبْحَانَكَ لا عِلْمَ لَنَا إِلَّا مَا عَلَّمْتَنَا إِنَّكَ أَنْتَ الْعَلِيمُ الْحَكِيمُ"

class Node
{
public:
    int data;
    Node *left;
    Node *right;
    Node(int val)
    {
        data = val;
        left = right = NULL;
    }
};

class Tree
{
private:
    Node *insertP(Node *root, int data)
    {
        if (root == NULL)
            return new Node(data);
        if (data < root->data)
            root->left = insertP(root->left, data);
        else
            root->right = insertP(root->right, data);
        return root;
    }

public:
    Node *root;
    Tree()
    {
        root = NULL;
    }

private:
    Node *insertP(Node *root, int data)
    {
        if (root == NULL)
        {
            return new Node(data);
        }
        if (data < root->data)
        {
            root->left = insertP(root->left, data);
        }
        else
        {
            root->right = insertP(root->right, data); //* Linking
        }
        return root;
    }
    //-----
    void preorderP(Node *root)
    {
        if (root == NULL)
        {
            return;
        }
        cout << root->data << " ";
        preorderP(root->left);
        preorderP(root->right);
    }
    //-----
    void inorderP(Node *root)
    {
        if (root == NULL)
        {
            return;
        }

        inorderP(root->left);
        cout << root->data << " ";
        inorderP(root->right);
    }
    //-----
    void postorderP(Node *root)
    {
        if (root == NULL)
        {
            return;
        }
        postorderP(root->left);
        postorderP(root->right);
        cout << root->data << " ";
    }
    //-----
    int getSizeP(Node *root)
    {
        if (root == NULL)
        {
            return 0;
        }
        return 1 + getSizeP(root->left) + getSizeP(root->right);
    }
    //-----
    int getMaxP(Node *root)
    {
        if (root->right == NULL)
            return root->data;
        return getMaxP(root->right);
    }
    //-----
    int getMinP(Node *root)
    {
        if (root->left == NULL)
            return root->data;
        return getMinP(root->left);
    }
    //-----
    bool searchP(Node *root, int val)
    {
        if (root == NULL)
            return false;
        else if (root->data == val)
            return true;
        else if (root->data < val)
            return searchP(root->right, val);
        else if (root->data > val)
            return searchP(root->left, val);
    }
    //-----
    void clearP(Node *root)
    {
        if (root == NULL)
            return;
        else
        {
            clearP(root->left);
            clearP(root->right);
            delete root;
        }
    }
    //-----
    Node *DeleteP(Node *root, int value)
    {
        if(root == NULL) return root;
        else if(root->data > value) root->left = DeleteP(root->left, value);
        else if(root->data < value) root->right = DeleteP(root->right, value);
        else
        {
            if(root->right == NULL)
            {
                Node *n = root->left;
                delete root;
                return n;
            }
            else if(root->left == nullptr)
            {
                Node *n = root->right;
                delete root;
                return n;
            }
            else
            {
                root->data = getMinP(root->right);
                root->right = DeleteP(root->right, getMinP(root->right));
            }
        }
        return root;
    }
    //----
    int getDepthP(Node *root)
    {
        if (root == NULL || (root->left == NULL && root->right == NULL))
            return 0;
        else
            return max(getDepthP(root->left), getDepthP(root->right)) + 1;
    }

public:
    void insert(int data)
    {
        root = insertP(root, data);
    }
    //-----
    void preorder()
    {
        preorderP(root);
    }
    //-----
    void inorder()
    {
        inorderP(root);
    }
    //-----
    void postorder()
    {
        postorderP(root);
    }
    //-----
    int getSize()
    {
        return getSizeP(root);
    }
    //-----
    int getMax()
    {
        return getMaxP(root);
    }
    //-----
    int getMin()
    {
        return getMinP(root);
    }
    //-----
    bool search(int val)
    {
        return searchP(root, val);
    }
    //-----
    void clear()
    {
        clearP(root);
    }
    //-----
    void *Delete(int val)
    {
        root = DeleteP(root, val);
    }
    //-----
    int getDepth()
    {
        return getDepthP(root);
    }
};

void solve()
{
    Tree t;
    t.insert(7);
    t.insert(5);
    t.insert(3);
    t.insert(2);
    t.inorder();
    cout << nl;
    t.preorder();
    cout << nl;
    t.postorder();
    cout << nl;
    cout << "Tree's size is " << t.getSize() << nl;
    cout << "Maximum number is " << t.getMax() << nl;
    cout << "Minimum number is " << t.getMin() << nl;
    cout << "Rows number is " << t.getDepth() << nl;
    cout << (t.search(5) ? "Found" : "Not found") << nl;
    t.clear();
}

signed main()
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    int tc = 1;
    // cin >> tc;
    while (tc--)
    {
        solve();
        if (tc)
            cout << nl;
    }
    return 0;
}