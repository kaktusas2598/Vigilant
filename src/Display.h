#ifndef DISPLAY_H
#define DISPLAY_H

namespace Display
{
	void init();
	void close();

	void clear();
	void update();

	void checkForClose();

	bool isOpen();
}

#endif
