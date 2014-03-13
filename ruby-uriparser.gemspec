require './lib/uriparser/version'

Gem::Specification.new do |s|
  s.name = 'ruby-uriparser'
  s.version = UriParser::VERSION
  s.platform = Gem::Platform::RUBY
  s.authors = ['Thiago Lewin']
  s.email = ['thiago_lewin@yahoo.com.br']
  s.homepage = 'https://github.com/tlewin'
  s.summary = 'Ruby wrapper for uriparser C library'
  s.description = s.summary
  s.require_paths = ['lib']
  s.files = Dir['**/*'] - ['ruby-uriparser-0.0.1.gem']
  s.extensions = ["ext/uriparser_ext/extconf.rb"]
  s.date = Time.now
end