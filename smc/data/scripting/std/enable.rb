# -*- coding: utf-8 -*-
module Std

  # This module adds the methods Sprite#disable and Sprite#enable to
  # the core sprite class. By fiddling with a sprite’s massive type
  # and visibility information, these methods can take a sprite
  # effectively out of game so that it doesn’t even block as if it
  # were just invisible.
  module Enable

    # Take the sprite off the game. Apart from visibility
    # and massivity all information remains set.
    def disable
      @old_pos = start_pos
      warp(-100, 100)
    end

    # Restore the sprite and set its massivity to +mass+.
    def enable(mass = nil)
      start_at(*@old_pos)
      self.massive_type = mass if mass
    end

  end

end

Sprite.send(:include, Std::Enable)
