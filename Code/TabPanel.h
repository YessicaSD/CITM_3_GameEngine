#ifndef TABPANEL_H_
#define TABPANEL_H_
#include <vector>
class Panel;
struct TabPanel
{
public:
	int x = 0, y = 0, width = 0, height = 0;
	char* name = nullptr;
	std::vector<Panel*> panels;
	bool isActive = true;

};
#endif // !TABPANEL_H_

