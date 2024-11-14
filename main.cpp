#include <iostream>
#include "DNSCache_singleton.hpp"

int main(){
    DNSCache::get_instance().update("s1", "b1");
    DNSCache::get_instance().update("s2", "b2");
    DNSCache::get_instance().update("s3", "b3");  
    
    std::cout << DNSCache::get_instance().resolve("s1") << std::endl;
    std::cout << DNSCache::get_instance().resolve("s2") << std::endl;
    std::cout << DNSCache::get_instance().resolve("s3") << std::endl;
    return 0;
}
