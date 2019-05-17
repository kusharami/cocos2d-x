/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Erawppa
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CCNotificationCenter.h"
#include "cocoa/CCArray.h"
#include "script_support/CCScriptSupport.h"
#include <string>

using namespace std;

NS_CC_BEGIN

static CCNotificationCenter *s_sharedNotifCenter = NULL;

CCNotificationCenter::CCNotificationCenter()
{
    m_observers = CCArray::createWithCapacity(3);
    m_observers->retain();
}

CCNotificationCenter::~CCNotificationCenter()
{
    m_observers->release();
}

CCNotificationCenter *CCNotificationCenter::sharedNotificationCenter(void)
{
    if (!s_sharedNotifCenter)
    {
        s_sharedNotifCenter = new CCNotificationCenter;
    }
    return s_sharedNotifCenter;
}

void CCNotificationCenter::purgeNotificationCenter(void)
{
    CC_SAFE_RELEASE_NULL(s_sharedNotifCenter);
}

//
// internal functions
//
bool CCNotificationCenter::observerExisted(CCObject *target,const char *name)
{
    CCObject* obj = NULL;
    CCARRAY_FOREACH(m_observers, obj)
    {
        CCNotificationObserver* observer = (CCNotificationObserver*) obj;
        if (observer && observer->getTarget() == target && !strcmp(observer->getName(),name))
            return true;
    }
    return false;
}

//
// observer functions
//
void CCNotificationCenter::addObserver(CCObject *target, 
                                       SEL_CallFuncO selector,
                                       const char *name,
                                       CCObject *obj)
{
    if (this->observerExisted(target, name))
        return;
    
    CCNotificationObserver *observer = new CCNotificationObserver(target, selector, name, obj);
    if (!observer)
        return;
    
    observer->autorelease();
    m_observers->addObject(observer);
}

void CCNotificationCenter::removeObserver(CCObject *target,const char *name)
{
    CCObject* obj = NULL;
    CCARRAY_FOREACH(m_observers, obj)
    {
        CCNotificationObserver* observer = (CCNotificationObserver*) obj;
        if (!observer)
            continue;
        
        if (!strcmp(observer->getName(),name) && observer->getTarget() == target)
        {
            m_observers->removeObject(observer);
            return;
        }
    }
}

int CCNotificationCenter::removeAllObservers(CCObject *target)
{
    CCObject *obj = NULL;
    CCArray *toRemove = CCArray::create();

    CCARRAY_FOREACH(m_observers, obj)
    {
        CCNotificationObserver *observer = (CCNotificationObserver *)obj;
        if (!observer)
            continue;

        if (observer->getTarget() == target)
        {
            toRemove->addObject(observer);
        }
    }

    m_observers->removeObjectsInArray(toRemove);
    return toRemove->count();
}

void CCNotificationCenter::registerScriptObserver(CCObject *target, int64_t handler, const char* name)
{
    
    if (this->observerExisted(target, name))
        return;
    
    CCNotificationObserver *observer = new CCNotificationObserver(target, NULL, name, NULL);
    if (!observer)
        return;
    
    observer->registerScriptHandler(handler);
    observer->autorelease();
    m_observers->addObject(observer);
}

void CCNotificationCenter::unregisterScriptObserver(CCObject *target,const char* name)
{        
    CCObject* obj = NULL;
    CCARRAY_FOREACH(m_observers, obj)
    {
        CCNotificationObserver* observer = (CCNotificationObserver*) obj;
        if (!observer)
            continue;
            
        if ( !strcmp(observer->getName(),name) && observer->getTarget() == target)
        {
            m_observers->removeObject(observer);
        }
    }
}

void CCNotificationCenter::postNotification(const char *name, CCObject *object)
{
    CCArray* ObserversCopy = CCArray::createWithCapacity(m_observers->count());
    ObserversCopy->addObjectsFromArray(m_observers);
    CCObject* obj = NULL;
    CCARRAY_FOREACH(ObserversCopy, obj)
    {
        CCNotificationObserver* observer = (CCNotificationObserver*) obj;
        if (!observer)
            continue;
        
        if ((observer->getObject() == object || observer->getObject() == NULL || object == NULL) && !strcmp(name,observer->getName()))
        {
            if (0 != observer->getScriptHandler())
            {
                CCScriptEngineProtocol* engine = CCScriptEngineManager::sharedManager()->getScriptEngine();
                engine->executeNotificationEvent(observer, name);
            }
            else
            {
                observer->performSelector(object);
            }
        }
    }
}

void CCNotificationCenter::postNotification(const char *name)
{
    this->postNotification(name,NULL);
}

int64_t CCNotificationCenter::getObserverHandlerByName(const char* name)
{
    if (NULL == name || strlen(name) == 0)
    {
        return -1;
    }
    
    CCObject* obj = NULL;
    CCARRAY_FOREACH(m_observers, obj)
    {
        CCNotificationObserver* observer = (CCNotificationObserver*) obj;
        if (NULL == observer)
            continue;
        
        if ( 0 == strcmp(observer->getName(),name) )
        {
            return observer->getScriptHandler();
        }
    }
    
    return -1;
}

////////////////////////////////////////////////////////////////////////////////
///
/// CCNotificationObserver
///
////////////////////////////////////////////////////////////////////////////////
CCNotificationObserver::CCNotificationObserver(CCObject *target, 
                                               SEL_CallFuncO selector,
                                               const char *name,
                                               CCObject *obj)
{
    m_target = target;
    m_selector = selector;
    m_object = obj;
    
    m_name = new char[strlen(name)+1];
    memset(m_name,0,strlen(name)+1);
    
    string orig (name);
    orig.copy(m_name,strlen(name),0);
}

CCNotificationObserver::~CCNotificationObserver()
{
    CC_SAFE_DELETE_ARRAY(m_name);
}

void CCNotificationObserver::performSelector(CCObject *obj)
{
    if (m_target)
    {
		if (obj) {
			(m_target->*m_selector)(obj);
		} else {
			(m_target->*m_selector)(m_object);
		}
    }
}

CCObject *CCNotificationObserver::getTarget()
{
    return m_target;
}

SEL_CallFuncO CCNotificationObserver::getSelector()
{
    return m_selector;
}

char *CCNotificationObserver::getName()
{
    return m_name;
}

CCObject *CCNotificationObserver::getObject()
{
    return m_object;
}


NS_CC_END
