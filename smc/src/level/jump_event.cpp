#include "../luawrap.hpp"
#include "jump_event.h"

namespace SMC{
  namespace Script{

    void cJump_Event::Fire(lua_State* p_state, cLevel_Player* p_player)
    {
      for(std::vector<int>::const_iterator iter = p_player->m_event_table["on_jump"].begin(); iter < p_player->m_event_table["on_jump"].end(); iter++){
        lua_Lref(p
      }
    }

  };
};
