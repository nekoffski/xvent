#pragma once

#include <string>

#include "EventProvider.h"

namespace xvent {

class EventListener {
public:
    explicit EventListener(const std::string& ident)
        : m_ident(ident) {
    }

    std::string getIdent() const {
        return m_ident;
    }

    virtual void handleEvents(const EventProvider&) = 0;

private:
    std::string m_ident;
};
}
