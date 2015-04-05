#ifndef TSC_COLLISION_HPP
#define TSC_COLLISION_HPP
namespace TSC {

    /**
     * Collision representation. An instance of this class encapsulates the
     * information about two collision rectangles that have intersected.
     * Collisions in TSC are always received by the moving object first,
     * because for performance reasons collision checks are only made
     * on objects that move. If both objects are moving, both objects
     * will get a collision; which one receives it first, is not determinable.
     * The moving object is referred to as the "collision causer" in this class,
     * whereas the standing object is the "collison sufferer". The collision
     * side detection methods by this class always ask the question on which
     * side the suffering rectangle has been hit by the causing rectangle.
     *
     * If the collision rectangles are not on the same line, but set off,
     * more than one of the collision edge methods may return true. By
     * "set off" I mean a situation such as this:
     *
     * ~~~~~~~~~~~~~~~~~~~~~~~~~
     *        ############
     *        #          #
     *        #          #
     *        #    R2    #
     *        #          #
     *    ########       #
     *    #      #       #
     *    #  R1  #########
     *    #      #
     *    ########
     * ~~~~~~~~~~~~~~~~~~~~~~~~~
     *
     * Where _R1_ is the causing (i.e. moving) rectangle, and _R2_ is the suffering,
     * (i.e. standing) rectangle. In that case, both Is_Collision_Bottom() and
     * Is_Collision_Left() will return true.
     */
    class cCollision
    {
    public:
        cCollision(cActor& collision_causer, cActor& collision_sufferer);

        inline cActor& Get_Collision_Causer() const {return m_collision_causer;}
        inline cActor& Get_Collision_Sufferer() const {return m_collision_sufferer;}

        bool Is_Collision_Top() const;
        bool Is_Collision_Bottom() const;
        bool Is_Collision_Left() const;
        bool Is_Collision_Right() const;
    private:
        cActor& m_collision_causer;
        cActor& m_collision_sufferer;
        sf::FloatRect m_causer_colrect;
        sf::FloatRect m_sufferer_colrect;
    };

}
#endif
