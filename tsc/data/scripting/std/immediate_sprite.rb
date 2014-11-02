# -*- coding: utf-8 -*-

module Std

  # More handy class than the core Sprite class. Instances of this class
  # do not need to have #show called on them to be displayed. As a downside,
  # you have to specify all position-relevant parameters directly on
  # instanciation with ::new.
  class ImmediateSprite < Sprite

    # Quick’n’easy way to bring a static object into the game. Specifying
    # position and massivity, this method also immediately <tt>show</tt>s
    # the object in gameplay.
    # === Parameters
    # [path]
    #   The path to the image file, relative to the +pixmaps+ directory.
    # [opts]
    #   A hash with the following arguments:
    #   [x]
    #     The X start position. Can contain fractions.
    #   [y]
    #     The Y start position. Can contain fractions.
    #   [massive (:massive)]
    #     The sprite’s massivity type. One of :massive, :passive, :halfmassive, :frontpassive, or :climbable.
    #     Defaults to :massive if ommited.
    def initialize(path, opts)
      x       = opts[:x]       || raise(ArgumentError, ":x not given")
      y       = opts[:y]       || raise(ArgumentError, ":y not given")
      massive = opts[:massive] || :massive

      sprite = super(path)
      sprite.start_at(x, y)
      sprite.massive_type = massive
      sprite.show

      sprite
    end

  end

end
