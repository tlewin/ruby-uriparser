# frozen_string_literal: true

require 'rake/extensiontask'
require 'rake/testtask'

Rake::ExtensionTask.new('uriparser_ext')

desc 'run tests'
Rake::TestTask.new(test: :compile) do |test|
  test.libs << 'test'
  test.pattern = 'test/**/*_test.rb'
  test.verbose = true
end

task default: :test
