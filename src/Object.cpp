#include "Mantaray/OpenGL/Object.hpp"
#include "Mantaray/OpenGL/ObjectChain.hpp"
#include "Mantaray/Core/Logger.hpp"

using namespace MR;

Object::Object() {

}

Object::~Object() {

}

void Object::allocate() {
    Logger::Log("Object", "allocate not implemented!", Logger::LOG_WARNING);
}

void Object::release() {
    Logger::Log("Object", "release not implemented!", Logger::LOG_WARNING);
}

void Object::bind() {
    Logger::Log("Object", "bind not implemented!", Logger::LOG_WARNING);
}

void Object::unbind() {
    Logger::Log("Object", "unbind not implemented!", Logger::LOG_WARNING);
}

void Object::link() {
    if (!m_HasAllocatedData) {
        allocate();
        m_HasAllocatedData = true;
        ObjectChain::Link(this);
    }
}


void Object::unlink() {
    if (m_HasAllocatedData) {
        release();
        m_HasAllocatedData = false;
        ObjectChain::UnLink(this);
    }
}

void Object::destroy() {
    unlink();
    if (m_Next != nullptr) {
        m_Next->destroy();
    }
}
