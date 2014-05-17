# _std.rb -- standard extension manifest & loading file.
# This file reads in all files from the standard scripting
# extension for SMC.

# Namespace for non-core (read non-C++) stuff for SMC
# scripting.
module Std
end

SMC.require "std/enable"
SMC.require "std/exit_door"
SMC.require "std/immediate_sprite"
