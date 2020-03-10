namespace MR {
class GLObject {
    friend class GLObjectChain;

    public:
    	GLObject();
        ~GLObject();
    
    protected:
        virtual void allocate();
        virtual void release();

    private:
    	void link();
    	void unlink();
        void destroy();

    private: 
        bool m_HasAllocatedData = false;
        GLObject* m_previous = nullptr;
        GLObject* m_next = nullptr;
};
}