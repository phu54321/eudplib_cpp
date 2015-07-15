#define VERSION "0.01-prototype"

#include "Linker.hpp"
#include "Module.hpp"
#include <fstream>

int main(int argc, char* argv[])
{
	printf("eudplib eud modules linker " VERSION);
	if(argc < 3)
	{
		printf("Usage : linker [output file] [input1] [input2] ... [inputN]\n");
		return -1;
	}

	// Load modules
	std::vector<ModulePtr> modules;
	bool hasError = false;
	for(int i = 2; i < argc; i++)
	{
		std::ifstream is(argv[i]);
		if(is.fail())
		{
			fprintf(stderr, "Linker error 690 : Cannot load module file \"%s\".\n", argv[i]);
			hasError = true;
			continue;
		}
		else
		{
			modules.push_back(std::make_shared<Module>(argv[i], is));
		}
	}
	if(hasError)
	{
		fprintf(stderr, "Link aborted.\n");
		return -2;
	}

	// Create payload
	PayloadPtr payload = linkModules(modules);
	if(!payload)
	{
		fprintf(stderr, "Link aborted.\n");
		return -3;
	}
	bytes payloadData = payload->toBytes();
	
	// Output module
	std::ofstream os(argv[1]);
	if(os.fail())
	{
		fprintf(stderr, "Linker error 358 : Cannot save payload to output file \"%s\".\n", argv[1]);
		fprintf(stderr, "Link aborted.\n"); 
		return -4;
	}
	os.write((const char*)payloadData.data(), payloadData.size());

	fprintf(stderr, "Link completed.\n");
	return 0;
}
