void listAll(const File* f, string path)  // two-parameter overload
{
    if(path == "")
    {
        path = "/" + f->name();
        cout << path << endl;
        path += "/";
    }
    if(f->files() == nullptr)
        return;
    for(vector<File*>::const_iterator it = f->files()->begin(); it != f->files()->end(); it++)
    {
        cout << path + (*it)->name() << endl;
        listAll(*it, path + (*it)->name() + "/");
    }
}
