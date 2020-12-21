#pragma once

#include <exception>
namespace xvent {

struct XventError : public std::exception {};

struct ListenerAlreadyRegistered : XventError {};
}
