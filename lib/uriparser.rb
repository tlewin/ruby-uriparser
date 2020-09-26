# frozen_string_literal: true

$LOAD_PATH.push __dir__
require 'uriparser_ext'

module UriParser #:nodoc: all
  class URI
    def port
      @port ||= str_port.nil? ? nil : str_port.to_i
    end

    def port=(value)
      @port         = value.to_i
      self.str_port = (value.nil? ? nil : value.to_s)
    end

    def user
      return unless userinfo

      userinfo.split(':').first
    end

    def password
      return unless userinfo

      userinfo.split(':')[1]
    end
  end
end
