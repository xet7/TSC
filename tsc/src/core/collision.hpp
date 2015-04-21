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
     * side detection methods by this class always ask the question which
     * side of the causing rectangle collided with the suffering rectangle (i.e.
     * they act from the point of view of the moving rectangle).
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
     * (i.e. standing) rectangle. In that case, both Is_Collision_Top() and
     * Is_Collision_Right() will return true.
     */
    class cCollision
    {
    public:
        cCollision(cActor* p_collision_causer, cActor* p_collision_sufferer);

        inline cActor* Get_Collision_Causer() const {return mp_collision_causer;}
        inline cActor* Get_Collision_Sufferer() const {return mp_collision_sufferer;}

        void Invert();

        bool Is_Collision_Top() const;
        bool Is_Collision_Bottom() const;
        bool Is_Collision_Left() const;
        bool Is_Collision_Right() const;
    private:
        cActor* mp_collision_causer;
        cActor* mp_collision_sufferer;
        sf::FloatRect m_causer_colrect;
        sf::FloatRect m_sufferer_colrect;
    };

}
#endif
