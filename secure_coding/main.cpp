extern "C" {

void test_gets();
void test_memstream();
void test_fotify_source();

}

void test_cin();
void test_string_out_of_range();
void test_strcpy();

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

#if (0)
    test_gets();
    test_memstream();
    test_cin();
    test_string_out_of_range();
    test_strcpy();
#endif

    test_fotify_source();

    return 0;
}
