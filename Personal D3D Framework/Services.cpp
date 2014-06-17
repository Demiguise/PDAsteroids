#include "Services.h"

Services* Services::m_pInstance = 0;

Services* Services::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new Services();
	}
	return m_pInstance;
}

Services::Services()
{
}


Services::~Services()
{
}
