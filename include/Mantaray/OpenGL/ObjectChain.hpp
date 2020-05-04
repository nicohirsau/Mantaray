#pragma once

namespace MR {
class ObjectChain {
    public:
        static void Initialize();
        static void TearDown();
        static void Link(class Object* link);
        static void UnLink(class Object* link);

    private:
        static bool Initialized;
        static class Object* ChainHead;
        static class Object* ChainTail;
        static class Logger Logger;
};
}
