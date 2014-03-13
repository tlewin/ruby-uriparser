require 'mkmf'
require 'rbconfig'

unless have_library('uriparser')
  error 'You need to install uriparser lib'
  exit
end

create_makefile 'uriparser_ext'