# A door sprite combined with a level exit.
class Std::ExitDoor

  # The Sprite instance for the door.
  attr_reader :door
  # The LevelExit instance for the level exit.
  attr_reader :levelexit

  # Creates, place and show a door at the given position. A LevelExit
  # object is placed centered on the door.
  def initialize(x, y)
    @door      = Sprite.new("game/level/door_wood_1.png")
    @levelexit = LevelExit.new

    @door.massive_type = :passive
    @door.start_at(x, y)
    @levelexit.start_at(x + 25, y + 60)
    @door.show
    @levelexit.show
  end

end
