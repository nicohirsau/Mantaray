#include <sstream> //for std::stringstream 
#include <string>  //for std::string

#include "Mantaray/Core/Logger.hpp"
#include "Mantaray/GLObjects/GLObjectChain.hpp"
#include "Mantaray/GLObjects/GLObject.hpp"

using namespace MR;

inline std::string GetPointerString(GLObject* pointer) {
    const void * address = static_cast<const void*>(pointer);
    std::stringstream ss;
    ss << address;  
    return ss.str(); 
}

bool GLObjectChain::m_initialized = false;
GLObject* GLObjectChain::m_chainTail = nullptr;
GLObject* GLObjectChain::m_chainHead = nullptr;
Logger GLObjectChain::m_logger("GLObjectChain");

void GLObjectChain::Initialize() {
    if (GLObjectChain::m_initialized) {
        GLObjectChain::m_logger.Log("Already Initialized!", Logger::LOG_WARNING);
        return;
    }
    GLObjectChain::m_chainHead = new GLObject();
    GLObjectChain::m_chainTail = GLObjectChain::m_chainHead;
    GLObjectChain::m_logger.Log("Head is: " + GetPointerString(GLObjectChain::m_chainHead), Logger::LOG_DEBUG);
    GLObjectChain::m_initialized = true;
}

void GLObjectChain::TearDown() {
    if (!GLObjectChain::m_initialized) {
        GLObjectChain::m_logger.Log("Chain is not Initialized! (TearDown)", Logger::LOG_WARNING);
        return;
    }
    GLObjectChain::m_chainHead->destroy();
    delete GLObjectChain::m_chainHead;
    GLObjectChain::m_chainHead = nullptr;
    GLObjectChain::m_initialized = false;
}

void GLObjectChain::Link(GLObject* link) {
    if (!GLObjectChain::m_initialized) {
        GLObjectChain::m_logger.Log("Chain is not Initialized! (Link)", Logger::LOG_WARNING);
        return;
    }
    GLObjectChain::m_logger.Log("Linking in: " + GetPointerString(link), Logger::LOG_DEBUG);
    link->m_previous = GLObjectChain::m_chainTail;
    GLObjectChain::m_chainTail->m_next = link;
    GLObjectChain::m_chainTail = link;
}

void GLObjectChain::UnLink(GLObject* link) {
    if (!GLObjectChain::m_initialized) {
        GLObjectChain::m_logger.Log("Chain is not Initialized! (UnLink)", Logger::LOG_WARNING);
        return;
    }
    GLObjectChain::m_logger.Log("Unlinking: " + GetPointerString(link), Logger::LOG_DEBUG);
    if (link->m_previous != nullptr) {
        link->m_previous->m_next = link->m_next;
    }
    if (link->m_next != nullptr) {
        link->m_next->m_previous = link->m_previous;
    }
}
