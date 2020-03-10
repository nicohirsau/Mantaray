namespace MR {
class GLObject;

class GLObjectChain {
    public:
        static void Initialize();
        static void TearDown();
        static void Link(GLObject* link);
        static void UnLink(GLObject* link);

    private:
        static GLObject* m_chainHead;
        static GLObject* m_chainTail;
};
}