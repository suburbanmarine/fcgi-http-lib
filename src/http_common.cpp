/**
 * This file is part of fcgi-http-lib, a library with utilities for handling basic HTTP requests over FCGI.
 * 
 * This software is distrubuted in the hope it will be useful, but without any warranty, including the implied warrranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See LICENSE.txt for details.
 * 
 * @author Jacob Schloss <jacob.schloss@suburbanmarine.io>
 * @copyright Copyright (c) 2021 Suburban Marine, Inc. All rights reserved.
 * @license Licensed under the LGPL-3.0 license. See LICENSE.txt for details.
 * SPDX-License-Identifier: LGPL-3.0-only
*/

#include "http-bridge/http_common.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <cctype>
#include <cstring>

constexpr std::array<std::pair<char const * const, http_common::REQUEST_METHOD>, 9> http_common::REQUEST_METHOD_STR;

char const * http_common::get_status_code_str(const STATUS_CODES& status_code)
{
    char const* msg = "";
    switch(status_code)
    {
        default:
        {
            msg = "unknown";
            break;
        }
    }
    return msg;
}
http_common::REQUEST_METHOD http_common::parse_req_method(const char req_method[])
{
    if(strlen(req_method) != 0)
    {
        for(auto str : REQUEST_METHOD_STR)
        {
            if(strcasecmp(str.first, req_method) == 0)
            {
                return str.second;
            }
        }
    }

    return REQUEST_METHOD::INVALID;
}

bool http_common::parse_content_type(const char content_type[], Content_type* const type)
{
    if( (!content_type) || (content_type[0]=='\0') )
    {
        return false;
    }

    std::string tmp = content_type;

    boost::algorithm::trim_if(tmp, ::isspace);

    std::list<std::string> split_list;
    boost::split(split_list, tmp, boost::is_any_of(";"));

    if(split_list.empty())
    {
        return false;
    }

    type->media_type = split_list.front();
    boost::algorithm::trim_if(type->media_type, ::isspace);
    boost::to_lower(type->media_type);
    split_list.pop_front();

    std::vector<std::string> directive_list;
    for(const std::string& chunk : split_list)
    {
        boost::split(directive_list, chunk, boost::is_any_of("="));

        if(directive_list.empty())
        {
            continue;
        }

        boost::algorithm::trim_if(directive_list[0], ::isspace);

        if(directive_list[0] == "charset")
        {
            if(directive_list.size() == 2)
            {
                type->charset = directive_list[1];
                boost::algorithm::trim_if(type->charset, ::isspace);
                boost::to_lower(type->charset);
            }
            else
            {
                return false;
            }
        }
        else if(directive_list[0] == "boundary")
        {
            if(directive_list.size() == 2)
            {
                type->boundary = directive_list[1];
                boost::algorithm::trim_if(type->boundary, ::isspace);
            }
            else
            {
                return false;
            }
        }
        else
        {
            //unk
        }
    }

    return true;
}
