/**
 * @file avltree_imp.hpp
 *
 * CopyRight F. J. Madrid-Cuevas <fjmadrid@uco.es>
 *
 * Sólo se permite el uso de este código en la docencia de las asignaturas sobre
 * Estructuras de Datos de la Universidad de Córdoba.
 *
 * Está prohibido su uso para cualquier otro objetivo.
 */
#pragma once
#include <cassert>

#include "avltree.hpp"

/****
 * AVLTNode class implementation.
 ****/

template <class T>
AVLTNode<T>::AVLTNode(T const &it, AVLTNode<T>::Ref parent,
                      AVLTNode<T>::Ref left, AVLTNode<T>::Ref right) : item_(it), parent_(parent), left_(left), right_(right), height_(0)
{
    // TODO
    compute_height();
    //
    assert(check_height_invariant());
}

template <class T>
typename AVLTNode<T>::Ref AVLTNode<T>::create(T const &it,
                                              AVLTNode<T>::Ref parent,
                                              AVLTNode<T>::Ref left,
                                              AVLTNode<T>::Ref right)
{
    return std::make_shared<AVLTNode<T>>(it, parent, left, right);
}

template <class T>
T AVLTNode<T>::item() const
{
    return this->item_;
}

template <class T>
int AVLTNode<T>::height() const
{
    return this->height_;
}

template <class T>
int AVLTNode<T>::balance_factor() const
{
    int l_height = 0;
    int r_height = 0;

    if (this->left_ != nullptr)
        l_height = this->left_->height() + 1;
    if (this->right_ != nullptr)
        r_height = this->right_->height() + 1;

    return r_height - l_height;
}

template <class T>
typename AVLTNode<T>::Ref AVLTNode<T>::parent() const
{
    return this->parent_;
}

template <class T>
typename AVLTNode<T>::Ref AVLTNode<T>::left() const
{
    return this->left_;
}

template <class T>
typename AVLTNode<T>::Ref AVLTNode<T>::right() const
{
    return this->right_;
}

template <class T>
bool AVLTNode<T>::check_height_invariant() const
{
    bool ret_val = false;

    if (this->left_ == nullptr && this->right_ == nullptr && this->height_ == 0)
        ret_val = true;
    else if (this->left_ != nullptr && this->right_ == nullptr && this->height_ == 1 + left()->height())
        ret_val = true;
    else if (this->right_ != nullptr && this->left_ == nullptr && this->height_ == 1 + right()->height())
        ret_val = true;
    else if (this->left_ != nullptr && this->right_ != nullptr)
    {
        if (left()->height() > right()->height() && this->height_ == 1 + left()->height())
            ret_val = true;
        else if (right()->height() >= left()->height() && this->height_ == 1 + right()->height())
            ret_val = true;
    }

    return ret_val;
}

template <class T>
void AVLTNode<T>::set_item(const T &new_it)
{
    this->item_ = new_it;
    compute_height();
    assert(item() == new_it);
}

template <class T>
void AVLTNode<T>::set_parent(AVLTNode<T>::Ref new_parent)
{
    this->parent_ = new_parent;
    compute_height();
    assert(parent() == new_parent);
}

template <class T>
void AVLTNode<T>::set_left(AVLTNode<T>::Ref new_child)
{
    this->left_ = new_child;
    compute_height();
    assert(check_height_invariant());
    assert(left() == new_child);
}

template <class T>
void AVLTNode<T>::set_right(AVLTNode<T>::Ref new_child)
{
    this->right_ = new_child;
    compute_height();
    assert(check_height_invariant());
    assert(right() == new_child);
}

template <class T>
void AVLTNode<T>::compute_height()
{
    if (this->left_ == nullptr && this->right_ == nullptr)
        this->height_ = 0;
    else if (this->left_ != nullptr && this->right_ != nullptr)
    {
        if (right()->height() >= left()->height())
            this->height_ = 1 + right()->height();
        else
            this->height_ = 1 + left()->height();
    }
    else if (this->left_ != nullptr)
        this->height_ = 1 + left()->height();
    else
        this->height_ = right()->height() + 1;

    //
    assert(check_height_invariant());
}

/***
 * AVLTree class implementation.
 ***/

template <class T>
AVLTree<T>::AVLTree()
{
    // TODO
    root_ = nullptr;
    prev_ = nullptr;
    current_ = nullptr;
    //
    assert(is_a_binary_search_tree());
    assert(is_a_balanced_tree());
    assert(is_empty());
}

template <class T>
AVLTree<T>::AVLTree(T const &item)
{
    // TODO
    root_ = AVLTNode<T>::create(item);
    //
    assert(is_a_binary_search_tree());
    assert(is_a_balanced_tree());
    assert(!is_empty());
}

