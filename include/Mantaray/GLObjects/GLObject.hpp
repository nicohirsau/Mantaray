#pragma once

namespace MR {
class GLObject {
    friend class GLObjectChain;

    public:
        GLObject();
        virtual ~GLObject();

        virtual void bind();
        virtual void unbind();
    
    protected:
        virtual void allocate();
        virtual void release();
        void link();
        void unlink();

    private:
        void destroy();

    private: 
        bool m_HasAllocatedData = false;
        GLObject* m_Previous = nullptr;
        GLObject* m_Next = nullptr;
};
}
