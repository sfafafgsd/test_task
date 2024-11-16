#include "DNSCache.hpp"

DNSCacheNode::DNSCacheNode() : next(nullptr), prev(nullptr), ip(""), name("") {}
DNSCacheNode::DNSCacheNode(const std::string& _name, const std::string& _ip) : ip(_ip), name(_name), next(nullptr), prev(nullptr) {}


DNSCache::DNSCache(size_t max_size) : m_size(max_size)
{
	m_head = new DNSCacheNode;
	m_tail = new DNSCacheNode;

	m_head->next = m_tail;
	m_tail->prev = m_head;
}

void DNSCache::update(const std::string& name, const std::string& ip)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (m_cache.find(name) != m_cache.end()) {
		DNSCacheNode* node = m_cache[name];

		delete_node(node);

		DNSCacheNode* temp = m_head->next;
		move2head(node);
		node->ip = ip;
	}
	else 
	{
		DNSCacheNode* new_node = new DNSCacheNode(name, ip);
		if (m_cache.size() == m_size) {
			DNSCacheNode* tmp = m_tail->prev;
			m_cache.erase(tmp->name);
			delete_node(tmp);
			move2head(new_node);
			m_cache[name] = new_node;
			delete tmp;
		}
		else {
			move2head(new_node);
			m_cache[name] = new_node;
		}
	}
	
}

std::string DNSCache::resolve(const std::string& name)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (m_cache.find(name) != m_cache.end()) {
		DNSCacheNode* node = m_cache[name];
		delete_node(node);
		move2head(node);

		return node->ip;
	}
	return "";
}

void DNSCache::delete_node(DNSCacheNode* node)
{
	node->prev->next = node->next;
	node->next->prev = node->prev;
}

void DNSCache::move2head(DNSCacheNode* node)
{
	DNSCacheNode* temp = m_head->next;
	m_head->next = node;
	node->prev = m_head;

	node->next = temp;
	temp->prev = node;
}

DNSCache::~DNSCache()
{
	DNSCacheNode* tmp = m_head;
	DNSCacheNode* next;
	while (tmp) {
		next = tmp->next;
		delete tmp;
		tmp = next;
	}
}
