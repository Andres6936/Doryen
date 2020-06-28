// Joan Andrés (@Andres6936) Github.

#include "Name.hpp"

using namespace Doryen;

Functor::Name::Name(std::string _name, const Console& _console) : ISample(_name), sample(_console)
{

}

void Functor::Name::operator()(KeyCode key, const Mouse& mouse)
{
	static int nbSets = 0;
	static int curSet = 0;
	static float delay = 0.0f;
	static TCODList<char*> names;
	static TCODList<char*> sets;
	int i;
	if (nbSets == 0)
	{
		TCODList<char*> files = Doryen::Platform::getDirectoryContent("Data/namegen", "*.cfg");
		// parse all the files
		for (char** it = files.begin(); it != files.end(); it++)
		{
			char tmp[256];
			sprintf(tmp, "Data/namegen/%s", *it);
			TCODNamegen::parse(tmp);
		}
		// get the sets list
		sets = TCODNamegen::getSets();
		nbSets = sets.size();
	}

	while (names.size() >= 15)
	{
		// remove the first element.
#ifndef TCOD_VISUAL_STUDIO
		char* nameToRemove = *(names.begin());
#endif
		names.remove(names.begin());
		// for some reason, this crashes on MSVC...
#ifndef TCOD_VISUAL_STUDIO
		free(nameToRemove);
#endif
	}

	sample.setDefaultBackground(Doryen::Color::lightBlue);
	sample.clear();
	sample.setDefaultForeground(Doryen::Color::white);
	sample.print(1, 1, format("{}\n\n+ : next generator\n- : prev generator", sets.get(curSet)));

	for (i = 0; i < names.size(); i++)
	{
		char* name = names.get(i);
		if (strlen(name) < sample.getWidth())
		{
			sample.printEx(sample.getWidth() - 2, 2 + i, TCOD_BKGND_NONE, TCOD_RIGHT, name);
		}
	}

	delay += Doryen::Platform::getLastFrameLength();
	if (delay >= 0.5f)
	{
		delay -= 0.5f;
		// add a new name to the list
		names.push(TCODNamegen::generate(sets.get(curSet), true));
	}
	if (key == KeyCode::KP_ADD)
	{
		curSet++;
		if (curSet == nbSets)
		{ curSet = 0; }
		names.push(strdup("======"));
	}
	else if (key == KeyCode::KP_SUB)
	{
		curSet--;
		if (curSet < 0)
		{ curSet = nbSets - 1; }
		names.push(strdup("======"));
	}
}
