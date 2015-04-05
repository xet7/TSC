#ifndef TSC_BINTREE_HPP
#define TSC_BINTREE_HPP
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
         * (`ptr` argument) for this node as the object cannot know what it
         * is.
         */
        Bintree(unsigned long value, T& data)
            : m_value(value), mp_left(NULL), mp_right(NULL), m_data(data);
        {
            //
        }
        ~Bintree();

        inline Bintree* Get_Left() const {return mp_left;}
        inline Bintree* Get_Right() const {return mp_right;}
        inline bool Is_Empty() const {return !mp_left && !mp_right;}

        inline T& Get_Data() const {return m_data;}
        inline void Set_Data(T& data){m_data = data;}

        const inline unsigned long& Get_Value() const {return m_value;}

        void Insert(Bintree* p_bintree);
        bool Contains(const unsigned long& value);
    private:
        Bintree* mp_left;
        Bintree* mp_right;

        unsigned long m_value;
        T& m_data;
    }

}
#endif
