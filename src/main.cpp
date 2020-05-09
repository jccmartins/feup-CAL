#include "Manager.h"
#include "Graph.h"
#include "lib/graphviewer.h"
#include "Interface.h"

int main()
{
	srand(time(NULL));
	Manager<int> *manager = new Manager<int>();
	Interface<int> interface(manager);
	interface.chooseMap();

	return 0;
}
