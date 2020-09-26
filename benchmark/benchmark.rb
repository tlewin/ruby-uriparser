# frozen_string_literal: true

require 'benchmark'
require 'uri'
require 'addressable/uri'

$LOAD_PATH.unshift __dir__

require 'uriparser'

n = ARGV[0] ? ARGV[0].to_i : 10_000

Benchmark.bmbm(12) do |x|
  puts 'Complex URLs'
  x.report('URI') do
    n.times { URI.parse('https://www.ruby-lang.org/en/?a=1#test') }
  end
  x.report('UriParser') do
    n.times { UriParser.parse('https://www.ruby-lang.org/en/?a=1#test') }
  end
  x.report('Addressable') do
    n.times { Addressable::URI.parse('https://www.ruby-lang.org/en/?a=1#test') }
  end
end

puts "\n" << '#' * 20 << "\n"

Benchmark.bmbm(12) do |x|
  puts 'Simple URLs'
  x.report('URI') do
    n.times { URI('https://www.ruby-lang.org/en/') }
  end
  x.report('UriParser') do
    n.times { UriParser.parse('https://www.ruby-lang.org/en/') }
  end
  x.report('Addressable') do
    n.times { Addressable::URI.parse('https://www.ruby-lang.org/en/') }
  end
end
