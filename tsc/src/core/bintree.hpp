#ifndef TSC_BINTREE_HPP
#define TSC_BINTREE_HPP
#include <cstdlib>
#include <functional>

namespace TSC {

    /**
     * Binary tree. This class is a simple implementation of a binary
     * tree structure, i.e. a container whose values are stored in
     * a structured way determined by their size. This allows to
     * to very fast checks as for whether a value is contained in
     * this container.
     *
     * The only values supported by this specific implementation are unsigned
     * longs. However, as each node also has an associated data reference,
     * this shouldn’t hurt. Just instanciate the template with your desired
     * type for the referenced data.
     *
     * Note that deleting an instance of this class will recursively delete
     * all child nodes of that particular instance.
     *
     * \see [Wikipedia on Binary trees](https://en.wikipedia.org/wiki/Binary_tree)
     */
    template<typename T>
    class Bintree
    {
    public:
        /**
         * Construct a binary tree node with the given value.
         *
         * \param value
         * The value to encapsulate.
         *
         * \param[in] data
         * Data to associate with this value. You can retrieve it from
         * the node again with Get_Data() and reset it with Set_Data().
         *
         * \remark When you delete the instance constructed by this method,
         * sub-nodes will also be deleted. *Not* deleted is the data pointer
         * (`data` argument) for this node as the object cannot know what it
         * is.
         */
        Bintree(unsigned long value, T* data)
            : mp_left(NULL), mp_right(NULL), m_value(value), mp_data(data)
        {
            //
        }

        ~Bintree()
        {
            if (mp_left)
                delete mp_left;
            if (mp_right)
                delete mp_right;
        }


        inline const Bintree* Get_Left() const {return mp_left;}
        inline const Bintree* Get_Right() const {return mp_right;}
        inline bool Is_Empty() const {return !mp_left && !mp_right;}

        inline T* Get_Data() const {return mp_data;}
        inline void Set_Data(T* data){mp_data = data;}

        inline const unsigned long& Get_Value() const {return m_value;}

        /**
         * Insert a node into the binary tree. This method does the following:
         *
         * * If the given node’s value is greater than this node’s value:
         *   Insert on the right side.
         * * If the given node’s value is smaller than/equal to this node’s
         *   value: Insert on the left side.
         * * If the target side is occupied already: repeat until it isn’t.
         */
        void Insert(Bintree<T>* p_bintree)
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
        bool Contains(const unsigned long& value)
        {
            // Are we the target?
            if (value == m_value)
                return true;

            if (value > m_value) { // Must be on right side
                if (mp_right) {
                    return mp_right->Contains(value);
                }
                else { // No right side, we’re a leaf node.
                    return false;
                }
            }
            else { // Must be on left side
                if (mp_left) {
                    return mp_left->Contains(value);
                }
                else { // No left side, we’re a leaf node.
                    return false;
                }
            }
        }

        /**
         * Returns the data associated with the given value.
         * If the value is not in the tree, returns NULL.
         *
         * \param[in] value
         * The value to check for.
         */
        T* Fetch(const unsigned long& value)
        {
            // Are we the target?
            if (value == m_value) {
                return mp_data;
            }

            if (value > m_value) { // Must be on right side
                if (mp_right) {
                    return mp_right->Fetch(value);
                }
                else { // No right side, we’re a leaf node.
                    return NULL;
                }
            }
            else { // Must be on left side
                if (mp_left) {
                    return mp_left->Fetch(value);
                }
                else { // No left side, we’re a leaf node.
                    return NULL;
                }
            }
        }

        /**
         * Iterate the entire binary tree. The elements are handed to
         * the callback in sorted order, i.e. `value` keeps incrementing
         * for each execution.
         *
         * \param cb
         * Callback function with signature `void cb(const unsigned long& value, T* p_data)`,
         * which receives the key value as its first argument, and the referenced
         * data as its second.
         */
        void Traverse(std::function<void (const unsigned long& value, T* p_data)> cb)
        {
            if (mp_left) {
                mp_left->Traverse(cb);
            }

            cb(m_value, mp_data);

            if (mp_right) {
                mp_right->Traverse(cb);
            }
        }
    private:
        Bintree<T>* mp_left;
        Bintree<T>* mp_right;

        unsigned long m_value;
        T* mp_data;
    };

}
#endif
