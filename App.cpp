#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>

// whether absolute file paths should be shown, uncomment to include
//#define SHOW_ABSOLUTE 

void ProcessCodeFile(std::filesystem::path file);

int main(int argc, char** argv)
{
	// verify input
	if (argc == 1)
	{
		std::cout << "Invalid Usage: specify a path ('.' for current path)" << std::endl;
		return 1;
	}
	
	// setup cwd filesystem
	std::string cwdPath = argv[1];
	std::filesystem::path cwd = std::filesystem::path(cwdPath);

#ifdef SHOW_ABSOLUTE
	std::cout << "Listing Directories in '" << std::filesystem::absolute(cwd) << '\'' << std::endl;
#else 
	std::cout << "Listing Directories in '" << cwd.string() << '\'' << std::endl;
#endif

	// loop through cwd recursively 
	for (auto const& dir_entry : std::filesystem::recursive_directory_iterator(cwd))
	{
		std::string path = dir_entry.path().string();

		// store the extension by sub stringing from the last '.' onward
		std::string extension;
		if (!dir_entry.is_directory())
			extension = path.substr(path.find_last_of('.'));
		else
			extension = "Directory";

		// if this is a code file
		if (extension == ".h" || extension == ".cpp")
		{
			ProcessCodeFile(dir_entry.path());
		}
	}

	return 0;
}

void ProcessCodeFile(std::filesystem::path file)
{
	int lineCount = 0;

	// open file and verify success
	std::ifstream fin(file);
	if (fin.fail())
		__debugbreak();

	std::string inputLine;

	// loop through file by line
	while (std::getline(fin, inputLine))
	{
		lineCount++;

		std::size_t found  = inputLine.find("TODO");
		std::size_t found2 = inputLine.find("todo");

		std::string path = "";

#ifdef SHOW_ABSOLUTE 
		path = std::filesystem::absolute(file).string();
#else
		path = file.string();
#endif

		if (found != std::string::npos)
		{
			std::cout << "TODO: (" << path << ' ' << lineCount << ") " << inputLine.substr(found + 4) << std::endl;
		}
		else if (found2 != std::string::npos)
		{
			std::cout << "TODO: (" << path << ' ' << lineCount << ") " << inputLine.substr(found2 + 4) << std::endl;
		}
	}
}
