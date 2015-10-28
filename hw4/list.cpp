void listAll(const MenuItem* m, string path) // two-parameter overload
{
	if (m == nullptr)
		return;
	path += m->name();
	
	if (path != "")
		cout << path << endl;
	
	if (m->menuItems() == nullptr)
		return;
	if (path != "")
		path += "/";

	const vector<MenuItem*>* ptr = m->menuItems();
	for (int k = 0; k < ptr->size(); k++)
		listAll((*ptr)[k], path);
}