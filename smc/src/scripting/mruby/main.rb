# -*- coding: utf-8 -*-
# main.rb -- SMC MRuby manifest & toplevel file
#
# This file gets loaded by SMC everytime a new MRuby interpreter
# needs to be initialized. Anything placed here is automatically
# globally available in *all* levels. If you want, you can add
# your own code here, but please don’t remove anything.
#
# You have access to a minimal SMC::require method that is just
# able to load files below the scripting/ directory of SMC.

#SMC.require "yourfile"
#SMC.require "yourotherfile"

# Don’t touch this if you don’t know exactly what you’re doing.
# It loads all the SMC wrapper classes into the running MRuby
# interpeter instance.
SMC.setup
