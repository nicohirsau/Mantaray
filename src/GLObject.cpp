#include "Mantaray/GLObjects/GLObject.h"
#include "Mantaray/GLObjects/GLObjectChain.h"
#include "Mantaray/Core/Logger.h"

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
