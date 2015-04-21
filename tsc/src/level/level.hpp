#ifndef TSC_LEVEL_HPP_HPP
#define TSC_LEVEL_HPP_HPP
namespace TSC {

    /**
     * This class represents a level that is being played.
     */
    class cLevel {
    public:
        static cLevel* Load_From_File(boost::filesystem::path levelfile);

#ifdef _DEBUG
        static cLevel* Construct_Debugging_Level();
#endif

        cLevel(boost::filesystem::path levelfile);
        ~cLevel();

        bool operator==(const cLevel& other) const;
        bool operator!=(const cLevel& other) const;

        void Update();
        void Draw(sf::RenderWindow& stage) const;

        inline std::string Name() const {return path_to_utf8(m_levelfile.stem());}
        void Sort_Z_Elements();

        void Add_Actor(cActor* p_actor, const unsigned long& uid = 0);
        void Check_Collisions_For_Actor(cActor& actor);

        inline cImage_Manager* Get_ImageManager(){return mp_img_manager;}

    private:
        cLevel(); // Private constructor
        void Init();
        void Add_Collision_If_Required(cCollision* p_collision);
        /// Grab the next free UID, adjusting the level’s internal mantissa.
        inline unsigned long Get_Next_UID(){return ++m_last_max_uid;}
        /**
         * Reset the level’s internal UID mantissa. You need this when
         * you assigned an UID directly rather than used Get_Next_UID().
         * This method only allows to raise the mantissa, it cannot be
         * lowered to prevent UID clashes.
         */
        inline void Adjust_UID_Mantissa(const unsigned long& new_max){if (new_max > m_last_max_uid) m_last_max_uid = new_max;}

        boost::filesystem::path m_levelfile;
        int m_engine_version;
        sf::Vector2f m_player_startpos;
        sf::FloatRect m_camera_limits;

        Bintree<cCollision> m_collisions; //< Detected collisions from the current frame.
        unsigned long m_last_max_uid;

        std::vector<cActor*> m_actors;
        /// Level backgrounds.
        std::vector<sf::Sprite*> m_backgrounds;

        cImage_Manager* mp_img_manager; // Pointer so not everyone using levels has to include image manager files
    };

}
#endif
