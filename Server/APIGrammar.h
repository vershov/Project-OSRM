/*

Copyright (c) 2013, Project OSRM, Dennis Luxen, others
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list
of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice, this
list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef APIGRAMMAR_H_
#define APIGRAMMAR_H_

#include <boost/bind.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_action.hpp>

namespace qi = boost::spirit::qi;

template <typename Iterator, class HandlerT>
struct APIGrammar : qi::grammar<Iterator> {
    APIGrammar(HandlerT * h) : APIGrammar::base_type(api_call), handler(h) {
        api_call = qi::lit('/') >> string[boost::bind(&HandlerT::setService, handler, ::_1)] >> *(query);
        query    = ('?') >> (+(zoom | output | jsonp | checksum | location | hint | cmp | language | instruction | geometry | alt_route | old_API | exec_time) ) ;

        zoom        = (-qi::lit('&')) >> qi::lit('z')            >> '=' >> qi::short_[boost::bind(&HandlerT::setZoomLevel, handler, ::_1)];
        output      = (-qi::lit('&')) >> qi::lit("output")       >> '=' >> string[boost::bind(&HandlerT::setOutputFormat, handler, ::_1)];
        jsonp       = (-qi::lit('&')) >> qi::lit("jsonp")        >> '=' >> stringwithPercent[boost::bind(&HandlerT::setJSONpParameter, handler, ::_1)];
        checksum    = (-qi::lit('&')) >> qi::lit("checksum")     >> '=' >> qi::int_[boost::bind(&HandlerT::setChecksum, handler, ::_1)];
        instruction = (-qi::lit('&')) >> qi::lit("instructions") >> '=' >> qi::bool_[boost::bind(&HandlerT::setInstructionFlag, handler, ::_1)];
        geometry    = (-qi::lit('&')) >> qi::lit("geometry")     >> '=' >> qi::bool_[boost::bind(&HandlerT::setGeometryFlag, handler, ::_1)];
        cmp         = (-qi::lit('&')) >> qi::lit("compression")  >> '=' >> qi::bool_[boost::bind(&HandlerT::setCompressionFlag, handler, ::_1)];
        location    = (-qi::lit('&')) >> qi::lit("loc")          >> '=' >> (qi::double_ >> qi::lit(',') >> qi::double_)[boost::bind(&HandlerT::addCoordinate, handler, ::_1)];
        hint        = (-qi::lit('&')) >> qi::lit("hint")         >> '=' >> stringwithDot[boost::bind(&HandlerT::addHint, handler, ::_1)];
        language    = (-qi::lit('&')) >> qi::lit("hl")           >> '=' >> string[boost::bind(&HandlerT::setLanguage, handler, ::_1)];
        alt_route   = (-qi::lit('&')) >> qi::lit("alt")          >> '=' >> qi::bool_[boost::bind(&HandlerT::setAlternateRouteFlag, handler, ::_1)];
        old_API     = (-qi::lit('&')) >> qi::lit("geomformat")   >> '=' >> string[boost::bind(&HandlerT::setDeprecatedAPIFlag, handler, ::_1)];
	exec_time   = (-qi::lit('&')) >> qi::lit("exec_time")  >> '=' >> qi::bool_[boost::bind(&HandlerT::setExecTimeFlag, handler, ::_1)];

        string            = +(qi::char_("a-zA-Z"));
        stringwithDot     = +(qi::char_("a-zA-Z0-9_.-"));
        stringwithPercent = +(qi::char_("a-zA-Z0-9_.-") | qi::char_('[') | qi::char_(']') | (qi::char_('%') >> qi::char_("0-9A-Z") >> qi::char_("0-9A-Z") ));
    }

    qi::rule<Iterator> api_call, query;
    qi::rule<Iterator, std::string()> service, zoom, output, string, jsonp, checksum, location, hint,
                                      stringwithDot, stringwithPercent, language, instruction, geometry,
                                      cmp, alt_route, old_API, exec_time;

    HandlerT * handler;
};

#endif /* APIGRAMMAR_H_ */