template <class T>
typename AVLTree<T>::Ref AVLTree<T>::create()
{
    return std::make_shared<AVLTree<T>>();
}

template <class T>
typename AVLTree<T>::Ref AVLTree<T>::create(T const &item)
{
    return std::make_shared<AVLTree<T>>(item);
}

template <class T>
typename AVLTree<T>::Ref AVLTree<T>::create(std::istream &in) noexcept(false)
{
    auto tree = AVLTree<T>::create();
    std::string token;
    in >> token;
    T num;

    if (token == "[]")
    {
        return tree;
    }
    if (token != "[")
    {
        throw std::runtime_error("Wrong input format.");
    }

    in >> token;
    std::istringstream my_stream(token);
    my_stream >> num;
    if (my_stream.fail())
    {
        throw std::runtime_error("Wrong input format.");
    }
    tree = AVLTree<T>::create(num);

    tree->set_left(AVLTree<T>::create(in));

    tree->set_right(AVLTree<T>::create(in));

    in >> token;
    if (token != "]")
    {
        throw std::runtime_error("Wrong input format.");
    }

    //
    if (!tree->is_a_binary_search_tree())
        throw std::runtime_error("It is not a binary search tree");
    if (!tree->is_a_balanced_tree())
        throw std::runtime_error("It is not an avl bstree");
    return tree;
}

#ifdef __ONLY_BSTREE__
/**
 * @brief Create an perfectly balanced BSTree by inserting the median of
 *        an ordered sub sequence data[begin, end).
 * @param data is the ordered sequence of values.
 * @param begin,
 * @param end specify a [begin, end) indexing interval of array data to use.
 * @pre 0 <= begin <= end <=data.size()
 * @pre begin==end || data[begin]<data[end];
 */
template <class T>
void create_inserting_median(std::vector<T> const &data,
                             size_t begin,
                             size_t end,
                             typename AVLTree<T>::Ref &tree)
{
    assert(begin <= end);
    assert(end <= data.size());
    assert(begin == end || data[begin] <= data[end - 1]);

    // TODO
    // Hint: if (end==begin) none thing must be done (it is an empty sub array)
    //  else, insert the median in the tree and (recursively) process
    //  the two sub sequences [begin, median_idx) and [median_idx+1, end)

    if (end - begin > 0)
    {
        tree->insert(data[begin + ((end - begin) / 2)]);
        create_inserting_median(data, begin, begin + ((end - begin) / 2), tree);
        create_inserting_median(data, (begin + ((end - begin) / 2)) + 1, end, tree);
    }

    //
}

template <class T>
typename AVLTree<T>::Ref
create_perfectly_balanced_bstree(std::vector<T> &data)
{
    typename AVLTree<T>::Ref tree = AVLTree<T>::create();
    std::sort(data.begin(), data.end());
    create_inserting_median(data, 0, data.size(), tree);
    assert(tree != nullptr);
    return tree;
}
#endif //__ONLY_BSTREE__

template <class T>
bool AVLTree<T>::is_empty() const
{
    return root_ == nullptr ? true : false;
}

template <class T>
T AVLTree<T>::item() const
{
    assert(!is_empty());
    return root_->item();
}

template <class T>
std::ostream &AVLTree<T>::fold(std::ostream &out) const
{
    if (is_empty())
    {
        out << "[]";
        return out;
    }
    out << "["
        << " " << root_->item();
    out << " ";
    left()->fold(out);
    out << " ";
    right()->fold(out);
    out << " "
        << "]";
    //
    return out;
}

template <class T>
bool AVLTree<T>::current_exists() const
{
    if (current_ != nullptr)
    {
        return true;
    }
    return false;
}

template <class T>
T AVLTree<T>::current() const
{
    assert(current_exists());
    return current_->item();
}

template <class T>
int AVLTree<T>::current_level() const
{
    assert(current_exists());
    int level = 0;
    // TODO
    auto thisnode = root_;
    while (thisnode != current_)
    {
        if (thisnode->item() > current_->item())
            thisnode = thisnode->left();
        else if (thisnode->item() < current_->item())
            thisnode = thisnode->right();
        level++;
    }
    //
    return level;
}

template <class T>
typename AVLTree<T>::Ref AVLTree<T>::left() const
{
    assert(!is_empty());
    typename AVLTree<T>::Ref subtree;
    auto l_subtree = AVLTree<T>::create();
    if (root_->left() != nullptr)
    {
        l_subtree->create_root(root_->left()->item());
        l_subtree->root_->set_left(root_->left()->left());
        l_subtree->root_->set_right(root_->left()->right());
        l_subtree->root_->set_parent(nullptr);
    }
    return l_subtree;
}

