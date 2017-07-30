#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef struct tagOBJECT OBJECT;
typedef struct tagOBSERVER OBSERVER;

typedef struct tagOBSERVER
{
    int id;
    char name[32];

    void (*update)(OBSERVER *this, OBJECT *pObject);
} OBSERVER;

#define MAX_BIND_NUMBER 100
typedef struct tagOBJECT
{
    OBSERVER *pObserverList[MAX_BIND_NUMBER];
    int num;

    int data;

    void (*notify)(OBJECT *this);
    int (*addObserver)(struct tagOBJECT *this, OBSERVER *observer);
    void (*delObserver)(struct tagOBJECT *this, OBSERVER *observer);
} OBJECT;

// object
int objectAddObserver(OBJECT *this, OBSERVER *observer);
void objectNotify(OBJECT *this);

void objectInit(OBJECT *o)
{
    memset(o, 0, sizeof(OBJECT));

    o->addObserver = objectAddObserver;
    o->notify = objectNotify;
    return;
}

int objectAddObserver(OBJECT *this, OBSERVER *observer)
{
    assert(observer);
    assert(this);
    this->pObserverList[this->num] = observer;
    this->num++;

    return this->num;
}

void objectNotify(OBJECT *this)
{
    int i;
    assert(this);

    for (i = 0; i < this->num; i++)
    {
        if (this->pObserverList[i] != NULL)
        {
            this->pObserverList[i]->update(this->pObserverList[i], this);
        }
    }
}

// observer1
void observer1Udpate(OBSERVER *this, OBJECT *pObject);
void observer1Init(OBSERVER *ob)
{
    memset(ob, 0, sizeof(OBSERVER));
    ob->id = 1;
    strcpy(ob->name, "ob1");
    ob->update = observer1Udpate;

    return;
}

void observer1Udpate(OBSERVER *this, OBJECT *pObject)
{
    assert(pObject);
    assert(this);
    printf("%s update data:%d\n", this->name, pObject->data);
}

// observer2
void observer2Udpate(OBSERVER *this, OBJECT *pObject);

void observer2Init(OBSERVER *ob)
{
    memset(ob, 0, sizeof(OBSERVER));
    ob->id = 2;
    strcpy(ob->name, "ob2");
    ob->update = observer2Udpate;

    return;
}

void observer2Udpate(OBSERVER *this, OBJECT *pObject)
{
    assert(pObject);
    assert(this);
    printf("%s update data:%d\n", this->name, pObject->data);
}

void bindObserverToObject(OBSERVER *pObserver, OBJECT *pObject)
{
    assert(pObserver);
    assert(pObject);

    pObject->addObserver(pObject, pObserver);
}

int main()
{
    OBJECT o;
    OBSERVER ob1;
    OBSERVER ob2;

    objectInit(&o);

    // 动态添加新的观察者
    observer1Init(&ob1);
    bindObserverToObject(&ob1, &o);

    // 数据变化通知观察者
    o.data = 10;
    objectNotify(&o);

    // 动态添加新的观察者
    observer2Init(&ob2);
    bindObserverToObject(&ob2, &o);

    // 数据变化通知观察者
    o.data = 20;
    objectNotify(&o);

    return 0;
}