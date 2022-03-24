/**
 * CopyRight F. J. Madrid-Cuevas <fjmadrid@uco.es>
 *
 * Sólo se permite el uso de este código en la docencia de las asignaturas sobre
 * Estructuras de Datos de la Universidad de Córdoba.
 *
 * Está prohibido su uso para cualquier otro objetivo.
 */
#pragma once

#include "btree.hpp"

template<class T>
BTNode<T>::BTNode (T const& it, BTNode<T>::Ref l,
                   BTNode<T>::Ref r)
{
    this->it_ = it;
    this->left_ = l;
    this->right_ = r;
    assert(item()==it);
    assert(left()==l);
    assert(right()==r);
}

template<class T>
typename BTNode<T>::Ref BTNode<T>::create(T const& it,
                                          BTNode<T>::Ref left,
                                          BTNode<T>::Ref right)
{
    return std::make_shared<BTNode<T>> (it, left, right);
}

template<class T>
T BTNode<T>::item() const
{
  return this->it_;
}



template<class T>
typename BTNode<T>::Ref BTNode<T>::left() const
{
  return this->left_;
}


template<class T>
typename BTNode<T>::Ref BTNode<T>::right() const
{
  return this->right_;
}

template<class T>
void BTNode<T>::set_item(const T& new_it)
{
    this->it_ = new_it;
    assert(item()==new_it);
}

template<class T>
void BTNode<T>::set_left(BTNode<T>::Ref new_child)
{
    this->left_ = new_child;
    assert(left()==new_child);
}

template<class T>
void BTNode<T>::set_right(BTNode<T>::Ref new_child)
{
    this->right_ = new_child;
    assert(right()==new_child);
}

/**
 * ADT BTree.
 * Models a BTree of T.
 */
template<class T>
BTree<T>::BTree ()
    {
        this->root_=nullptr;
        assert(is_empty());
    }

    template<class T>
    BTree<T>::BTree (const T& it)
    {
        this->root_=BTNode<T>::create(it,nullptr,nullptr);
        assert(!is_empty());
        assert(item()==it);
        assert(left()->is_empty());
        assert(right()->is_empty());
    }

template<class T>
typename BTree<T>::Ref BTree<T>::create()
  {
      return std::make_shared<BTree<T>> ();
  }

template<class T>
  typename BTree<T>::Ref BTree<T>::create(T const& item)
  {
      return std::make_shared<BTree<T>>(item);
  }

template<class T>
  typename BTree<T>::Ref BTree<T>::create (std::istream& in) noexcept(false)
  {
    auto tree = BTree<T>::create();
      std::string token;
        T num;

        in>>token;
        if(token == "[]"){
            return tree;
        }
        if(token != "["){
            throw std::runtime_error("Wrong input format.");
        }
        
        in>>token;
        std::istringstream my_stream(token);
        my_stream>>num;
        if(my_stream.fail()){
            throw std::runtime_error("Wrong input format.");
        }
        tree->set_root(BTNode<T>::create(num));
        tree->set_left(BTree<T>::create(in));
        tree->set_right(BTree<T>::create(in));

        in>>token;
        if(token != "]"){
            throw std::runtime_error("Wrong input format.");
        }

        return tree;
  }

template<class T>
  bool BTree<T>::is_empty () const
  {
      if(root() == nullptr) return true;
      return false;
  }

template<class T>
  T BTree<T>::item() const
  {
      assert(!is_empty());
      return root()->item();
  }

template<class T>
  typename BTree<T>::Ref BTree<T>::left() const
  {
      assert(!is_empty());
      return BTree<T>::create(root()->left());
  }

template<class T>
  typename BTree<T>::Ref BTree<T>::right() const
  {
      assert(!is_empty());
      return BTree<T>::create(root()->right());
  }

template<class T>
  std::ostream& BTree<T>::fold(std::ostream& out) const
  {
    auto aux_left = BTree<T>::create();  
    auto aux_right = BTree<T>::create();
    
    if(this->root_==nullptr){
      out << "[]";
      return out;
    }else{
      out << "[ ";
      out << this->root_->item();
      out<<" ";
      this->left()->fold(out);
      out<<" ";
      this->right()->fold(out);
      out<<" ]";
      return out;
    }
  }

  template<class T>
    void BTree<T>::create_root(const T& it)
    {
        assert(is_empty());
        
        this->root_=BTNode<T>::create(it,nullptr,nullptr);

        assert(!is_empty());
        assert(item()==it);
        assert(left()->is_empty());
        assert(right()->is_empty());

    }

template<class T>
  void BTree<T>::set_item(const T& new_it)
  {
      assert(!is_empty());
      root()->set_item(new_it);
      assert(item()==new_it);
  }

template<class T>
  void BTree<T>::set_left(typename BTree<T>::Ref new_left)
  {
      assert(!is_empty());
      root()->set_left(new_left->root());
      assert(left()->root()==new_left->root());
  }

template<class T>
  void BTree<T>::set_right(typename BTree<T>::Ref new_right)
  {
      assert(!is_empty());
      root()->set_right(new_right->root());
      assert(right()->root()==new_right->root());
  }

template<class T>
  BTree<T>::BTree (typename BTNode<T>::Ref n)
  {
      this->root_=n;
      assert(root()==n);
  }

template<class T>
typename BTree<T>::Ref BTree<T>::create(typename BTNode<T>::Ref root)
  {
      //We cannot use std::make_shared here
      //because the constructor is protected.
      typename BTree<T>::Ref tree ( new BTree<T>(root) );
      return tree;
  }

template<class T>
  typename BTNode<T>::Ref BTree<T>::root() const
  {
      return this->root_;
  }

template<class T>
  void BTree<T>::set_root(typename BTNode<T>::Ref new_root)
  {
      this->root_=new_root;
      assert(root()==new_root);
  }

