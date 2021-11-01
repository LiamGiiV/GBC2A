#include <fstream>  
#include <string>
#include <vector>

#define MAX_HEADER 8

int main(int argc, char *argv[])
{
	std::string filename;
	std::fstream* fileStream = nullptr;
	std::fstream* newFile = nullptr;
	std::vector<std::string> fileContents;

	// get command line arument
	if (argc == 2 && argv)
	{
		fileStream = new std::fstream(argv[1]);
		filename = argv[1];
		filename.erase(filename.size() - 1, 1);
		filename.insert(filename.size(), "z80");
		newFile = new std::fstream(filename, std::ios_base::out); // ???
	}
	else
	{
		printf("Expected one command-line argument, only a complete filename.");
	}

	if (fileStream != nullptr && fileStream->is_open())
	{
		std::string temp;
		int i = 0;
		while (std::getline(*fileStream, temp))
		{
			if (i > MAX_HEADER)
			{
				fileContents.push_back(temp);
			}
			i++;
		}

		fileContents.pop_back();
		fileContents.pop_back();

		// Change '0x' to '$'
		for (auto it = fileContents.begin(); it != fileContents.end(); it++)
		{
			int position = 0;

			// Erase '\t' and add 'DB ' to the front of each line
			it->erase(position, 1);
			it->insert(position, "DB ");

			// Replace "0x" with '$'
			do
			{
				position = it->find("0x");
				if (position > -1)
				{
					it->erase(position, 2);
					it->insert(position, "$");
				}
			}
			while (position != std::string::npos);

			if (it->back() == ',')
			{
				it->erase(it->size() - 1, 1);
			}

			it->insert(it->size(), "\n");

			newFile->write(it->c_str(), it->size());
		}
	}

	if (fileStream)
	{
		delete fileStream;
	}

	if (newFile)
	{
		delete newFile;
	}

	return 0;
}