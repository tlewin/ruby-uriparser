# ruby-uriparser

[![Gem Version](https://badge.fury.io/rb/ruby-uriparser.png)](http://badge.fury.io/rb/ruby-uriparser)
[![Build Status](https://travis-ci.org/tlewin/ruby-uriparser.svg?branch=master)](https://travis-ci.org/tlewin/ruby-uriparser)
[![Ruby Style Guide](https://img.shields.io/badge/code_style-rubocop-brightgreen.svg)](https://github.com/rubocop-hq/rubocop)

Ruby-uriparser is a wrapper for [uriparser](http://uriparser.sourceforge.net/) C library which is fast (linear input length time complexity), approximatelly 7.5x faster than the standard ruby URI library with a low memory consumption.

It is a fairly reasonable hypothesis to consider that __all__ Ruby Rack applications will call, at least once, the `URI.parse` method for each http request, so this simple optimization can bring a significant impact on a high load environment. (See Benchmark)

## Usage

    require 'uriparser'

    UriParser.parse('https://localhost:9000/path/?a=1#x') # return UriParser::URI object

If you want to override the URI class just include the following line in your code:

    # Will override URI#parse and Kernel#URI methods
    require 'uriparser/uri_gem'

## Benchmark

The following numbers were computed for 100,000 `URI.parse` calls using the ruby benchmark library in a Mac OS X (10.8.5)/ 2.9Ghz Intel Core I5/ 8GB 1600 MHz DDR3 machine:

    Rehearsal ------------------------------------------------
    URI            0.750000   0.000000   0.750000 (  0.750619)
    UriParser      0.110000   0.010000   0.120000 (  0.111885)
    Addressable    1.880000   0.000000   1.880000 (  1.886091)
    --------------------------------------- total: 2.750000sec

                       user     system      total        real
    URI            0.730000   0.000000   0.730000 (  0.728169)
    UriParser      0.110000   0.000000   0.110000 (  0.107235)
    Addressable    1.870000   0.000000   1.870000 (  1.863334)

## Installation

    gem install ruby-uriparser

## External dependencies

The ruby-uriparser gems needs the library [uriparser](http://uriparser.sourceforge.net/) installed to compile.

See the availables ports list: [http://uriparser.sourceforge.net/#ports]().

If you are on Mac OS X just run `brew install uriparser`.

## License

This project is released under the MIT license (See LICENSE file).

Copyright (c) 2014 Thiago Lewin - http://github.com/tlewin

## Third party libraries

This project includes code from the New BSD licensed:
* Copyright (C) 2007, Weijia Song <songweijia@gmail.com>
* Copyright (C) 2007, Sebastian Pipping <webmaster@hartwork.org>

All rights reserved.
