# ruby-uriparser

[![Gem Version](https://badge.fury.io/rb/ruby-uriparser.svg)](https://badge.fury.io/rb/ruby-uriparser)
[![Build Status](https://travis-ci.org/tlewin/ruby-uriparser.svg?branch=master)](https://travis-ci.org/tlewin/ruby-uriparser)
[![Ruby Style Guide](https://img.shields.io/badge/code_style-rubocop-brightgreen.svg)](https://github.com/rubocop-hq/rubocop)

Ruby-uriparser is a wrapper for [uriparser](http://uriparser.sourceforge.net/) C library which is fast (linear input length time complexity), approximatelly 7.5x faster than the standard ruby URI library with a low memory consumption.

It is a fairly reasonable hypothesis to consider that __all__ Ruby Rack applications will call, at least once, the `URI.parse` method for each http request, so this simple optimization can bring a significant impact on a high load environment. (See Benchmark)

## Usage

```ruby
require 'uriparser'

UriParser.parse('https://localhost:9000/path/?a=1#x') # return UriParser::URI object
```

If you want to override the URI class just include the following line in your code:

```ruby
# Will override URI#parse and Kernel#URI methods
require 'uriparser/uri_gem'
```

## Benchmark

The following numbers were computed for 100,000 `URI.parse` calls using the Ruby 2.6 benchmark library in a Mac OS X (10.15.6)/ 2,7 GHz Dual-Core Intel Core i5/ 16 GB 1867 MHz DDR3:

    # Complex URLs
    Rehearsal ------------------------------------------------
    URI            1.139526   0.007341   1.146867 (  1.189431)
    UriParser      0.148833   0.008094   0.156927 (  0.163185)
    Addressable    2.096471   0.008317   2.104788 (  2.152700)
    --------------------------------------- total: 3.408582sec

                    user     system      total        real
    URI            1.131435   0.007336   1.138771 (  1.215291)
    UriParser      0.202037   0.005046   0.207083 (  0.259723)
    Addressable    2.393879   0.019947   2.413826 (  2.676498)

    # Simple URLs
    Rehearsal ------------------------------------------------
    URI            0.623932   0.002970   0.626902 (  0.641151)
    UriParser      0.145798   0.005798   0.151596 (  0.162660)
    Addressable    2.222884   0.025276   2.248160 (  2.511593)
    --------------------------------------- total: 3.026658sec

                    user     system      total        real
    URI            0.702562   0.007480   0.710042 (  0.763542)
    UriParser      0.153022   0.004182   0.157204 (  0.170975)
    Addressable    1.879151   0.008923   1.888074 (  1.958248)

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
