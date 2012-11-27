#ifndef QUEUE_LIST_H
#define QUEUE_LIST_H

template<class T>
class QueueList
{
private:
	class Node
	{
	public:
		Node* next;
		T data;
	public:
		Node()
		{
			next = NULL;
		}
		Node(T p_data)
		{
			data = p_data;
			next = NULL;
		}
	};

	Node* m_frontNode;
	Node* m_backNode;
	unsigned int m_length;

public:
	QueueList<T>()
	{
		m_frontNode = NULL;
		m_backNode = NULL;
		m_length = 0;
	}

	~QueueList<T>()
	{
		while(m_frontNode != NULL)
		{
			popFront();
		}
	}

	void pushBack(T p_data)
	{
		if(m_frontNode == NULL)
		{
			m_frontNode = new Node(p_data);
			m_backNode = m_frontNode;
		}
		else
		{
			Node* newNode = new Node(p_data);
			m_backNode->next = newNode;
			m_backNode = newNode;
		}

		m_length ++;
	}

	T popFront()
	{
		Node* tempNode = m_frontNode;
		m_frontNode = m_frontNode->next;
		T data = tempNode->data;
		delete tempNode;
		m_length --;

		return data;
	}

	const unsigned int& length()
	{
		return m_length;
	}

	void buildArray(T* p_buffer)
	{
		Node* nodeTraveller = m_frontNode;

		for(unsigned int i = 0; i < m_length; i++)
		{
			p_buffer[i] = nodeTraveller->data;
			nodeTraveller = nodeTraveller->next;
		}
	}

	const T& front()
	{
		return m_frontNode->data;
	}
};

#endif