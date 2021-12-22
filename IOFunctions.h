#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>


namespace IOFunctions
{
	inline void LoadMapFromFile(std::string FilePath, std::vector<std::string>& grid)
	{
		char tile;

		//x and y size of the file
		int sizeX = 0;
		int sizeY = 0;


		//create and open .txt file
		std::ifstream mapFile;
		mapFile.open(FilePath);

		//get rows
		std::string line;
		while (std::getline(mapFile, line))
			sizeY++;


		//get columns
		std::stringstream s(line);
		char value;
		//while there's something in the line, ignore ','  and increase the number of columns
		while (s >> value)	if (value != ',') sizeX++;


		grid.resize(sizeY);


		//start from beginning of the file 
		mapFile.clear();
		mapFile.seekg(0);


		//populate the array
		for (int y = 0; y < sizeY; y++)
			for (int x = 0; x < sizeX; x++)
			{
				mapFile.get(tile);

				grid[y] += tile;

				mapFile.ignore();
			}


		mapFile.close();
	}
}