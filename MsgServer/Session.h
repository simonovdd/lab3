#pragma once

struct Session
{
	int m_ID;
	string m_Name;
	time_t m_timer;

	queue<Message> m_Messages;
	vector<Message>	MessageList;
	CCriticalSection m_CS;

	Session(int ID, string Name)
		:m_ID(ID), m_Name(Name), m_timer(time(0))
	{
	}

	void Add(Message& m)
	{
		CSingleLock sl(&m_CS, TRUE);
		MessageList.push_back(m);
		m_Messages.push(m);
	}

	void Send(CSocket& s)
	{
		CSingleLock sl(&m_CS, TRUE);
		if (m_Messages.empty())
		{
			Message::Send(s, m_ID, M_BROKER, M_NODATA);
		}
		else
		{
			m_Messages.front().Send(s);
			m_Messages.pop();
		}
	}
	void SendList(CSocket& s)
	{
		CSingleLock sl(&m_CS, TRUE);
		if (MessageList.empty())
		{
			Message::Send(s, m_ID, M_BROKER, M_NODATA);
		}
		else
		{
			Message message = MessageList.at(0);
			message.m_Data = "";
			for (Message m : MessageList) {
				message.m_Data += m.m_Data + ";";		
			}
			message.m_Header.m_Size = message.m_Data.length();
			message.Send(s);
		}
	}

};