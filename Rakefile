require 'rake/extensiontask'
require 'rake/testtask'

Rake::ExtensionTask.new('uriparser_ext')

desc 'run tests'
Rake::TestTask.new(:test => :'ext:build') do |test|
  test.libs << 'test'
  test.pattern = 'test/**/*_test.rb'
  test.verbose = true
end

namespace :ext do
  desc 'build extention'
  task :build => [:clobber, :compile]
end

task :default => :test