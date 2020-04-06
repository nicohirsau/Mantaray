#pragma once

namespace MR {
class GLObjectChain {
    public:
        static void Initialize();
        static void TearDown();
        static void Link(class GLObject* link);
        static void UnLink(class GLObject* link);

    private:
        static bool Initialized;
        static class GLObject* ChainHead;
        static class GLObject* ChainTail;
        static class Logger Logger;
};
}
