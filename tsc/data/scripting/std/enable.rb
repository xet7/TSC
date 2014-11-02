# -*- coding: utf-8 -*-
module Std

  # This module adds the methods Sprite#disable and Sprite#enable to
  # the core sprite class. By fiddling with a sprite’s position these
  # methods can take a sprite effectively out of game so that it
  # doesn’t even block as if it were just invisible.
  #
  # Under the hood, the sprite is moved outside the visible level
  # area while the original position is remembered. On calling #enable,
  # the original position is restored on the sprite.
  module Enable

    # Take the sprite off the game. Apart from position
    # all information remains set.
    def disable
      @old_pos = start_pos
      warp(-100, 100)
    end

    # Restore the sprite and optionally set its massivity to +mass+;
    # if that is unset, it remains at whatever massivity it was before.
    def enable(mass = nil)
      start_at(*@old_pos)
      self.massive_type = mass if mass
    end

  end

end

Sprite.send(:include, Std::Enable)
