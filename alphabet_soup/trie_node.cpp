/**
 * @file trie_node.cpp
 *
 * CopyRight F. J. Madrid-Cuevas <fjmadrid@uco.es>
 *
 * Sólo se permite el uso de este código en la docencia de las asignaturas sobre
 * Estructuras de Datos de la Universidad de Córdoba.
 *
 * Está prohibido su uso para cualquier otro objetivo.
 */

#include <cassert>
#include <sstream>
#include <cstdint>

#include "trie_node.hpp"

TrieNode::TrieNode(bool is_key_state)
{
    // TODO
    key = is_key_state;
    current = nullptr;
    //
    assert(is_key() == is_key_state);
    assert(!current_exists());
};

TrieNode::Ref TrieNode::create(bool is_key_state)
{
    return std::make_shared<TrieNode>(is_key_state);
}

TrieNode::~TrieNode()
{
}

bool TrieNode::is_key() const
{
    if (!key)
        return false;
    else
        return true;
}

bool TrieNode::has(char k) const
{
    if (children.find(k) != children.end())
        return true;
    return false;
}

TrieNode::Ref
TrieNode::child(char k) const
{
    assert(has(k));
    return children.at(k);
}

bool TrieNode::current_exists() const
{
    if (current == nullptr)
        return false;
    else
    {
        for (auto it = children.begin(); it != children.end(); it++)
        {
            if (it->second == current)
            {
                return true;
            }
        }
    }
    return false;
}

TrieNode::Ref
TrieNode::current_node() const
{
    assert(current_exists());
    return current;
}

char TrieNode::current_symbol() const
{
    assert(current_exists());
    char symbol = 0;
    for (auto it = children.begin(); it != children.end(); it++)
    {
        if (it->second == current)
        {
            symbol = it->first;
            break;
        }
    }
    return symbol;
}

void TrieNode::set_is_key_state(bool new_state)
{
    key = new_state;
    assert(is_key() == new_state);
}

bool TrieNode::find_child(char s)
{
    bool found = false;
    auto iter = children.find(s);

    if (iter != children.end())
    {
        found = true;
        current = iter->second;
    }
    else
    {
        found = false;
        current = nullptr;
    }

    assert(found || !current_exists());
    assert(!found || current_symbol() == s);
    return found;
}

void TrieNode::goto_first_child()
{
    current = children.begin()->second;
}

void TrieNode::goto_next_child()
{
    assert(current_exists());
    for (auto it = children.begin(); it != children.end(); it++)
    {
        if (it->second == current)
        {
            it++;
            if (it == children.end())
                current = nullptr;
            else
                current = it->second;
            break;
        }
    }
}

void TrieNode::set_child(char k, Ref node)
{
    assert(node != nullptr);
    children[k] = node;
    current = node;
    assert(current_symbol() == k);
    assert(current_node() == node);
}

std::ostream &
TrieNode::fold(std::ostream &out) const
{
    out << "[ ";
    if (is_key())
    {
        out << "T";
    }
    else
    {
        out << "F";
    }
    for (auto it = children.begin(); it != children.end(); it++)
    {
        out << " " << std::hex << (uint16_t)it->first << " ";
        it->second->fold(out);
    }
    out << " ]";

    return out;
}

TrieNode::Ref TrieNode::create(std::istream &in) noexcept(false)
{
    TrieNode::Ref node = nullptr;

    std::string token;
    in >> token;
    if (token != "[")
        throw std::runtime_error("Wrong input format");

    // Comprobar que existe key
    in >> token;
    if (token == "T")
    {
        node = TrieNode::create(true);
    }
    else if (token == "F")
    {
        node = TrieNode::create(false);
    }
    else
    {
        throw std::runtime_error("Wrong input format");
    }
    // Llamada recursiva para cada hijo
    while (in >> token && token != "]")
    {
        node->set_child(char(std::stoi(token, nullptr, 16)), TrieNode::create(in));
    }
    if (token != "]")
    {
        throw std::runtime_error("Wrong input format");
    }

    return node;
}
