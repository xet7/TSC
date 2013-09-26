# -*- coding: utf-8 -*-
# main.rb — SMC MRuby manifest & toplevel file
#
# This file gets loaded by SMC everytime a new MRuby interpreter
# needs to be initialized. Anything placed here is automatically
# globally available in *all* levels (except local variables,
# of course). If you want, you can add your own code here,
# but please don’t remove anything you don’t know what it does.
#
# You have access to a minimal SMC::require method that is just
# able to load files below the scripting/ directory of SMC.
# If you want to write your own SMC scripting expansion pack,
# please make it install in a subdirectory of scripting/ to
# prevent naming conflicts with other scripting expansion packs.
# Then tell your users to add one or more SMC.require lines
# to this file.
#
# Users that just want to make code globally available in SMC
# can place files directly in the scripting/ directory and
# load the file like this:
#
#   SMC.require "yourfile"
#   SMC.require "yourotherfile"
#
# Note calls to SMC.require can be nested, i.e. the files loaded
# by SMC.require can request other files to be read by calling
# SMC.require themselves again.

# Don’t touch this if you don’t know exactly what you’re doing.
# It loads all the SMC wrapper classes into the running MRuby
# interpeter instance.
SMC.setup

# Secret Maryo Chronicles core extension pack. These are helpers
# you usually don’t want to disable.
SMC.require "core/aliases"
