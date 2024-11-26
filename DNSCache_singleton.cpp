#include "DNSCache_singleton.hpp"

DNSCacheNode::DNSCacheNode() {}
DNSCacheNode::DNSCacheNode(const std::string_view& _name, const std::string_view& _ip) : ip(_ip), name(_name) {}

void DNSCache::set_max_size(size_t max_size)
{
	m_size = max_size;
}

DNSCache::DNSCache() : m_size(2 + 2)
{
	m_allocated_nodes = std::make_unique<DNSCacheNode[]>(m_size);

	m_head = &m_allocated_nodes[m_node_counter++];
	m_tail = &m_allocated_nodes[m_node_counter++];

	m_head->next = m_tail;
	m_tail->prev = m_head;
}

void DNSCache::update(const std::string_view& name, const std::string_view& ip)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (m_cache.find(name) != m_cache.end()) {
		DNSCacheNode* node = m_cache[name];
		remove_node(node);
		move2head(node);
		node->ip = ip;
	}
	else 
	{
		DNSCacheNode *new_node = nullptr;
		if (m_node_counter != m_size)
		{
			new_node = &m_allocated_nodes[m_node_counter++];
		}
		else
		{
			new_node = m_tail->prev;
			m_cache.erase(new_node->name);
			remove_node(new_node);
		}
		new_node->ip = ip;
		new_node->name = name;
		move2head(new_node);
		m_cache[name] = new_node;
	}
	
}

std::string_view DNSCache::resolve(const std::string_view& name)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (m_cache.find(name) != m_cache.end()) {
		DNSCacheNode* node = m_cache[name];
		remove_node(node);
		move2head(node);

		return node->ip;
	}
	return "";
}

void DNSCache::remove_node(DNSCacheNode* node)
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

DNSCache& DNSCache::get_instance()
{
	static DNSCache inst;
	return inst;
}

DNSCache::~DNSCache() {}
