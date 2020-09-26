# frozen_string_literal: true

require 'uri'
require 'uriparser'

# wrapper the ruby URI class
module URI #:nodoc: all
  def self.parser(uri)
    UriParser.parse(uri)
  end
end

module Kernel #:nodoc: all
  def URI(uri) # rubocop:disable Naming/MethodName
    UriParser.parse(uri)
  end
end
