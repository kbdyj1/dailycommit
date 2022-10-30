#include <iostream>

namespace { //=================================================================

template <typename B, int D>
class Discriminator : public B {

};

template <typename Setter0, typename Setter1, typename Setter2, typename Setter3>
class PolicySelector : public Discriminator<Setter0, 0>
                     , public Discriminator<Setter1, 1>
                     , public Discriminator<Setter2, 2>
                     , public Discriminator<Setter3, 3>
{

};

} //namespace =================================================================

void test_ch_21_named_template_arg()
{

}
