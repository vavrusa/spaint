#include "networkservice.h"

NetworkService::NetworkService()
{
}

bool NetworkService::observe(CanvasMgr* cvmgr)
{
   return true;
}

bool NetworkService::start()
{
   return true;
}

bool NetworkService::stop()
{
   return true;
}

#include "networkservice.moc"
