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
         * sub-nodes will also be deleted. *Not* deleted is the wrapped data
         * (`data` argument) for this node as the object cannot know what it
         * is.
         */
        Bintree(unsigned long value, T data)
            : mp_left(NULL), mp_right(NULL), m_value(value), m_data(data)
        {
            //
        }

        /**
         * Default constructor. The constructed binary tree node has the
         * value 0 and its mapped data is determined by the data type’s
         * default constructor. The default value returned by Fetch() is
         * constructed witht he wrapped data types default constructor as well.
         *
         * This should not be used if possible. Instanciate by using the
         * other constructor which takes both the value and the data.
         * Reason is that apart from the assignment operator=() you can't
         * change the value of a binary tree node after its construction.
         */
        Bintree()
            : mp_left(NULL), mp_right(NULL), m_value(0)
        {
            //
        }

        /**
         * Copy constructor. This is a *recursive* (!) copy. The data object
         * is copied by use of its copy constructor.
         */
        Bintree(const Bintree<T>& other)
            : mp_left(NULL), mp_right(NULL), m_value(other.m_value), m_data(other.m_data)
        {
            if (other.mp_left)
                mp_left = new Bintree<T>(*other.mp_left);
            if (other.mp_right)
                mp_right = new Bintree<T>(*other.mp_right);
        }

        ~Bintree()
        {
            if (mp_left)
                delete mp_left;
            if (mp_right)
                delete mp_right;
        }

        /**
         * Assignment operator. Copies the contents of `rhs` *recursively*
         * into this binary tree instance.
         *
         * It should be obvious that it is a bad idea to call this on
         * a binary tree node that is not a toplevel node. Best way to
         * get your binary tree mixed up out of order.
         */
        Bintree<T>& operator=(const Bintree<T>& rhs)
        {
            if (this != &rhs) { // Skip self-assignment.
                if (mp_left)
                    delete mp_left;
                if (mp_right)
                    delete mp_right;

                mp_left  = NULL;
                mp_right = NULL;

                m_value = rhs.m_value;
                m_data  = rhs.m_data;

                if (rhs.mp_left) {
                    mp_left = new Bintree<T>(*rhs.mp_left); // copy constructor for recursive copy
                }
                if (rhs.mp_right) {
                    mp_right = new Bintree<T>(*rhs.mp_right); // copy constructor for recursive copy
                }
            }

            return *this;
        }

        inline const Bintree* Get_Left() const {return mp_left;}
        inline const Bintree* Get_Right() const {return mp_right;}
        inline bool Is_Empty() const {return !mp_left && !mp_right;}

        inline T& Get_Data() {return m_data;}
        inline const T& Get_Data() const {return m_data;} // Can't hand out non-const ref into const object
        inline void Set_Data(T& data){m_data = data;}

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
        bool Contains(const unsigned long& value) const
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
         * Returns a pointer to the data associated with the given
         * value.  If the value is not in the tree, returns the
         * default value passed as an argument.
         *
         * \param[in] value
         * The value to check for.
         *
         * \param[in] defaultvalue
         * This is returned if no match is found. If you wrap a pointer,
         * you will usually want to set this to NULL to get a NULL pointer
         * back when nothing is found.
         *
         * \returns Reference to the data object. The reference is not
         * `const`, which means you can modify it (which is intended),
         * and which means it’s impossible to make this function
         * `const`.
         *
         * \remark There is an overload of this function that guarantees
         * a const `this` by returning a `const` reference instead of
         * an ordinary reference.
         */
        T& Fetch(const unsigned long& value, T& defaultvalue)
        {
            // Are we the target?
            if (value == m_value) {
                return m_data;
            }

            if (value > m_value) { // Must be on right side
                if (mp_right) {
                    return mp_right->Fetch(value);
                }
                else { // No right side, we’re a leaf node.
                    return defaultvalue;
                }
            }
            else { // Must be on left side
                if (mp_left) {
                    return mp_left->Fetch(value);
                }
                else { // No left side, we’re a leaf node.
                    return defaultvalue;
                }
            }
        }

        /**
         * The Fetch() method doesn’t do anything to the actual `this`
         * object, but technical restrictions forbid to hand out a
         * non-const reference to something inside the object from a
         * const function (the caller could modify the reference,
         * which would cause `this` to indirectly change). If you have
         * no intention to modify the returned reference, use this
         * overload instead.
         *
         * The returned reference is `const`. You can’t modify it, and hence
         * we can guarantee that `this` is also `const` when calling this function.
         */
        inline const T& Fetch(const unsigned long& value, const T& defaultvalue) const
        {
            return Fetch(value, defaultvalue);
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
        void Traverse(std::function<void (const unsigned long& value, T& data)> cb)
        {
            if (mp_left) {
                mp_left->Traverse(cb);
            }

            cb(m_value, m_data);

            if (mp_right) {
                mp_right->Traverse(cb);
            }
        }

        /**
         * Clear this tree. Clearing means to recursively delete
         * the left and right nodes of this tree, the referenced
         * data pointers are of course not deleted as the Bintree
         * cannot know who manages their memory.
         *
         * After this method has been called, Is_Empty() returns
         * true.
         */
        void Clear()
        {
            if (mp_left)
                delete mp_left;
            if (mp_right)
                delete mp_right;

            mp_left = NULL;
            mp_right = NULL;
        }
    private:
        Bintree<T>* mp_left;
        Bintree<T>* mp_right;

        unsigned long m_value;
        T m_data;
    };

}
#endif
