require 'rake/extensiontask'
require "bundler/gem_tasks"
require "rspec/core/rake_task"

RSpec::Core::RakeTask.new(:spec)

task :default => :spec

task :spec => :compile

gem_spec = Gem::Specification.load('listpack.gemspec')
Rake::ExtensionTask.new("listpack_c", gem_spec) do |ext|
  ext.lib_dir = 'lib/listpack'
  ext.ext_dir = 'ext/listpack'
end
