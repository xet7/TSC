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
     * longs.
     *
     * Note that deleting an instance of this class will recursively delete
     * all child nodes of that particular instance.
     *
     * \see [Wikipedia on Binary trees](https://en.wikipedia.org/wiki/Binary_tree)
     */
    class Bintree
    {
    public:
        Bintree(unsigned long value);
        ~Bintree();

        inline Bintree* Get_Left() const {return mp_left;}
        inline Bintree* Get_Right() const {return mp_right;}
        inline bool Is_Empty() const {return !mp_left && !mp_right;}

        const inline unsigned long& Get_Value() const {return m_value;}

        void Insert(Bintree* p_bintree);
        bool Contains(const unsigned long& value);
    private:
        Bintree* mp_left;
        Bintree* mp_right;

        unsigned long m_value;
    }

}
#endif
