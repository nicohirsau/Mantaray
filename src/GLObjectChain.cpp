#include "Mantaray/GLObjects/GLObjectChain.h"
#include "Mantaray/GLObjects/GLObject.h"

using namespace MR;

GLObject* GLObjectChain::m_chainTail = nullptr;
GLObject* GLObjectChain::m_chainHead = nullptr;

void GLObjectChain::Initialize() {
    GLObjectChain::m_chainHead = new GLObject();
    GLObjectChain::m_chainTail = GLObjectChain::m_chainHead;
}

void GLObjectChain::TearDown() {
    GLObjectChain::m_chainHead->destroy();
    delete GLObjectChain::m_chainHead;
    GLObjectChain::m_chainHead = nullptr;
}

void GLObjectChain::Link(GLObject* link) {
    link->m_previous = GLObjectChain::m_chainTail;
    GLObjectChain::m_chainTail->m_next = link;
    GLObjectChain::m_chainTail = link;
}

void GLObjectChain::UnLink(GLObject* link) {
    if (link->m_previous != nullptr) {
        link->m_previous->m_next = link->m_next;
    }
    if (link->m_next != nullptr) {
        link->m_next->m_previous = link->m_previous;
    }
}
