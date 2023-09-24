void test_gets();
void test_memstream();
void test_cin();
void test_string_out_of_range();

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

#if (0)
    test_gets();
    test_memstream();
    test_cin();
#endif

    test_string_out_of_range();

    return 0;
}
