# Build configuration file for mruby.

# Absolute path to the directory this file resides in,
# independent of any PWD or invocation stuff.
THIS_DIR = File.expand_path(File.dirname(__FILE__))
# Absolute path mruby resides in
MRUBY_DIR = File.join(THIS_DIR, "..", "mruby", "mruby")

config = lambda do |conf|
  # Some standard things included with mruby
  conf.gem "#{MRUBY_DIR}/mrbgems/mruby-print" # #p, #puts, #print
  conf.gem "#{MRUBY_DIR}/mrbgems/mruby-math" # Math
  conf.gem "#{MRUBY_DIR}/mrbgems/mruby-time" # Time
  conf.gem "#{MRUBY_DIR}/mrbgems/mruby-struct" # Struct
  conf.gem "#{MRUBY_DIR}/mrbgems/mruby-sprintf" # #sprintf
  conf.gem "#{MRUBY_DIR}/mrbgems/mruby-enum-ext" # Enumerable
  conf.gem "#{MRUBY_DIR}/mrbgems/mruby-string-ext" # Strings
  conf.gem "#{MRUBY_DIR}/mrbgems/mruby-numeric-ext" # Numbers
  conf.gem "#{MRUBY_DIR}/mrbgems/mruby-array-ext" # Arrays
  conf.gem "#{MRUBY_DIR}/mrbgems/mruby-hash-ext" # Hashes
  conf.gem "#{MRUBY_DIR}/mrbgems/mruby-random" # Random

  # Additional things
  conf.gem "#{THIS_DIR}/../mruby/mgems/mruby-sleep"         # Sleep
  conf.gem "#{THIS_DIR}/../mruby/mgems/mruby-pcre-regexp"   # PCRE Regular Expressions
  conf.gem "#{THIS_DIR}/../mruby/mgems/mruby-md5"           # MD5
  conf.gem "#{THIS_DIR}/../mruby/mgems/mruby-json"          # JSON
end

MRuby::Build.new do |conf|
  toolchain :gcc
  config.call(conf)
end

if ENV["CROSSCOMPILE_TARGET"] and !ENV["CROSSCOMPILE_TARGET"].empty?
  prefix = ENV["CROSSCOMPILE_TARGET"]

  MRuby::CrossBuild.new(prefix) do |conf|
    toolchain :gcc

    conf.cc do |cc|
      cc.command = ENV["CC"] || "#{prefix}-gcc"
    end

    conf.linker do |linker|
      linker.command = ENV["LD"] || "#{prefix}-gcc"
    end

    conf.archiver do |archiver|
      archiver.command = ENV["AR"] || "#{prefix}-ar"
    end

    config.call(conf)
  end
end
