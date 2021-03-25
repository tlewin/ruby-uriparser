# frozen_string_literal: true

require 'mkmf'
require 'rbconfig'

have_library('uriparser') || abort("-----\nERROR: You need to install uriparser lib.\n-----")

create_makefile 'uriparser_ext'
