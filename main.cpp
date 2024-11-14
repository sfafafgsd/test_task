#include <iostream>
#include "DNSCache.hpp"

int main(){
    DNSCache cache(2);
    
    cache.update("s1", "b1");
    cache.update("s2", "b2");
    cache.update("s3", "b3"); // removes s1

    std::cout << cache.resolve("s1") << std::endl;
    std::cout << cache.resolve("s2") << std::endl;
    std::cout << cache.resolve("s3") << std::endl;
    

    return 0;
}
