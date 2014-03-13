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
      @port     = value.to_i
      str_port  = value
    end

    def user
      userinfo.split(':').first
    end

    def password
      userinfo.split(':')[1]
    end
  end
end