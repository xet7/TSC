#ifndef TSC_LEVEL_PLAYER_HPP
#define TSC_LEVEL_PLAYER_HPP

namespace TSC {

    class cLevel_Player: public cSpriteActor
    {
    public:
        cLevel_Player();
        virtual ~cLevel_Player();

        virtual bool Handle_Collision(cCollision* p_collision);
    };
}

#endif
