# frozen_string_literal: true

require 'mkmf'
require 'rbconfig'

abort "-----\nERROR: You need to install uriparser lib.\n-----" unless have_library('uriparser')

create_makefile 'uriparser_ext'
