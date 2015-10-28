#ifndef HASHTABLE_INCLUDED
#define HASHTABLE_INCLUDED

//Hash table implemented with a dynamic array of pointers to Nodes (buckets)
//each bucket points to a circular doubly linked list with a dummy node
//Hash table class also includes a circular doubly linked list of NodePtrs to record order of insertion
template <typename KeyType,	typename ValueType>
class HashTable
{
public:
	HashTable(unsigned int numBuckets,	unsigned int capacity);
	~HashTable();
	bool isFull() const;
	bool set(const KeyType&	key, const ValueType& value, bool permanent = false);
	bool get(const KeyType& key, ValueType& value) const;
	bool touch(const KeyType& key);
	bool discard(KeyType& key,	ValueType& value);

private:
	//	We	prevent a	HashTable from	being	copied	or	assigned	by	declaring	the
	//	copy	constructor	and	assignment	operator	private	and	not	implementing	them.
	HashTable(const HashTable&);
	HashTable& operator=(const HashTable&);

	struct Node;

	//holds pointers to recently added nodes
	struct NodePtr
	{
		Node* m_ptr; //pointer to node that was added to hash table
		NodePtr* m_prev;
		NodePtr* m_next;
	};

	//pointer to doubly linked list of NodePtrs
	//used to keep track of order Nodes were added to hash table
	NodePtr* m_leastRecent;

	//node containing key, value, and pointer to it's place in the list of recently added items
	struct Node
	{
		//default construct permanent node (m_order = nullptr, no pointer to list of recent items)
		Node():m_order(nullptr){}
		KeyType m_key;
		ValueType m_value;
		Node* m_prev;
		Node* m_next;
		NodePtr* m_order; //pointer to place in recent list
	};

	bool findKey(const KeyType& key, Node*& p) const;
	void setRecent(NodePtr* p);

	Node** m_buckets;
	unsigned int m_nbuckets;
	unsigned int m_capacity;
	unsigned int m_count;
};

template <typename KeyType,	typename ValueType>
HashTable<KeyType, ValueType>::HashTable(unsigned int numBuckets, unsigned int capacity)
	:m_nbuckets(numBuckets), m_capacity(capacity), m_count(0), m_leastRecent(nullptr)
{
	m_buckets = new Node* [m_nbuckets];
	//m_buckets is an array of headpointers to dummy nodes for each linked list

	//initialize dummy nodes at head of each linked list in m_buckets array
	for (unsigned int k = 0; k < m_nbuckets; k++)
	{
		m_buckets[k] = new Node;
		Node* head = m_buckets[k];
		head->m_next = head;
		head->m_prev = head;
	}

	//initialize dummy node at head of recent items list
	m_leastRecent = new NodePtr;
	m_leastRecent->m_next = m_leastRecent;
	m_leastRecent->m_prev = m_leastRecent;
}

template <typename KeyType,	typename ValueType>
HashTable<KeyType, ValueType>::~HashTable()
{
	for (unsigned int k = 0; k < m_nbuckets; k++)
	{
		Node* p = m_buckets[k]->m_next;
		while (p!= m_buckets[k])
		{
			Node* tmp = p;
			p = p->m_next;
			delete tmp;
		}
	}
	delete [] m_buckets;

	NodePtr* p = m_leastRecent->m_next;
	while(p != m_leastRecent)
	{
		NodePtr* tmp = p;
		p = p->m_next;
		delete tmp;
	}
	delete m_leastRecent;
}

template <typename KeyType,	typename ValueType>
bool HashTable<KeyType, ValueType>::isFull() const
{
	return m_count == m_capacity;
}

//function to find a key in hash table
template <typename KeyType,	typename ValueType>
bool HashTable<KeyType, ValueType>::findKey(const KeyType& key, Node*& p) const
{
	//get bucket number
	unsigned int computeHash(KeyType key);
	unsigned int result = computeHash(key);
	unsigned int bucket = result % m_nbuckets;

	//search for key in linked list
	Node* head = m_buckets[bucket];
	for (p = head->m_next; p != head && p->m_key != key; p = p->m_next)
		;
	return (p != head);
}

//set an item as most recent in list of added items
template <typename KeyType,	typename ValueType>
void HashTable<KeyType, ValueType>::setRecent(NodePtr* p)
{
	//set previous and next NodePtrs to skip over current NodePtr object
	p->m_prev->m_next = p->m_next;
	p->m_next->m_prev = p->m_prev;

	//set current NodePtr's prev and next ptrs
	p->m_prev = m_leastRecent->m_prev;
	p->m_prev->m_next = p;
	//connect to dummy node
	p->m_next = m_leastRecent;
	m_leastRecent->m_prev = p;
}

template <typename KeyType,	typename ValueType>
bool HashTable<KeyType, ValueType>::set(const KeyType&	key, const ValueType& value, bool permanent)
{
	Node* position;

	//if key not already in table
	if ( !findKey(key, position) )
	{
		if (isFull())
			return false;
		//add new association key/value
		Node* p = new Node;
		p->m_key = key;
		p->m_value = value;
		//insert at end of list
		p->m_prev = position->m_prev;
		p->m_prev->m_next = p;
		//connect to dummy node
		p->m_next = position;
		position->m_prev = p;
		m_count++;

		//non-permanent items get recorded in list of recently added items
		if (!permanent)
			{
				NodePtr* tmp = new NodePtr;
				tmp->m_ptr = p;
				//insert at end of least recent list
				tmp->m_prev = m_leastRecent->m_prev;
				tmp->m_prev->m_next = tmp;
				//connect to dummy node
				tmp->m_next = m_leastRecent;
				m_leastRecent->m_prev = tmp;

				p->m_order = tmp; //Node itself should have pointer to its place in the recent items list
			}
	}
	
	else //key found in table: update value, don't change perm status
	{
		position->m_value = value;

		//make this node the most recent in list (if it is not a permament node)
		if (position->m_order != nullptr)
			setRecent(position->m_order);
	}
	return true;
}

template <typename KeyType,	typename ValueType>
bool HashTable<KeyType, ValueType>::get(const KeyType& key, ValueType& value) const
{
	Node* position;

	//key not found
	if ( !findKey(key, position) )
		return false;

	//key found
	value = position->m_value;
	return true;
}

template <typename KeyType,	typename ValueType>
bool HashTable<KeyType, ValueType>::touch(const KeyType& key)
{
	Node* position;

	//if key found && association is non-permanent
	if ( findKey(key, position) && position->m_order != nullptr)
	{
		setRecent(position->m_order);
		return true;
	}
	return false;
}

template <typename KeyType,	typename ValueType>
bool HashTable<KeyType, ValueType>::discard(KeyType& key, ValueType& value)
{
	//if empty list
	if (m_leastRecent->m_next == m_leastRecent)
		return false;

	Node* leastRecent = m_leastRecent->m_next->m_ptr;
	key = leastRecent->m_key;
	value = leastRecent->m_value;

	//delete Node
	leastRecent->m_prev->m_next = leastRecent->m_next;
	leastRecent->m_next->m_prev = leastRecent->m_prev;
	delete leastRecent;
	m_count--;

	//update recent items list
	NodePtr* tmp = m_leastRecent->m_next;
	m_leastRecent->m_next = tmp->m_next;
	tmp->m_next->m_prev = m_leastRecent;
	delete tmp;
	return true;
}

#endif // HASHTABLE_INCLUDED