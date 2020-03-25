#include "Mantaray/GLObjects/GLObject.hpp"
#include "Mantaray/GLObjects/GLObjectChain.hpp"
#include "Mantaray/Core/Logger.hpp"

using namespace MR;

GLObject::GLObject() {

}

GLObject::~GLObject() {

}

void GLObject::allocate() {
    Logger::Log("GLObject", "allocate not implemented!", Logger::LOG_WARNING);
}

void GLObject::release() {
    Logger::Log("GLObject", "release not implemented!", Logger::LOG_WARNING);
}

void GLObject::bind() {
    Logger::Log("GLObject", "bind not implemented!", Logger::LOG_WARNING);
}

void GLObject::unbind() {
    Logger::Log("GLObject", "unbind not implemented!", Logger::LOG_WARNING);
}

void GLObject::link() {
    if (!m_HasAllocatedData) {
        allocate();
        m_HasAllocatedData = true;
        GLObjectChain::Link(this);
    }
}


void GLObject::unlink() {
    if (m_HasAllocatedData) {
        release();
        m_HasAllocatedData = false;
        GLObjectChain::UnLink(this);
    }
}

void GLObject::destroy() {
    unlink();
    if (m_next != nullptr) {
        m_next->destroy();
    }
}
