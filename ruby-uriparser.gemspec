require './lib/uriparser/version'

Gem::Specification.new do |s|
  s.name          = 'ruby-uriparser'
  s.version       = UriParser::VERSION
  s.date          = Time.now
  s.platform      = Gem::Platform::RUBY
  s.authors       = ['Thiago Lewin']
  s.email         = ['thiago_lewin@yahoo.com.br']
  s.homepage      = 'https://github.com/tlewin/ruby-uriparser'
  s.summary       = 'Ruby wrapper for uriparser C library'
  s.description   = s.summary
  s.require_paths = ['lib']
  s.files         = Dir['LICENSE', 'README.md', 'Gemfile', 'Rakefile', 'lib/**/*rb', 'ext/**/*'].select! {|item| !File.directory?(item)}
  s.test_files    = Dir['test/**/*']
  s.extensions    = ["ext/uriparser_ext/extconf.rb"]
  s.license       = 'MIT'

  s.add_development_dependency 'shoulda', '~> 3.5', '>= 3.5.0'
end