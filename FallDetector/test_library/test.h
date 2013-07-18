#ifdef TEST_EXPORTS
#define TEST_API __declspec(dllexport)
#else
#define TEST_API __declspec(dllimport)
#endif

namespace TestNamespace
{
    class Test
    {
    public:
        static TEST_API double Hey();
    };
}