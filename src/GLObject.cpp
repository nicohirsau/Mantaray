#include "Mantaray/GLObjects/GLObject.h"
#include "Mantaray/GLObjects/GLObjectChain.h"

using namespace MR;

GLObject::GLObject() {
    link();
}

GLObject::~GLObject() {
    unlink();
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
