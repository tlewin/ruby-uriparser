# frozen_string_literal: true

require 'minitest/autorun'
require 'shoulda/context'
require 'shoulda/matchers'

$LOAD_PATH.unshift File.expand_path('../lib', __dir__)
require 'uriparser'

Shoulda::Matchers.configure do |config|
  config.integrate do |with|
    with.test_framework :minitest
  end
end
