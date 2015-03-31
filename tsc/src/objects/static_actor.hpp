#ifndef TSC_STATIC_ACTOR_HPP
#define TSC_STATIC_ACTOR_HPP

namespace TSC {

    /**
     * Actors that don’t move and don’t do much interaction. This
     * is notably the case for ground tiles.
     */
    class cStaticActor: public cActor
    {
    public:
        cStaticActor(boost::filesystem::path texture_path);
        virtual ~cStaticActor();
    };

}

#endif
