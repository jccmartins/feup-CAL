#include "Manager.h"
#include "Graph.h"
#include "lib/graphviewer.h"
#include "Interface.h"

int main()
{
	srand(time(NULL));
	Manager<long> *manager = new Manager<long>();
	Interface<long> interface(manager);
	interface.chooseMap();

	return 0;
}
