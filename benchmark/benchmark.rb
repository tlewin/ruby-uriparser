require 'benchmark'
require 'uri'
require 'addressable/uri'

$LOAD_PATH.unshift File.expand_path(File.dirname(__FILE__) + '/../lib')

require 'uriparser'


n = ARGV[0] ? ARGV[0].to_i : 10_000

Benchmark.bmbm(12) do |x|
  puts 'Complex URLs'
  x.report('URI') {n.times {URI.parse('https://www.ruby-lang.org/en/?a=1#test')}}
  x.report('UriParser') {n.times {UriParser.parse('https://www.ruby-lang.org/en/?a=1#test')}}
  x.report('Addressable') {n.times {Addressable::URI.parse('https://www.ruby-lang.org/en/?a=1#test')}}
end

puts "\n" << '#' * 20 << "\n"

Benchmark.bmbm(12) do |x|
  puts 'Simple URLs'
  x.report('URI') {n.times {URI('https://www.ruby-lang.org/en/')}}
  x.report('UriParser') {n.times {UriParser.parse('https://www.ruby-lang.org/en/')}}
  x.report('Addressable') {n.times {Addressable::URI.parse('https://www.ruby-lang.org/en/')}}
end