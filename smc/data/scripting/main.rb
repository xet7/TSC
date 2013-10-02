# main.rb -- SMC MRuby manifest & toplevel file
#
# This file gets loaded by SMC everytime a new MRuby interpreter
# needs to be initialized. Anything placed here is automatically
# globally available in *all* levels (except local variables,
# of course). If you want, you can add your own code here,
# but please don't remove anything you don't know what it does.

# Don't touch this if you don't know exactly what you're doing.
# It loads all the SMC wrapper classes into the running MRuby
# interpeter instance. This must be the first statement in main.rb.
SMC.setup

# Turtle will be Armadillo in the future
Armadillo = Turtle
