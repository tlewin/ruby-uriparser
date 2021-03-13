# frozen_string_literal: true

require 'mkmf'
require 'rbconfig'

unless have_library('uriparser')
  abort "-----\nERROR: You need to install uriparser lib.\n-----"
end

create_makefile 'uriparser_ext'
