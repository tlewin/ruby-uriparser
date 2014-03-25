require 'test_helper'

class UriParserTest < Test::Unit::TestCase

  FULL_URI        = 'http://user:pwd@ruby-lang.org:80/en/about?yes=1#any'
  SIMPLE_URI      = 'https://ruby-lang.org'
  WEIRD_PATH_URI  = 'http://example.org/one/two/../../one'

  def setup
    @full_uri   = UriParser.parse(FULL_URI)
    @simple_uri = UriParser.parse(SIMPLE_URI)
    
    @manual_uri = UriParser::URI.new
    @manual_uri.scheme    = 'http'
    @manual_uri.host      = 'example.org'
    @manual_uri.path      = '/one/two/../'
    @manual_uri.fragment  = 'test'

    @weird_path_uri = UriParser.parse(WEIRD_PATH_URI)
  end

  context 'URI parsing' do
    should 'raise an exception if was unable to parse the URI' do
      assert_raise(StandardError) {UriParser.parse('invalid uri')}
    end

    should 'parse valid URI' do
      assert_equal @full_uri.scheme,    'http'
      assert_equal @full_uri.userinfo,  'user:pwd'
      assert_equal @full_uri.host,      'ruby-lang.org'
      assert_equal @full_uri.str_port,  '80'
      assert_equal @full_uri.path,      '/en/about'
      assert_equal @full_uri.query,     'yes=1'
      assert_equal @full_uri.fragment,  'any'
    end

    should 'paser valid but incomplete URI' do
      assert_equal @simple_uri.scheme,    'https'
      assert_equal @simple_uri.userinfo,  nil
      assert_equal @simple_uri.host,      'ruby-lang.org'
      assert_equal @simple_uri.str_port,  nil
      assert_equal @simple_uri.path,      ''
      assert_equal @simple_uri.query,     nil
      assert_equal @simple_uri.fragment,  nil
    end

    should 'return URI object after a successful parsing' do
      assert_instance_of UriParser::URI, @simple_uri
    end
  end

  context 'URI class API' do
    should 'allow update any URI field' do
      @full_uri.scheme    = 'ftp'
      @full_uri.userinfo  = nil
      @full_uri.host      = 'example.org'
      @full_uri.str_port  = nil
      @full_uri.path      = '/'
      @full_uri.query     = 'xx=11'
      @full_uri.fragment  = nil
      assert_equal @full_uri.scheme,    'ftp'
      assert_equal @full_uri.userinfo,  nil
      assert_equal @full_uri.host,      'example.org'
      assert_equal @full_uri.str_port,  nil
      assert_equal @full_uri.path,      '/'
      assert_equal @full_uri.query,     'xx=11'
      assert_equal @full_uri.fragment,  nil
    end

    should 'split userinfo data' do
      assert_equal @full_uri.user, 'user'
      assert_equal @full_uri.password, 'pwd'
    end

    should 'return nil if no user/password informed' do
      uri = UriParser.parse('http://ruby-lang.org:80/en/about?yes=1#any')
      assert_equal uri.user, nil
      assert_equal uri.password, nil
    end

    should 'port and str_port be synchronized' do
      @full_uri.port = 1
      assert_equal @full_uri.str_port, '1'
    end
  end

  context 'URI normalizing' do
    should 'normalize a valid URI' do
      @weird_path_uri.normalize!
      assert_equal @weird_path_uri.scheme,    'http'
      assert_equal @weird_path_uri.userinfo,  nil
      assert_equal @weird_path_uri.host,      'example.org'
      assert_equal @weird_path_uri.str_port,  nil
      assert_equal @weird_path_uri.path,      '/one'
      assert_equal @weird_path_uri.query,     nil
      assert_equal @weird_path_uri.fragment,  nil
    end

    should 'normalize an updated URI' do
      @simple_uri.path = '/one/two/../../one'
      assert_equal @simple_uri.path, '/one/two/../../one'
      @simple_uri.normalize!
      assert_equal @simple_uri.scheme,    'https'
      assert_equal @simple_uri.userinfo,  nil
      assert_equal @simple_uri.host,      'ruby-lang.org'
      assert_equal @simple_uri.str_port,  nil
      assert_equal @simple_uri.path,      '/one'
      assert_equal @simple_uri.query,     nil
      assert_equal @simple_uri.fragment,  nil
    end

    should 'normalize an URI manually created' do
      uri = UriParser::URI.new
      uri.scheme    = 'http'
      uri.host      = 'example.org'
      uri.path      = '/one/two/../'
      uri.fragment  = 'test'
      uri.normalize!
      assert_equal uri.scheme,    'http'
      assert_equal uri.userinfo,  nil
      assert_equal uri.host,      'example.org'
      assert_equal uri.str_port,  nil
      assert_equal uri.path,      '/one'
      assert_equal uri.query,     nil
      assert_equal uri.fragment,  'test'
    end
  end
end