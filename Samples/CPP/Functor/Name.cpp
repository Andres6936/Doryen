// Joan Andrés (@Andres6936) Github.

#include "Name.hpp"

using namespace Doryen;

Functor::Name::Name(std::string _name, Console& _console) : ISample(_name, _console)
{

}

void Functor::Name::render(KeyCode key, const Mouse& mouse)
{
	static int nbSets = 0;
	static int curSet = 0;
	static float delay = 0.0f;
	static TCODList<char*> names;
	static TCODList<char*> sets;
	int i;
	if (nbSets == 0)
	{
		std::vector<std::filesystem::directory_entry> files =
				Doryen::Platform::getDirectoryContent("Data/namegen", ".cfg");

		// parse all the files
		for (auto it = files.begin(); it != files.end(); it++)
		{
			TCODNamegen::parse(it->path());
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

	sample.setDefaultBackground(Doryen::Palette::PRIMARY_LIGHT);
	sample.clear();
	sample.setDefaultForeground(Doryen::Palette::GRAY_WARN_1);

	sample.write(1, 1, format("{}", sets.get(curSet)));
	sample.write(1, 3, "+ : next generator");
	sample.write(1, 4, "- : prev generator");

	for (i = 0; i < names.size(); i++)
	{
		char* name = names.get(i);

		if (strlen(name) < sample.getWidth())
		{
			sample.write(30, 2 + i, name);
		}
	}

	delay += sample.getLastFrameLength();
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
