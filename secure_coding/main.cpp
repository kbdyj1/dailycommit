extern "C" {

void test_gets();
void test_memstream();

}

void test_cin();
void test_string_out_of_range();

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    test_gets();
#if (0)
    test_memstream();
    test_cin();
    test_string_out_of_range();
#endif

    return 0;
}
