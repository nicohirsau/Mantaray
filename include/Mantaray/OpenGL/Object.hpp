#pragma once

namespace MR {
class Object {
    friend class ObjectChain;

    public:
        Object();
        virtual ~Object();

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
        Object* m_Previous = nullptr;
        Object* m_Next = nullptr;
};
}
