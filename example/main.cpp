#include <boost/version.hpp>
#include <cstdio>
int main() {
    printf("Boost version: %d.%d.%d\n",
            BOOST_VERSION / 100000,
           (BOOST_VERSION / 100) % 1000,
           BOOST_VERSION % 100);
    return 0;
}
