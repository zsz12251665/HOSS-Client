#include "todolist.h"

ToDoList::ToDoList(QObject *parent) : QObject(parent)
{
    //样例
    mItems.append({true, QStringLiteral("打球")});
    mItems.append({false, QStringLiteral("游戏")});
}

QVector<ToDoItem> ToDoList::items() const
{
    return mItems;
}
bool ToDoList::setItemAt(int index, const ToDoItem &item)
{

    if(index<0 || index >= mItems.size())
        return false;

    const ToDoItem &oldItem = mItems.at(index);

    if(item.done == oldItem.done && item.description == oldItem.description){
        return false;
    }

    mItems[index] = item;
    return true;
}

void ToDoList::appendItem()
{
    emit preItemAppend();

    ToDoItem item;
    item.done = false;
    mItems.append(item);

    emit postItemAppended();
}
void ToDoList::removeCompletedItems()
{
    for (int i=0;i<mItems.size();) {
        if(mItems.at(i).done){
            emit preItemRemoved(i);

            mItems.removeAt(i);

            emit postItemRemoved();
        }
        else{
            ++i;
        }
    }
}
