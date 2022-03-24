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
#include <exception>
#include <memory>
#include <iostream>
#include <queue>

#include "btree_utils.hpp"

template<class T>
int compute_height (typename BTree<T>::Ref t)
{
    assert(t != nullptr);
    int height = -1;
    if(!t->is_empty()){
        int left=compute_height<T>(t->left())+1;
        int right=compute_height<T>(t->right())+1;
        if(left>right) height=left;
        else height=right;
    }
    return height;
}

template<class T>
size_t compute_size (typename BTree<T>::Ref t)
{
    assert(t != nullptr);
    size_t ret_val = 0;
    if(!t->is_empty()){
        ret_val=1+compute_size<T>(t->left())+compute_size<T>(t->right());
    }
    return ret_val;
}

template <class T, typename Processor>
bool
prefix_process(typename BTree<T>::Ref tree, Processor& p)
{
    assert(tree != nullptr);
    bool retVal = true;
    if(!tree->is_empty()){
        retVal = p(tree->item());
        if(retVal){
            retVal = prefix_process<T>(tree->left(), p);
            if(retVal){
                retVal = prefix_process<T>(tree->right(), p);
            }
        }
    }
    return retVal;
}

template <class T, class Processor>
bool
infix_process(typename BTree<T>::Ref tree, Processor& p)
{
    assert(tree != nullptr);
    bool retVal = true;
    if(!tree->is_empty()){
        retVal=infix_process<T,Processor>(tree->left(), p);
        retVal=retVal && p(tree->item());
        retVal=retVal && infix_process<T,Processor>(tree->right(), p);
    }
    return retVal;
}

template <class T, class Processor>
bool
postfix_process(typename BTree<T>::Ref tree, Processor& p)
{
    assert(tree != nullptr);
    bool retVal = true;
    if(!tree->is_empty()){
        retVal=postfix_process<T,Processor>(tree->left(), p);
        retVal=retVal && postfix_process<T,Processor>(tree->right(), p);
        retVal=retVal && p(tree->item());
    }
    return retVal;
}


template <class T, class Processor>
bool
breadth_first_process(typename BTree<T>::Ref tree, Processor& p)
{
    assert(tree != nullptr);
    bool go_on = true;
    typename BTree<T>::Ref subtree;
    std::queue<typename BTree<T>::Ref> q;
    q.push(tree); //Encolamos el árbol antes de empezar

    while (!q.empty() && go_on)
    {
        subtree = q.front();      //Igualamos al subtree al frente de la cola
        if (!subtree->is_empty()) //Mientras que el subtree no esté vacío, entramos en la condición
        {
            go_on = p(subtree->item());
            q.push(subtree->left());  //Metemos el árbol izquierdo en la cola
            q.push(subtree->right()); //Metemos el árbol derecho en la cola
        }
        q.pop(); //Desencolamos el árbol
    }
    return go_on;
}

template <class T>
std::ostream&
print_prefix(std::ostream& out, typename BTree<T>::Ref tree)
{
    //TODO
    //You must create a lambda function with a parameter to be printed and
    //  use a prefix_process to process the tree with this lambda.
    //Remenber: the lambda must return true.

    auto p=[&out](T n) mutable->bool{
        out << n << " ";
        return true;
    };
    prefix_process<T>(tree,p);

    //
    return out;
}

template <class T>
std::ostream&
print_infix(std::ostream& out, typename BTree<T>::Ref tree)
{
    //TODO
    //You must create a lambda function with a parameter to be printed and
    //  use a infix_process to process the tree with this lambda.
    //Remenber: the lambda must return true.

    auto p=[&out](T n) mutable->bool{
        out << n << " ";
        return true;
    };
    infix_process<T,decltype(p)>(tree,p);

    //
    return out;
}

template <class T>
std::ostream&
print_postfix(std::ostream& out, typename BTree<T>::Ref tree)
{
    //TODO
    //You must create a lambda function with a parameter to be printed and
    //  use a postfix_process to process the tree with this lambda.
    //Remenber: the lambda must return true.

    auto p=[&out](T n) mutable->bool{
        out << n << " ";
        return true;
    };
    postfix_process<T,decltype(p)>(tree,p);

    //
    return out;
}

template <class T>
std::ostream&
print_breadth_first(std::ostream& out, typename BTree<T>::Ref tree)
{
    //TODO
    //You must create a lambda function with a parameter to be printed and
    //  use a breadth_first_process to process the tree with this lambda.
    //Remenber: the lambda must return true.

    //Imprimir con una funcion lambda que imprima el elemento
    auto p=[&out](T n) mutable->bool{
        out << n << " ";
        return true;
    };
    breadth_first_process<T>(tree,p);

    //
    return out;
}

template <class T>
bool search_prefix(typename BTree<T>::Ref tree, const T& it, size_t& count)
{
    bool found = false;
    count = 0;
    //TODO
    //You must create a lambda function with a parameter to compare it to the
    // value of the element to be searched for.
    // Use the lambda with in the prefix_process.
    //Remenber: Also, the lambda must update the count variable and
    //must return True/False.

    auto p=[&found,&count,&it](T n) mutable->bool{
        count++;
        if(n==it) found=true;
        return true;
    };
    prefix_process<T>(tree,p);


    //
    return found;
}

