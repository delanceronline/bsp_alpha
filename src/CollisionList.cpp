// CollisionList.cpp: implementation of the CollisionList class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "DXTemplate.h"
#include "CollisionList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CollisionList::CollisionList()
{
	head = NULL;
}

CollisionList::~CollisionList()
{
	//Clean collision pairs.
   	CleanList();
}

bool CollisionList::Add(CollisionPair *cp)
{
	if(head == NULL)
		head = cp;
	else
	{
		CollisionPair *cptemp, *cptemp_prev = NULL;
		cptemp = head;
		
		while(cptemp->m_next != NULL)
		{
			cptemp_prev = cptemp;
			cptemp = cptemp->m_next;
		}

		cptemp->m_previous = cptemp_prev;
		cptemp->m_next = cp;
	}

	return true;
}

void CollisionList::CleanList()
{
	//Clean collision pairs.
	if(head != NULL)
	{
		CollisionPair *temp, *swap;
		temp = head;
		while(temp != NULL)
		{
			swap = temp;
			temp = temp->m_next;

			delete swap;
			//swap = NULL;
		}
		head = NULL;
	}
}
