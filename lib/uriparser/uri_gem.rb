require 'uri'
require 'uriparser'

# wrapper the ruby URI class
module URI
  def self.parser uri
    UriParser.parse(uri)
  end
end

module Kernel
  def URI(uri)
    UriParser.parse(uri)
  end
end