template <class T>
typename AVLTree<T>::Ref AVLTree<T>::right() const
{
    assert(!is_empty());
    typename AVLTree<T>::Ref subtree;
    auto r_subtree = AVLTree<T>::create();
    if (root_->right() != nullptr)
    {
        r_subtree->create_root(root_->right()->item());
        r_subtree->root_->set_left(root_->right()->left());
        r_subtree->root_->set_right(root_->right()->right());
        r_subtree->root_->set_parent(nullptr);
    }
    return r_subtree;
}

template <class T>
int AVLTree<T>::size() const
{
    int s = 0;
    // TODO
    if (!is_empty())
        s = 1 + left()->size() + right()->size();
    return s;
}

template <class T>
int AVLTree<T>::height() const
{
    int h = -1;
    // TODO
    if (!is_empty())
        h = root_->height();
    //
    return h;
}

template <class T>
int AVLTree<T>::balance_factor() const
{
#ifdef __ONLY_BSTREE__
    return 0;
#else
    int bf = 0;
    // TODO
    if (!is_empty())
        bf = root_->balance_factor();
    //
    return bf;
#endif
}

template <class T>
bool AVLTree<T>::has(const T &k) const
{
#ifndef NDEBUG
    bool old_current_exists = current_exists();
    T old_current;
    if (old_current_exists)
        old_current = current();
#endif

    bool found = false;

    auto thisnode = root_;
    while (thisnode != nullptr && !found)
    {
        if (thisnode->item() > k)
        {
            thisnode = thisnode->left();
        }
        else if (thisnode->item() < k)
        {
            thisnode = thisnode->right();
        }
        else
            found = true;
    }

#ifndef NDEBUG
    assert(!old_current_exists || old_current == current());
#endif
    return found;
}

/**
 * @brief infix process of a node.
 * The Processor must allow to be used as a function with a parameter  (the
 * item to be processed) and returning true if the process must continue or
 * false if not.
 * @param node is the node to be processed.
 * @param p is the Processor.
 * @return true if all the tree was in-fix processed.
 */
template <class T, class Processor>
bool infix_process(typename AVLTNode<T>::Ref node, Processor &p)
{
    bool retVal = true;
    // TODO
    // Remember: if node is nullptr return true.
    if (node != nullptr)
    {
        retVal = retVal && infix_process<T, Processor>(node->left(), p);
        retVal = retVal && p(node);
        retVal = retVal && infix_process<T, Processor>(node->right(), p);
    }
    //
    return retVal;
}

template <class T>
bool AVLTree<T>::is_a_binary_search_tree() const
{
    bool is_bst = true;
    if (!is_empty())
    {
        if (root_->left() != nullptr)
            is_bst = is_bst && (item() > left()->item());
        if (root_->right() != nullptr)
            is_bst = is_bst && (item() < right()->item());
        is_bst = is_bst && left()->is_a_binary_search_tree() && right()->is_a_binary_search_tree();
    }
    return is_bst;
}

template <class T>
bool AVLTree<T>::is_a_balanced_tree() const
{
#ifdef __ONLY_BSTREE__
    return true;
#else
    bool is_balanced = true;
    // TODO
    // Remenber: a empty tree is balanced.
    //
    // Remenber: A non empty tree is balanced if both children are balanced
    // and the absolute value of the balance factor of the tree is less or
    // equal to one.

    return true;

    if (is_empty())
    {
        return true;
    }

    auto p = [](typename AVLTNode<T>::Ref node) mutable -> bool
    {
        if (node->balance_factor() > (-2) && node->balance_factor() < 2)
        {
            return true;
        }
        return false;
    };

    is_balanced = infix_process<T>(root_, p);
    //
    return is_balanced;
#endif
}

template <class T>
void AVLTree<T>::create_root(T const &v)
{
    assert(is_empty());
    // TODO
    root_ = AVLTNode<T>::create(v);
    //
    assert(is_a_binary_search_tree());
    assert(is_a_balanced_tree());
    assert(!is_empty());
    assert(item() == v);
}

template <class T>
bool AVLTree<T>::search(T const &k)
{
    bool found = false;
    current_ = root_;
    if (!is_empty())
    {
        while (current_exists() && !found)
        {
            if (current() == k)
                found = true;
            else
            {
                if (current() > k)
                {
                    prev_ = current_;
                    current_ = current_->left();
                }
                else
                {
                    prev_ = current_;
                    current_ = current_->right();
                }
            }
        }
    }

    assert(!found || current() == k);
    assert(found || !current_exists());
    return found;
}

