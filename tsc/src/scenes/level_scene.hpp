#ifndef TSC_SCENES_LEVEL_SCENE_HPP
#define TSC_SCENES_LEVEL_SCENE_HPP
namespace TSC {

    class cLevelScene: public cScene
    {
    public:
        cLevelScene(const std::string& startlevel);
        virtual ~cLevelScene();

        virtual void Handle_Event(sf::Event& evt);
        virtual void Update();
        virtual void Draw(sf::RenderWindow& stage);
        virtual std::string Name() const;

        void Add_Level(const std::string& levelname);
        void Add_Level(cLevel* p_level);
        void Set_Current_Level(size_t i);
        inline cLevel* Get_Current_Level(){return mp_current_level;}

    private:
        std::vector<cLevel*> m_active_levels;
        cLevel* mp_current_level;
    };
}
#endif
