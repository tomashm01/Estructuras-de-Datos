/**
 * CopyRight F. J. Madrid-Cuevas <fjmadrid@uco.es>
 *
 * Sólo se permite el uso de este código en la docencia de las asignaturas sobre
 * Estructuras de Datos de la Universidad de Córdoba.
 *
 * Está prohibido su uso para cualquier otro objetivo.
 */
#pragma once

#include "slist.hpp"
#include <sstream>
#include <stdexcept>
#include <cassert>

template <class T>
SNode<T>::SNode (T const& it)
{
    this->_item=it;
    this->_next=nullptr;
    assert(!has_next());
}

template <class T>
SNode<T>::SNode (T const& it, SNode<T>::Ref& next)
{
    this->_item=it;
    this->_next=next;
}

template <class T>
SNode<T>::~SNode()
{
}

template <class T>
typename SNode<T>::Ref SNode<T>::create(T const& it, SNode<T>::Ref next)
{
    return std::make_shared<SNode<T>> (it, next);
}

template <class T>
T SNode<T>::item() const
{
    return this->_item;
}

template <class T>
bool SNode<T>::has_next() const
{
    if(this->_next!=nullptr) return true;
    return false;
}

template <class T>
typename SNode<T>::Ref SNode<T>::next() const
{
    return this->_next;
}

template <class T>
void SNode<T>::set_item(const T& new_it)
{
    this->_item=new_it;
    assert(item()==new_it);
}

template <class T>
void SNode<T>::set_next(SNode<T>::Ref n)
{
    this->_next=n;
    assert(n == next());
}

template<class T>
SList<T>::SList ()
{
    this->_head=nullptr;    
    this->_curr=nullptr;
    this->_prev=nullptr;
    assert(is_empty());
}

template<class T>
SList<T>::~SList()
{
}

template<class T>
typename SNode<T>::Ref SList<T>::head() const
{
    return this->_head;
}

template<class T>
typename SNode<T>::Ref SList<T>::curr() const
{
    return this->_curr;
}

template<class T>
typename SList<T>::Ref SList<T>::create()
{
    return std::make_shared<SList<T>> ();
}

template<class T>
typename SList<T>::Ref SList<T>::create(std::istream& in) noexcept(false)
{
    auto list = SList<T>::create();
    auto aux= SList<T>::create();
    std::string cadena;
    in >> cadena;
    T token;
    //TODO
    //Hint: use std::istriongstream to convert from "string" to template
    // parameter T type.
    // Throw std::runtime_error("Wrong input format.") exception if a input
    // format error was found.

    if(cadena=="[]") return list;
    else if(cadena=="]") throw std::runtime_error("Wrong input format.");
    else if(cadena=="["){

        while(in>>cadena){
            if(cadena=="]") break;
            std::istringstream ss(cadena);
            ss>>token;
            aux->push_front(token);
        }
        while(!aux->is_empty()){
            list->push_front(aux->front());
            aux->pop_front();
        }

        if(cadena!="]") throw std::runtime_error("Wrong input format.");
        return list;
    }
    else throw std::runtime_error("Wrong input format.");

    //
    return list;
}

template<class T>
bool SList<T>::is_empty () const
{
    if(this->_head!=nullptr) return false;
    return true;
}

template<class T>
size_t SList<T>::size () const
{
    size_t ret_val = 0;
    if(head()!=nullptr){
        typename SNode<T>::Ref aux=head();
        while(aux!=nullptr){
            ret_val++;
            aux=aux->next();
        }
    }
    return ret_val;
}

template<class T>
T SList<T>::front() const
{
    assert(!is_empty());
    return this->_head->item();
}

template<class T>
T SList<T>::current() const
{    
    assert(! is_empty());
    return this->_curr->item();
}

template<class T>
bool SList<T>::has_next() const
{
    assert(!is_empty());
    if(_curr->has_next()) return true;
    return false;
}

template<class T>
T SList<T>::next() const
{
    assert(has_next());
    return this->_curr->next()->item();
}


template<class T>
bool SList<T>::has(T const& it) const
{
    typename SNode<T>::Ref start=_head;
    while(start!=nullptr){
        if(start->item()==it) return true;
        start=start->next();
    }
    return false;
}

