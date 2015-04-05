#include "bintree.hpp"

using namespace TSC;

/**
 * Construct a binary tree node with the given value.
 *
 * \param value
 * The value to encapsulate.
 */
Bintree::Bintree(unsigned long value)
{
    m_value = value;
    mp_left = NULL;
    mp_right = NULL;
}

Bintree::~Bintree()
{
    if (mp_left)
        delete mp_left;
    if (mp_right)
        delete mp_right;
}

/**
 * Insert a node into the binary tree. This method does the following:
 *
 * * If the given node’s value is greater than this node’s value:
 *   Insert on the right side.
 * * If the given node’s value is smaller than/equal to this node’s
 *   value: Insert on the left side.
 * * If the target side is occupied already: repeat until it isn’t.
 */
void Bintree::Insert(Bintree* p_bintree)
{
    if (p_bintree->Get_Value() > m_value) { // Must go on the right side
        if (mp_right) { // There’s a tree already
            mp_right->Insert(p_bintree);
        }
        else { // We’re a leaf node.
            mp_right = p_bintree;
        }
    }
    else { // Must go on the left side
        if (mp_left) { // There’s a tree already
            mp_left->Insert(p_bintree);
        }
        else { // We’re a leaf node.
            mp_left = p_bintree;
        }
    }
}

/**
 * Checks whether the given value is contained in this node or any
 * of its sub-nodes.
 *
 * \param[in] value
 * The value to check for.
 */
bool Bintree::Contains(const unsigned long& value)
{
    // Are we the target?
    if (value == m_value)
        return true;

    if (value > m_value) { // Must be on right side
        if (m_right) {
            return m_right->Contains(value);
        }
        else { // No right side, we’re a leaf node.
            return false;
        }
    }
    else { // Must be on left side
        if (m_left) {
            return m_left->Contains(value);
        }
        else { // No left side, we’re a leaf node.
            return false;
        }
    }
}
