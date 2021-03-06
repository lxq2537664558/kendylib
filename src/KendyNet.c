#include "KendyNet.h"
#include "epoll.h"
#include "Engine.h"
#include "Socket.h"
#include "link_list.h"
#include "HandleMgr.h"
#include "SocketWrapper.h"
#include <assert.h>

int32_t InitNetSystem()
{
	return InitHandleMgr();
}

int32_t EngineRun(HANDLE engine,int32_t timeout)
{
	engine_t e = GetEngineByHandle(engine);
	if(!e)
		return -1;
	return e->Loop(e,timeout);	
}

HANDLE CreateEngine()
{
	HANDLE engine = NewEngine();
	if(engine >= 0)
	{
		engine_t e = GetEngineByHandle(engine);
		if(0 != e->Init(e))
		{
			CloseEngine(engine);
			engine = -1;
		}
		else
		{
			memset(e->events,0,sizeof(e->events));
			LINK_LIST_CLEAR(e->actived);
		}
	}
	return engine;
}

void CloseEngine(HANDLE handle)
{
	ReleaseEngine(handle);
}

int32_t Bind2Engine(HANDLE e,HANDLE s,OnRead _OnRead,OnWrite _OnWrite)
{
	engine_t engine = GetEngineByHandle(e);
	socket_t sock   = GetSocketByHandle(s);
	if(!engine || ! sock)
		return -1;
	sock->OnRead = _OnRead;
	sock->OnWrite = _OnWrite;
	if(engine->Register(engine,sock) == 0)
	{
		sock->engine = engine;
		return 0;
	}
	return -1;
}

int32_t WSASend(HANDLE sock,st_io *io,int32_t now,uint32_t *err_code)
{
	assert(io);
	socket_t s = GetSocketByHandle(sock);
	if(!s)
	{
		*err_code = 0;
		return -1;
	}
	if(!now)
	{
		LINK_LIST_PUSH_BACK(s->pending_send,io);
		if(s->engine && s->writeable && !s->isactived)
		{
			s->isactived = 1;
			LINK_LIST_PUSH_BACK(s->engine->actived,s);
		}
		*err_code = EAGAIN;
		return -1;
	}
	else
	{
		int32_t bytes_transfer = 0;
		return raw_send(s,io,&bytes_transfer,err_code);
	}
}

int32_t WSARecv(HANDLE sock,st_io *io,int32_t now,uint32_t *err_code)
{
	assert(io);
	socket_t s = GetSocketByHandle(sock);
	if(!s)
	{
		*err_code = 0;
		return -1;
	}
	if(!now)
	{
		LINK_LIST_PUSH_BACK(s->pending_recv,io);
		if(s->engine && s->readable && !s->isactived)
		{
			s->isactived = 1;
			LINK_LIST_PUSH_BACK(s->engine->actived,s);	
		}
		*err_code = EAGAIN;
		return -1;
	}
	else
	{
		int bytes_transfer = 0;
		return raw_recv(s,io,&bytes_transfer,err_code);
	}
}