template<class T>
void SList<T>::fold(std::ostream& out) const
{
    typename SNode<T>::Ref aux=this->_head;
    if(aux==nullptr) out<<"[]";
    else{
        out << "[ ";
        out << this->_head->item();
        while (aux->has_next())
        {
            aux = aux->next();
            out << " ";
            out << aux->item();
        }
        out << " ]";
    }
}

template<class T>
void SList<T>::set_current(T const& new_v)
{
    assert(!is_empty());
    //TODO
    this->_curr->set_item(new_v);
    //
    assert(current()==new_v);
}


template<class T>
void SList<T>::push_front(T const& new_it)
{
#ifndef NDEBUG
    size_t old_size = size();
#endif
    auto node=SNode<T>::create(new_it,this->_head);
    if(this->_curr==this->_head) this->_curr=node;
    this->_head=node;
    
    assert(front()==new_it);
    assert(size() == (old_size+1));
}

template<class T>
void SList<T>::insert(T const& new_it)
{
#ifndef NDEBUG
    bool old_is_empty = is_empty();
    size_t old_size = size();
    T old_item;
    if (!old_is_empty)
        old_item = current();
#endif

    if(this->is_empty()){
        this->push_front(new_it);
        this->_curr=this->_head;
    }
    else{
        typename SNode<T>::Ref node = SNode<T>::create(new_it, this->_curr->next());
        this->_curr->set_next(node);
    }

    assert(!old_is_empty || (front()==new_it && current()==new_it));
    assert(old_is_empty || (old_item == current() && has_next() && next()==new_it));
    assert(size()==(old_size+1));
}

template<class T>
void SList<T>::pop_front()
{
    assert(!is_empty());
#ifndef NDEBUG
    size_t old_size = size();
    auto old_head_next = head()->next();
#endif
    //TODO
    if(this->_curr==this->_head) this->_curr = this->_head->next();
    this->_head = this->_head->next();
    assert(is_empty() || head() == old_head_next);
    assert(size() == (old_size-1));
}


template<class T>
void SList<T>::remove()
{
    assert(!is_empty());
#ifndef NDEBUG
    size_t old_size = size();
    bool old_has_next = has_next();
    T old_next;
    if (has_next())
        old_next=next();
#endif
    //TODO
    //Case 1: current is the head.

    //Case 2: remove in a middle position.

    //Case 3: remove the last element.
    //Remenber to locate previous of prev_.

    if(this->_curr==this->_head){
        pop_front();
        this->_prev=nullptr;
        this->_curr=this->_head;
    }else{
        if (this->_curr->has_next()){
            this->_prev->set_next(this->_curr->next());
            this->_curr = this->_curr->next();
        }
        else{
            find(this->_prev->item());
            this->_curr->set_next(nullptr);
        }
    }

    assert(!old_has_next || current()==old_next);
    assert(size() == (old_size-1));
}

template<class T>
void SList<T>::goto_next()
{
    assert(has_next());
#ifndef NDEBUG
    auto old_next = next();
#endif
    this->_prev = this->_curr;
    this->_curr = this->_curr->next();
    assert(current()==old_next);
}

template<class T>
void SList<T>::goto_first()
{
    assert(!is_empty());
    //TODO
    this->_prev=nullptr;
    this->_curr=this->_head; 
    //
    assert(current()==front());
}

template<class T>
bool SList<T>::find(T const& it)
{
    assert(!is_empty());
    bool found = false;
    
    
    if(this->_head->item()==it) return true;
    this->_curr=this->_head;
    while(this->_curr->has_next() && !found){
        this->_prev=this->_curr;
        this->_curr=this->_curr->next();
        
        if(this->_curr->item()==it){
            found=true;
            break;
        }
        
    }

    assert(!found || current()==it);
    assert(found || !has_next());
    return found;
}

template<class T>
bool SList<T>::find_next(T const& it)
{
    assert(has_next());
    bool found = false;
    
    while (this->_curr->has_next() && !found){
        this->_prev = this->_curr;
        this->_curr = this->_curr->next();
        if (this->_curr->item() == it) found = true;
    }

    assert(!found || current()==it);
    assert(found || !has_next());
    return found;
}

