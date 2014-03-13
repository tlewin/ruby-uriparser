# rupy-uriparser

Ruby-uriparser is a wrapper for [uriparser](http://uriparser.sourceforge.net/) C library which is fast (linear input length time complexity), approximatelly 7.5x faster than the standard ruby URI library with a low memory consumption.

It is a fairly reasonable hypothesis to consider that __all__ Ruby Rack applications will call, at least once, the `URI.parse` method for each http request, so this simple optimization can bring a significant impact on a high load environment. (See Benchmark)

## Usage

    require 'uriparser'

    UriParser.parse('https://localhost:9000/path/?a=1#x') # return UriParser::URI object

If you want to override the URI class just include the following line in your code:

    # Will override URI#parse and Kernel#URI methods
    require 'uriparser/uri_gem'

## Benchmark

The following numbers were computed for 10,000 `URI.parse` calls using the ruby benchmark library in a Mac OS X (10.8.5)/ 2.9Ghz Intel Core I5/ 8GB 1600 MHz DDR3 machine:

    Rehearsal ----------------------------------------------
    URI          0.080000   0.000000   0.080000 (  0.075157)
    UriParser    0.010000   0.000000   0.010000 (  0.009013)
    ------------------------------------- total: 0.090000sec

                     user     system      total        real
    URI          0.070000   0.000000   0.070000 (  0.073731)
    UriParser    0.000000   0.000000   0.000000 (  0.006310)

## Installation

    gem install ruby-uriparser

## External dependencies

The ruby-uriparser gems needs the library [uriparser](http://uriparser.sourceforge.net/) installed to compile.

See the availables ports list: [http://uriparser.sourceforge.net/#ports]().

If you are on Mac OS X just run `brew install uriparser`.

## License

This project includes code from the New BSD licensed:
Copyright (C) 2007, Weijia Song <songweijia@gmail.com>
Copyright (C) 2007, Sebastian Pipping <webmaster@hartwork.org>
All rights reserved.

This project is released under the MIT license (See LICENSE file).
Copyright (c) 2014 Thiago Lewin - http://github.com/tlewin