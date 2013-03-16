# -*- coding: utf-8 -*-
# main.rb -- SMC MRuby manifest & toplevel file
#
# This file gets loaded by SMC everytime a new MRuby interpreter
# needs to be initialized. Anything placed here is automatically
# globally available in *all* levels. If you want, you can add
# your own code here, but please don’t remove anything.
#
# You have access to a minimal #require method that is just
# able to load files below the directory this file resides in.

require "eventable"
