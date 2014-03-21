$LOAD_PATH.push File.expand_path(File.dirname(__FILE__))
require 'uriparser_ext'

module UriParser
  class URI
    def port
      @port ||= if str_port.nil?
        nil
      else
        str_port.to_i
      end
    end

    def port=value
      @port         = value.to_i
      self.str_port = (value.nil? ? nil : value.to_s)
    end

    def user
      if userinfo
        userinfo.split(':').first
      else
        nil
      end
    end

    def password
      if userinfo
        userinfo.split(':')[1]
      else
        nil
      end
    end
  end
end