template <class T>
void AVLTree<T>::insert(T const &k)
{
    // Check invariants.
    assert(is_a_binary_search_tree());
    assert(is_a_balanced_tree());

    if (!search(k))
    {
        // TODO
        if (is_empty())
        {
            current_ = AVLTNode<T>::create(k);
            root_ = current_;
        }
        else
        {
            current_ = AVLTNode<T>::create(k);
            if (prev_->item() > k)
                prev_->set_left(current_);
            else
                prev_->set_right(current_);
        }
        make_balanced();
    }

    // Check invariants.
    assert(is_a_binary_search_tree());
    assert(is_a_balanced_tree());

    // check postconditions.
    assert(current_exists());
    assert(current() == k);
}

template <class T>
void AVLTree<T>::remove()
{
    // check preconditions.
    assert(current_exists());

#ifndef NDEBUG
    // the invariants only must be checked for the first recursive call.
    // We use a static variable to count the recursion levels.
    // see section "Static variables in a Function" in
    // ref https://www.geeksforgeeks.org/static-keyword-cpp/ for more info.
    static int recursion_count = 0;
    recursion_count++;
    if (recursion_count == 1)
    {
        // Check invariants.
        assert(is_a_binary_search_tree());
        assert(is_a_balanced_tree());
    }
#endif // NDEBUG

    bool replace_with_subtree = true;
    typename AVLTNode<T>::Ref subtree;

    // TODO
    //  Check which of cases 0,1,2,3 we have (see theorical class slides).
    if (current_->left() == nullptr && current_->right() == nullptr)
        subtree = nullptr;
    else if (current_->right() == nullptr)
        subtree = current_->left();
    else if (current_->left() == nullptr)
        subtree = current_->right();
    else
        replace_with_subtree = false;
    //

    if (replace_with_subtree)
    {
        // TODO
        // Manage cases 0,1,2
        if (prev_ == nullptr)
            root_ = subtree;
        else if (prev_->right() == current_)
            prev_->set_right(subtree);
        else
            prev_->set_left(subtree);

        current_ = nullptr;
        //
        make_balanced();
    }
    else
    {
        // TODO
        // Manage case 3.
        auto aux = current_;
        find_inorder_sucessor();
        aux->set_item(current_->item());
        remove();
        //
    }

#ifndef NDEBUG
    // We come back so the recursion count must be decreased.
    recursion_count--;
    assert(recursion_count >= 0);
    if (recursion_count == 0)
    {
        // Only check for the last return.
        // Check invariants.
        assert(is_a_binary_search_tree());
        assert(is_a_balanced_tree());

        // Check postconditions.
        assert(!current_exists());
    }
#endif
}

template <class T>
AVLTree<T>::AVLTree(typename AVLTNode<T>::Ref root_node)
{
    // TODO
    root_ = root_node;
    //
    assert(!current_exists());
}

template <class T>
typename AVLTree<T>::Ref AVLTree<T>::create(typename AVLTNode<T>::Ref root)
{
    AVLTree<T>::Ref ret_v(new AVLTree<T>(root));
    return ret_v;
}

template <class T>
void AVLTree<T>::set_left(Ref subtree)
{
    assert(!is_empty());
    // TODO
    // Remenber to set parent's link of the subtree root to this.
    if (!subtree->is_empty())
    {
        auto newtree = AVLTNode<T>::create(subtree->root_->item(), root_, subtree->root_->left(), subtree->root_->right());
        newtree->compute_height();
        root_->set_left(newtree);
    }
    else
        root_->set_left(nullptr);
    //
    assert(subtree->is_empty() || left()->item() == subtree->item());
    assert(!subtree->is_empty() || left()->is_empty());
}

template <class T>
void AVLTree<T>::set_right(Ref subtree)
{
    assert(!is_empty());
    // TODO
    // Remenber to set parent's link of the subtree root to this.
    if (!subtree->is_empty())
    {
        auto newtree = AVLTNode<T>::create(subtree->root_->item(), root_, subtree->root_->left(), subtree->root_->right());
        newtree->compute_height();
        root_->set_right(newtree);
    }
    else
        root_->set_right(nullptr);
    //
    assert(subtree->is_empty() || right()->item() == subtree->item());
    assert(!subtree->is_empty() || right()->is_empty());
}

template <class T>
typename AVLTNode<T>::Ref AVLTree<T>::current_node() const
{
    typename AVLTNode<T>::Ref node;
    // TODO
    node = current_;
    //
    return node;
}

