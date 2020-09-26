# frozen_string_literal: true

require 'minitest/autorun'
require 'shoulda/context'
require 'shoulda/matchers'

require './lib/uriparser'

Shoulda::Matchers.configure do |config|
  config.integrate do |with|
    with.test_framework :minitest
  end
end
