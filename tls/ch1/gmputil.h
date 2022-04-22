#ifndef GMPUTIL_H
#define GMPUTIL_H

#include <gmpxx.h>
#include <assert.h>
#include <sstream>
#include <iomanip>
#include <random>

namespace gmputil //===========================================================
{

mpz_class nextPrime(mpz_class n)
{
    mpz_class r;

    mpz_nextprime(r.get_mpz_t(), n.get_mpz_t());

    return r;
}

mpz_class powm(mpz_class base, mpz_class exp, mpz_class mod)
{
    mpz_class r;

    assert(mod);
    mpz_powm(r.get_mpz_t(), base.get_mpz_t(), exp.get_mpz_t(), mod.get_mpz_t());

    return r;
}

template <typename Iter>
void mpzToBigEndian(mpz_class n, Iter begin, Iter end)
{
    for (Iter iter=end; iter!=begin; n /= 0x100) {
        *--iter = mpz_class{n % 0x100}.get_ui();
    }
}

template <typename Iter>
mpz_class bigEndianToMpz(Iter begin, Iter end)
{
    std::stringstream ss;
    ss << "0x";

    for (Iter iter=begin; iter!=end; iter++) {
        ss << std::hex << std::setfill('0') << std::setw(2) << +*iter;
    }
    return mpz_class{ss.str()};
}

mpz_class randomPrime(unsigned byte)
{
    unsigned char array[byte];
    std::uniform_int_distribution<> dist(0, 0xff);
    std::random_device rd;
    for (int i=0; i<byte; i++) {
        array[i] = dist(rd);
    }

    auto z = nextPrime(bigEndianToMpz(array, array+byte));

    for (int i=0; i<byte; i++) {
        array[i] = 0xff;
    }

    if (z > bigEndianToMpz(array, array+byte))
        return randomPrime(byte);
    else
        return z;
}

template <typename Container>
std::string toHex(std::string message, const Container& c)
{
    std::stringstream ss;
    ss << message << " : 0x";
    for (unsigned char a : c) {
        ss << std::hex << std::setw(2) << std::setfill('0') << +a;
    }
    return ss.str();
}

} // gmputil ==================================================================

#endif // GMPUTIL_H
