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

bool GLObjectChain::Initialized = false;
GLObject* GLObjectChain::ChainTail = nullptr;
GLObject* GLObjectChain::ChainHead = nullptr;
Logger GLObjectChain::Logger("GLObjectChain");

void GLObjectChain::Initialize() {
    if (GLObjectChain::Initialized) {
        GLObjectChain::Logger.Log("Already Initialized!", Logger::LOG_WARNING);
        return;
    }
    GLObjectChain::ChainHead = new GLObject();
    GLObjectChain::ChainTail = GLObjectChain::ChainHead;
    GLObjectChain::Logger.Log("Head is: " + GetPointerString(GLObjectChain::ChainHead), Logger::LOG_DEBUG);
    GLObjectChain::Initialized = true;
}

void GLObjectChain::TearDown() {
    if (!GLObjectChain::Initialized) {
        GLObjectChain::Logger.Log("Chain is not Initialized! (TearDown)", Logger::LOG_WARNING);
        return;
    }
    GLObjectChain::ChainHead->destroy();
    delete GLObjectChain::ChainHead;
    GLObjectChain::ChainHead = nullptr;
    GLObjectChain::Initialized = false;
}

void GLObjectChain::Link(GLObject* link) {
    if (!GLObjectChain::Initialized) {
        GLObjectChain::Logger.Log("Chain is not Initialized! (Link)", Logger::LOG_WARNING);
        return;
    }
    GLObjectChain::Logger.Log("Linking in: " + GetPointerString(link), Logger::LOG_DEBUG);
    link->m_Previous = GLObjectChain::ChainTail;
    GLObjectChain::ChainTail->m_Next = link;
    GLObjectChain::ChainTail = link;
}

void GLObjectChain::UnLink(GLObject* link) {
    if (!GLObjectChain::Initialized) {
        GLObjectChain::Logger.Log("Chain is not Initialized! (UnLink)", Logger::LOG_WARNING);
        return;
    }
    GLObjectChain::Logger.Log("Unlinking: " + GetPointerString(link), Logger::LOG_DEBUG);
    if (link->m_Previous != nullptr) {
        link->m_Previous->m_Next = link->m_Next;
    }
    if (link->m_Next != nullptr) {
        link->m_Next->m_Previous = link->m_Previous;
    }
}
