#ifndef _DNSCACHE_HPP
#define _DNSCACHE_HPP

#include <string>
#include <unordered_map>
#include <mutex>

struct DNSCacheNode 
{
	std::string name;
	std::string ip;
	DNSCacheNode* next;
	DNSCacheNode* prev;

	DNSCacheNode();
	DNSCacheNode(const std::string& _name, const std::string& _ip);
};

class DNSCache
{
private:
	size_t m_size;
	std::unordered_map<std::string, DNSCacheNode*> m_cache;
	DNSCacheNode* m_head;
	DNSCacheNode* m_tail;

	std::mutex m_mutex;

	void delete_node(DNSCacheNode* node);
	void move2head(DNSCacheNode* node);
public:
	explicit DNSCache(size_t max_size);
	void update(const std::string& name, const std::string& ip);
	std::string resolve(const std::string& name);
	~DNSCache();
};

#endif