template <class T>
typename AVLTNode<T>::Ref AVLTree<T>::root_node() const
{
    typename AVLTNode<T>::Ref node;
    // TODO
    node = root_;
    //
    return node;
}

template <class T>
typename AVLTNode<T>::Ref AVLTree<T>::parent_node() const
{
    typename AVLTNode<T>::Ref node;
    // TODO
    node = prev_;
    //
    return node;
}
template <class T>
void AVLTree<T>::find_inorder_sucessor()
{
    assert(current_exists());
    assert(is_a_binary_search_tree());

#ifndef NDEBUG
    T old_curr = current();
#endif
    // TODO
    prev_ = current_;
    current_ = current_->right();
    while (current_->left() != nullptr)
    {
        prev_ = current_;
        current_ = current_->left();
    }
    //
    assert(current_exists() && current_node()->left() == nullptr);
#ifndef NDEBUG
    assert(current() > old_curr);
#endif
}

template <class T>
void AVLTree<T>::rotate_left(typename AVLTNode<T>::Ref node)
{
    assert(node->left() != nullptr);
    // TODO
    // Remenber: when set a node A as child of a node B, also is needed set
    //  node B as parent of node A.
    // Remenber: update the node height at the end.
    auto lc = node->left(); // Creamos el nodo que tendra el valor del izquiedo

    if (node->parent() == nullptr) // Entramos si el nodo no tiene padre
        root_ = lc;

    else if (node == node->parent()->right()) // Entramos si el nodo es igual al derecho del padre de nodo
        node->parent()->set_right(lc);

    else if (node == node->parent()->left()) // Entramos si el nodo es igual al izquiedo del padre de nodo
        node->parent()->set_left(lc);

    lc->set_parent(node->parent()); // Seteamos el padre

    if (lc->right() != nullptr) // SI el nodo lc tiene derecho
    {
        node->set_left(lc->right());
        lc->right()->set_parent(node);
    }
    else
    {
        node->set_left(nullptr);
        node->left()->compute_height();
    }

    lc->set_right(node);  // Seteamos el derecho de lc
    node->set_parent(lc); // Seteamos el padre de node

    // Calculamos la altura de estos nodos al acabar
    node->compute_height();
    lc->compute_height();

    //
}

template <class T>
void AVLTree<T>::rotate_right(typename AVLTNode<T>::Ref node)
{
    assert(node->right() != nullptr);
    // TODO
    // Remenber: when set a node A as child of a node B, also is needed set
    //  node B as parent of node A.
    // Remenber: update the node height at the end.

    auto rc = node->right(); // Creamos el nodo que tendra el valor del derecho

    if (node->parent() == nullptr) // Entramos si el nodo no tiene padre
        root_ = rc;

    else if (node == node->parent()->right()) // Entramos si el nodo es igual al derecho del padre de nodo
        node->parent()->set_right(rc);

    else if (node == node->parent()->left()) // Entramos si el nodo es igual al izquierdo del padre de nodo
        node->parent()->set_left(rc);

    rc->set_parent(node->parent()); // Seteamos el padre

    if (rc->left() != nullptr) // Si el nodo rc tiene izquierdo
    {
        node->set_right(rc->left());
        rc->left()->set_parent(node);
    }
    else
    {
        node->set_right(nullptr);
        node->right()->compute_height();
    }

    rc->set_left(node);   // Seteamos el izquierdo de rc
    node->set_parent(rc); // Seteamos el padre de node

    // Calculamos la altura de estos nodos al acabar
    node->compute_height();
    rc->compute_height();

    //
}
template <class T>
void AVLTree<T>::make_balanced()
{
#ifdef __ONLY_BSTREE__
    return;
#else

    typename AVLTNode<T>::Ref node = current_node();
    int bfParent, bfChild;
    typename AVLTNode<T>::Ref child;

    while (prev_ != nullptr)
    {

        prev_->compute_height();
        bfParent = prev_->balance_factor();

        if (bfParent < -1)
        {
            child = prev_->left();
            bfChild = child->balance_factor();
            if (bfChild <= 0)
                rotate_left(prev_);
            else
            {
                rotate_right(child);
                rotate_left(prev_);
            }
        }
        else if (bfParent > 1)
        {
            child = prev_->right();
            bfChild = child->balance_factor();
            if (bfChild >= 0)
                rotate_right(prev_);
            else
            {
                rotate_left(child);
                rotate_right(prev_);
            }
        }
        else
        {
            prev_ = prev_->parent();
        }
    }

   
    assert(!current_exists() || current_node()->parent() == parent_node());
#endif //__ONLY_BSTREE__
}