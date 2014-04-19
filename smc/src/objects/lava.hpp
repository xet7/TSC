#ifndef SMC_LAVA_HPP
#define SMC_LAVA_HPP
#include "animated_sprite.hpp"

namespace SMC {

	class cLava: public cAnimated_Sprite
	{
	public:
		cLava(cSprite_Manager* p_sprite_manager);
		cLava(XmlAttributes& attributes, cSprite_Manager* p_sprite_manager);
		virtual ~cLava();

		virtual cLava* Copy() const;
		virtual void Update();
		virtual void Draw(cSurface_Request* p_request = NULL);

		virtual xmlpp::Element* Save_To_XML_Node(xmlpp::Element* p_element);

		virtual void Handle_Collision_Player(cObjectCollision* p_collision);
		virtual void Handle_Collision_Enemy(cObjectCollision* p_collision);

	protected:
		virtual std::string Get_XML_Type_Name();

	private:
		void Init();
	};

}

#endif
