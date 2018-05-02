require 'rake/extensiontask'
require "bundler/gem_tasks"
require "rspec/core/rake_task"

RSpec::Core::RakeTask.new(:spec)

task :default => :spec

Rake::ExtensionTask.new "listpack" do |ext|
  ext.lib_dir = 'lib/listpack'
end
