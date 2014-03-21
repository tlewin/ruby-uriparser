require 'test_helper'

class UriParserTest < Test::Unit::TestCase

  context 'URI parsing' do
    should 'raise an exception if was unable to parse the URI' do
      assert_raise(StandardError) {UriParser.parse('invalid uri')}
    end

    should 'parse valid URI' do
      uri = UriParser.parse('http://user:pwd@ruby-lang.org:80/en/about?yes=1#any')
      assert_equal uri.scheme,    'http'
      assert_equal uri.userinfo,  'user:pwd'
      assert_equal uri.host,      'ruby-lang.org'
      assert_equal uri.str_port,  '80'
      assert_equal uri.path,      '/en/about'
      assert_equal uri.query,     'yes=1'
      assert_equal uri.fragment,  'any'
    end

    should 'paser valid but incomplete URI' do
      uri = UriParser.parse('https://ruby-lang.org')
      assert_equal uri.scheme,    'https'
      assert_equal uri.userinfo,  nil
      assert_equal uri.host,      'ruby-lang.org'
      assert_equal uri.str_port,  nil
      assert_equal uri.path,      ''
      assert_equal uri.query,     nil
      assert_equal uri.fragment,  nil
    end

    should 'return URI object after a successful parsing' do
      uri = UriParser.parse('https://ruby-lang.org')
      assert_instance_of UriParser::URI, uri
    end
  end

  context 'URI class API' do
    should 'allow update any URI field' do
      uri = UriParser.parse('http://user:pwd@ruby-lang.org:80/en/about?yes=1#any')
      uri.scheme    = 'ftp'
      uri.userinfo  = nil
      uri.host      = 'example.org'
      uri.str_port  = nil
      uri.path      = '/'
      uri.query     = 'xx=11'
      uri.fragment  = nil
      assert_equal uri.scheme,    'ftp'
      assert_equal uri.userinfo,  nil
      assert_equal uri.host,      'example.org'
      assert_equal uri.str_port,  nil
      assert_equal uri.path,      '/'
      assert_equal uri.query,     'xx=11'
      assert_equal uri.fragment,  nil
    end

    should 'split userinfo data' do
      uri = UriParser.parse('http://user:pwd@ruby-lang.org:80/en/about?yes=1#any')
      assert_equal uri.user, 'user'
      assert_equal uri.password, 'pwd'
    end

    should 'return nil if no user/password informed' do
      uri = UriParser.parse('http://ruby-lang.org:80/en/about?yes=1#any')
      assert_equal uri.user, nil
      assert_equal uri.password, nil
    end

    should 'port and str_port be synchronized' do
      uri = UriParser.parse('http://ruby-lang.org:80/en/about?yes=1#any')
      uri.port = 1
      assert_equal uri.str_port, '1'
    end
  end

  context 'URI normalizing' do
    should 'normalize a valid URI' do
      uri = UriParser.parse('http://example.org/one/two/../../one')
      uri.normalize!
      assert_equal uri.scheme,    'http'
      assert_equal uri.userinfo,  nil
      assert_equal uri.host,      'example.org'
      assert_equal uri.str_port,  nil
      assert_equal uri.path,      '/one'
      assert_equal uri.query,     nil
      assert_equal uri.fragment,  nil
    end

    should 'normalize an updated URI' do
      uri = UriParser.parse('http://example.org')
      uri.path = '/one/two/../../one'
      assert_equal uri.path, '/one/two/../../one'
      uri.normalize!
      assert_equal uri.scheme,    'http'
      assert_equal uri.userinfo,  nil
      assert_equal uri.host,      'example.org'
      assert_equal uri.str_port,  nil
      assert_equal uri.path,      '/one'
      assert_equal uri.query,     nil
      assert_equal uri.fragment,  nil
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