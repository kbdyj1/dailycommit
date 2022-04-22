#include <iostream>
#include <vector>
#include <iterator>
#include "gmputil.h"

namespace { //=================================================================

int powm(int base, int exp, int mod)
{
    auto ret = base;
    for (int i=1; i<exp; i++) {
        ret *= base;
        ret %= mod;
    }
    return ret;
}

bool isPrime(int base, int mod)
{
    int exp = 2;
    int r = base * base;
    for ( ; r != base; exp++) {
        r *= base;
        r %= mod;
    }
    return exp == mod;
}

std::vector<int> primitiveRoot(int mod)
{
    auto v = std::vector<int>{};
    for (int i=2; i<mod; i++) {
        if (isPrime(i, mod))
            v.push_back(i);
    }
    return v;
}

void printVector(const std::vector<int>& v)
{
    std::copy(std::begin(v), std::end(v), std::ostream_iterator<int>(std::cout, " "));
}

struct DiffienHellman
{
    mpz_class setPeerPublicKey(mpz_class pubKey) {
        K = gmputil::powm(pubKey, x, p);
        return K;
    }
    mpz_class p { "0x"
                  "AAAAB3FFAC1FC2EAAAABFFAAAFEAFFFFFFDFFFFF17FBFFFFFFFFFF9FFFFBFDFF"
                  "FFFFAFFFFDFFF7FDF4FF5EF18FF4FF9FBFFFFFFFFF7FFEFEFF4F9FFF5FFFBFFF"
                  "FBF7FFDFFFFBFFFFCFFAF3FFFFFBFFFF1FFFFFFAFFFCFFF8F6FFFFAFFDFDFFFF"
                  "F3FAFFFAFFCF1F9F53F0FF68EFFFB1FFFFFFFFFFFFF88FFFFDFFFFBF6FFFB0FF" };
    mpz_class K;
    mpz_class g = 2;
    mpz_class x = gmputil::randomPrime(255);
    mpz_class y = gmputil::powm(g, x, p);
};

} // namespace ================================================================

void test_powm()
{
    for (int i=2; i<16; i++) {
        std::cout << "powm(3, " << i << " , 19) : " << powm(2, i, 19) << std::endl;
    }
}

void test_primitiveRoot()
{
    auto v = primitiveRoot(17);
    printVector(v);
}

void test_gmputil()
{
    uint8_t array[8];
    mpz_class a{"0x1234567890abcdef"};
    gmputil::mpzToBigEndian(a, array, array+8);
    auto b = gmputil::bigEndianToMpz(array, array+8);

    std::cout << "a == b : " << (a == b) << std::endl;

    DiffienHellman alice;
    DiffienHellman bob;

    alice.setPeerPublicKey(bob.y);
    bob.setPeerPublicKey(alice.y);

    std::cout << "alice.K == bob.K : " << (alice.K == bob.K) << std::endl;
}
