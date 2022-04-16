#include "meta_struct.h"
#include <iostream>

int main()
{
    using spi_meta = meta_struct< //
        named_type<"rx", size_t>, //
        named_type<"tx", size_t> //
        >;

    using config = meta_struct< //
        named_type<"spi0", spi_meta>, //
        named_type<"spi1", spi_meta> //
        >;

    config meta {
        arg<"spi0">(spi_meta {
            arg<"rx">(10), //
            arg<"tx">(11) //
        }), //

        arg<"spi1">(spi_meta {
            arg<"rx">(13), //
            arg<"tx">(15) //
        }), //
    };

    for_each([&](auto c) { std::cout << c.name() << " : " << c.value() << "\n"; }, get<"spi0">(meta).value());
    for_each([&](auto c) { std::cout << c.name() << " : " << c.value() << "\n"; }, get<"spi1">(meta).value());
    return 0;
}