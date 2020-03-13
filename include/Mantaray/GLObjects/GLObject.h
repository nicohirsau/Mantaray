#pragma once

namespace MR {
class GLObject {
    friend class GLObjectChain;

    public:
    	GLObject();
        virtual ~GLObject();
    
    protected:
        virtual void allocate();
        virtual void release();
    	void link();
    	void unlink();

    private:
        void destroy();

    private: 
        bool m_HasAllocatedData = false;
        GLObject* m_previous = nullptr;
        GLObject* m_next = nullptr;
};
}