template <class T>
bool search_infix(typename BTree<T>::Ref tree, const T& it, size_t& count)
{
    bool found = false;
    count = 0;
    //TODO
    //You must create a lambda function with a parameter to compare it to the
    // value of the element to be searched for.
    // Use the lambda with in the infix_process.
    //Remenber: Also, the lambda must update the count variable and
    //must return True/False.

    auto p=[&found,&count,&it](T n) mutable->bool{
        count++;
        if(n==it) found=true;
        return !found;
    };
    infix_process<T,decltype(p)>(tree,p);

    //
    return found;
}

template <class T>
bool search_postfix(typename BTree<T>::Ref tree, const T& it, size_t& count)
{
    bool found = false;
    count = 0;
    //TODO
    //You must create a lambda function with a parameter to compare it to the
    // value of the element to be searched for.
    // Use the lambda with in the postfix_process.
    //Remenber: Also, the lambda must update the count variable and
    //must return True/False.

    //Usar una funcion lambda para buscar la cantidad de veces que aparece el elemento
    auto p=[&found,&count,&it](T n) mutable->bool{
        count++;
        if(n==it) found=true;
        return !found;
    };
    postfix_process<T,decltype(p)>(tree,p);

    //
    return found;
}

template <class T>
bool search_breadth_first(typename BTree<T>::Ref tree, const T& it, size_t& count)
{
    bool found = false;
    count = 0;
    //TODO
    //You must create a lambda function with a parameter to compare it to the
    // value of the element to be searched for.
    // Use the lambda with in the breadth_first_process.
    //Remenber: Also, the lambda must update the count variable and
    //must return True/False.

    auto p=[&found,&count,it](T n) mutable->bool{
        count++;
        if(n==it) found=true;
        return !found;
    };
    breadth_first_process<T>(tree,p);

    //
    return found;
}


template<class T>
bool check_btree_in_order(typename BTree<T>::Ref const& tree)
{
    bool ret_val = true;
    if (!tree->is_empty())
    {
        if (!tree->left()->is_empty()) //Si está vacío, no hacec falta que entremos
        {
            auto maxValue = [=]() -> T { //Función lambda que calcula el valor máximo
                auto current = tree->left();
                while (!current->right()->is_empty())
                    current = current->right();
                return current->item();
            };
            if (maxValue() > tree->item()) //Comparamos con el valor máximo que hemos calculado. Si no se cumple, automáticamente el árbol no está ordenado
                ret_val = false;
        }

        if (!tree->right()->is_empty()) //Si está vacío, no hacec falta que entremos
        {
            auto minValue = [=]() -> T { //Función lambda que calcula el valor mínimo
                auto current = tree->right();
                while (!current->left()->is_empty())
                    current = current->left();
                return current->item();
            };
            if (minValue() < tree->item()) //Comparamos con el valor mínimo que hemos calculado. Si no se cumple, automáticamente el árbol no está ordenado
                ret_val = false;
        }

        if (!check_btree_in_order<T>(tree->left()) || !check_btree_in_order<T>(tree->right())) //Comprobamos que ninguno de los subárboles estén desordenados
            ret_val = false;
    }
    return ret_val;
}

template<class T>
bool has_in_order(typename BTree<T>::Ref tree, T const& v)
{
    assert(check_btree_in_order<T>(tree));    
    bool ret_val = false;
    if (!tree->is_empty()) //Si el árbol está vacío no entramos porque obviamente no habría nada que buscar
    {
        if (v < tree->item()) //Si el valor que buscamos es menor que el nodo raiz, buscamos por la rama izquierda ya que el árbol está ordenado
            ret_val = has_in_order<T>(tree->left(), v);
        else if (v > tree->item()) //Si el valor que buscamos es mayor que el nodo raiz, buscamos por la rama izquierda ya que el árbol está ordenado
            ret_val = has_in_order<T>(tree->right(), v);
        else //Si es igual, devolvemos true, ya que lo habremos encontrado
            ret_val = true;
    }
    return ret_val;
}

template <class T>
void insert_in_order(typename BTree<T>::Ref tree, T const& v)
{
    assert(check_btree_in_order<T>(tree));
     if (tree->is_empty()) //Si el árbol está vacío, creamos directamente el nodo raiz
        tree->create_root(v);

    else if (v < tree->item()) //Si el valor que queremos insertar es menor que el item del nodo raiz, vamos por la izquierda
    {
        if (tree->left()->is_empty())
            tree->set_left(BTree<T>::create(v));
        else
            insert_in_order<T>(tree->left(), v);
    }
    else if (v > tree->item()) //Si el valor que queremos insertar es mayor que el item del nodo raiz, vamos por la derecha
    {
        if (tree->right()->is_empty())
            tree->set_right(BTree<T>::create(v));
        else
            insert_in_order<T>(tree->right(), v);
    }
    assert(has_in_order<T>(tree, v));
}
