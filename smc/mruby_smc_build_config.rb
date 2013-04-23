# Build configuration file for mruby.

# Absolute path to the directory this file resides in,
# independent of any PWD or invocation stuff.
THIS_DIR = File.expand_path(File.dirname(__FILE__))
# Absolute path mruby resides in
MRUBY_DIR = File.join(THIS_DIR, "..", "mruby", "mruby")

config = lambda do |conf, root|
  # Some standard things included with mruby
  conf.gem "#{root}/mrbgems/mruby-sprintf"
  conf.gem "#{root}/mrbgems/mruby-print"
  conf.gem "#{root}/mrbgems/mruby-math"
  conf.gem "#{root}/mrbgems/mruby-time"
  conf.gem "#{root}/mrbgems/mruby-struct"
  conf.gem "#{root}/mrbgems/mruby-enum-ext"
  conf.gem "#{root}/mrbgems/mruby-string-ext"
  conf.gem "#{root}/mrbgems/mruby-numeric-ext"
  conf.gem "#{root}/mrbgems/mruby-array-ext"
  conf.gem "#{root}/mrbgems/mruby-hash-ext"
  conf.gem "#{root}/mrbgems/mruby-range-ext"
  conf.gem "#{root}/mrbgems/mruby-proc-ext"
  conf.gem "#{root}/mrbgems/mruby-symbol-ext"
  conf.gem "#{root}/mrbgems/mruby-random"
  # conf.gem "#{root}/mrbgems/mruby-eval"

  # Generate mirb command
  conf.gem "#{root}/mrbgems/mruby-bin-mirb"

  # Additional things
  conf.gem "#{THIS_DIR}/../mruby/mgems/mruby-sleep"         # Sleep
  conf.gem "#{THIS_DIR}/../mruby/mgems/mruby-pcre-regexp"   # PCRE Regular Expressions
  conf.gem "#{THIS_DIR}/../mruby/mgems/mruby-md5"           # MD5
  conf.gem "#{THIS_DIR}/../mruby/mgems/mruby-json"          # JSON
end

MRuby::Build.new do |conf|
  toolchain :gcc
  config.call(conf, root)
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

    config.call(conf, root)
  end
end
