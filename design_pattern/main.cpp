namespace { //=================================================================

} // namespace ================================================================

void test_composit();
void test_decorator();
void test_flyweight();
void test_proxy();
void test_chain_of_responsibility();

int main()
{
#if (0) //done
    test_composit();
    test_decorator();
    test_flyweight();
    test_proxy();
#endif

    test_chain_of_responsibility();

    return 0;
}
