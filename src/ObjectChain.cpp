#include <sstream> //for std::stringstream 
#include <string>  //for std::string

#include "Mantaray/Core/Logger.hpp"
#include "Mantaray/OpenGL/ObjectChain.hpp"
#include "Mantaray/OpenGL/Object.hpp"
#include "Mantaray/OpenGL/ObjectLibrary.hpp"

using namespace MR;

inline std::string GetPointerString(Object* pointer) {
    const void * address = static_cast<const void*>(pointer);
    std::stringstream ss;
    ss << address;  
    return ss.str(); 
}

bool ObjectChain::Initialized = false;
Object* ObjectChain::ChainTail = nullptr;
Object* ObjectChain::ChainHead = nullptr;
Logger ObjectChain::Logger("ObjectChain");

void ObjectChain::Initialize() {
    if (ObjectChain::Initialized) {
        ObjectChain::Logger.Log("Already Initialized!", Logger::LOG_WARNING);
        return;
    }
    ObjectChain::ChainHead = new Object();
    ObjectChain::ChainTail = ObjectChain::ChainHead;
    ObjectChain::Logger.Log("Head is: " + GetPointerString(ObjectChain::ChainHead), Logger::LOG_DEBUG);
    ObjectChain::Initialized = true;
    ObjectLibrary::InitializeDefaultEntries();
}

void ObjectChain::TearDown() {
    if (!ObjectChain::Initialized) {
        ObjectChain::Logger.Log("Chain is not Initialized! (TearDown)", Logger::LOG_WARNING);
        return;
    }
    ObjectChain::ChainHead->destroy();
    delete ObjectChain::ChainHead;
    ObjectChain::ChainHead = nullptr;
    ObjectChain::Initialized = false;
}

void ObjectChain::Link(Object* link) {
    if (!ObjectChain::Initialized) {
        ObjectChain::Logger.Log("Chain is not Initialized! (Link)", Logger::LOG_WARNING);
        return;
    }
    ObjectChain::Logger.Log("Linking in: " + GetPointerString(link), Logger::LOG_DEBUG);
    link->m_Previous = ObjectChain::ChainTail;
    ObjectChain::ChainTail->m_Next = link;
    ObjectChain::ChainTail = link;
}

void ObjectChain::UnLink(Object* link) {
    if (!ObjectChain::Initialized) {
        ObjectChain::Logger.Log("Chain is not Initialized! (UnLink)", Logger::LOG_WARNING);
        return;
    }
    ObjectChain::Logger.Log("Unlinking: " + GetPointerString(link), Logger::LOG_DEBUG);
    if (link->m_Previous != nullptr) {
        link->m_Previous->m_Next = link->m_Next;
    }
    if (link->m_Next != nullptr) {
        link->m_Next->m_Previous = link->m_Previous;
    }
}
