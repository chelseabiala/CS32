void removeBad(list<Movie*>& li)
{
    for(list<Movie*>::iterator it = li.begin(); it != li.end();)
    {
        if((*it)->rating() <= 55)
        {
            Movie* temp = *it;
            it = li.erase(it);
            delete temp;
        }
        else
            it++;
    }
}
