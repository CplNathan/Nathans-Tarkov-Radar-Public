#pragma once
#include "../vmread/hlapi/hlapi.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string>
#include <codecvt>
#include <locale>
#include <random>
#include <chrono>
#include <list>
#include <sstream>
#include <iomanip>
#include <map>
#include "inttypes.h"

struct Vector4
{
    float x, y, z, d;
};

struct Vector3
{
    float x, y, z;
};

struct Vector2
{
    float x, y;
};

struct Matrix3x4
{
    Vector4 A;
    Vector4 B;
    Vector4 C;
};

std::string escape_json(const std::string &s)
{
    std::ostringstream o;
    for (auto c = s.cbegin(); c != s.cend(); c++)
    {
        switch (*c)
        {
        case '"':
            o << "\\\"";
            break;
        case '\\':
            o << "\\\\";
            break;
        case '\b':
            o << "\\b";
            break;
        case '\f':
            o << "\\f";
            break;
        case '\n':
            o << "\\n";
            break;
        case '\r':
            o << "\\r";
            break;
        case '\t':
            o << "\\t";
            break;
        default:
            if ('\x00' <= *c && *c <= '\x1f')
            {
                o << "\\u"
                  << std::hex << std::setw(4) << std::setfill('0') << (int)*c;
            }
            else
            {
                o << *c;
            }
        }
    }
    return o.str();
}