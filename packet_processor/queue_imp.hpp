/**
 * CopyRight F. J. Madrid-Cuevas <fjmadrid@uco.es>
 *
 * Sólo se permite el uso de este código en la docencia de las asignaturas sobre
 * Estructuras de Datos de la Universidad de Córdoba.
 *
 * Está prohibido su uso para cualquier otro objetivo.
 */
#pragma once

#include <cassert>
#include "queue.hpp"

template<class T>
Queue<T>::Queue ()
{
    //TODO    
    this->input_=Stack<T>::create();
    this->output_=Stack<T>::create();
    //
    assert(is_empty());
}

template<class T>
bool
Queue<T>::is_empty () const
{
    return this->input_->is_empty() && this->output_->is_empty();
}

template<class T>
size_t
Queue<T>::size () const
{
    return this->input_->size() + this->output_->size();
}

template<class T>
T
Queue<T>::front() const
{
    assert(! is_empty());
    T ret_val;
    return this->output_->top();
}

template<class T>
T Queue<T>::back() const
{
    assert(! is_empty());
    return this->back_;
}

template<class T>
void
Queue<T>::enque(const T& new_it)
{
#ifndef NDEBUG
    size_t old_size = size();
#endif
    //TODO
    //Remenber we enque into the input stack.
    //Hint: maybe you need to update the back item.
    this->input_->push(new_it);
    this->back_ = new_it;
    if(this->output_->is_empty()) flush_input_to_output();
    //
    assert(back()==new_it);
    assert(size()==(old_size+1));
}

template<class T>
void
Queue<T>::deque()
{
    assert(! is_empty());
#ifndef NDEBUG
    size_t old_size = size();
#endif
    //TODO
    //Remenber we deque from the output stack and if the output stack is empty
    //we need flush the input stack into the output stack first.
    if(this->output_->is_empty()) flush_input_to_output();
    this->output_->pop();
    if(this->output_->is_empty() && !this->input_->is_empty()) flush_input_to_output();
    //
    assert(size()==(old_size-1));
}


template<class T>
void
Queue<T>::flush_input_to_output()
{
    assert(!input_->is_empty());
#ifndef NDEBUG
    T old_back = back();
#endif
    //TODO
    //Remenber: the first item pushed into output is
    // the new back() of the queue.
    while(!this->input_->is_empty()){
        this->output_->push(this->input_->top());
        this->input_->pop();
    }
    //
    assert(old_back == back());
}
