#ifndef _DNSCACHE_HPP
#define _DNSCACHE_HPP

#include <string>
#include <unordered_map>
#include <mutex>
#include <memory>

struct DNSCacheNode 
{
	std::string_view name = "";
	std::string_view ip = "";
	DNSCacheNode* next = nullptr;
	DNSCacheNode* prev = nullptr;

	DNSCacheNode();
	DNSCacheNode(const std::string_view& _name, const std::string_view& _ip);
};

class DNSCache
{
private:
	size_t m_size;
	std::unordered_map<std::string_view, DNSCacheNode*> m_cache;

	std::unique_ptr<DNSCacheNode[]> m_allocated_nodes;
	size_t m_node_counter = 0;

	DNSCacheNode* m_head = nullptr;
	DNSCacheNode* m_tail = nullptr;

	std::mutex m_mutex;

	void remove_node(DNSCacheNode* node);
	void move2head(DNSCacheNode* node);
public:
	explicit DNSCache(size_t max_size);
	void update(const std::string_view& name, const std::string_view& ip);
	std::string_view resolve(const std::string_view& name);
};

#endif
