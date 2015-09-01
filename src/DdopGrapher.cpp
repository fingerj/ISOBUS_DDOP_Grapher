/*
Copyright (c) 2015 jacob.finger@agcocorp.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#define VER_MAJOR	1
#define VER_MINOR	0
#define VER_PATCH	0

#include <iostream>
#include "xmlparser.h"

#include <windows.h>

void printUsage()
{
	std::cout << "Usage: DdopGrapher file(1).xml file(2).xml ... file(N).xml\n"
		<< "  Parses input list of files for ISO11783 DVC elements.\n"
		<< "  For each DVC element an attempt will be made to make a dot graph and a PNG file with name 'file'_[AtrributeA].\n"
		<< "  Output will be in sub directories dot and png.\n"
		<< " -D\t\tUse dot.exe from PATH\n"
		<< " -d dot\t\tUse 'dot' as dot exectuable, default is ../dot/dot.exe\n"
		<< " -p count\tUse 'count' dot processes when drawing, default is 8\n"
		<< " -v\t\tPrint Version and exit" << std::endl;
	std::cout << "Press Enter to exit." << std::endl;
	char exitWait = std::cin.get();
}

int main(int argc, char* argv[])
{
	XmlParser parser;
	std::string dotExecutable = "..\\dot\\dot.exe";
	int processCount = 8;

	// skip arg 0 (executable name)
	for( int i = 1; i < argc; ++i )
	{
		std::cout << "arg: " << i << ", " << argv[i] << std::endl;
		if( argv[i][0] != '-' )
			parser.addFile( argv[i] );
		else
		{
			switch( argv[i][1] )
			{
				case 'd':
					if( argc > i+1 )
					{
						dotExecutable = argv[++i];
						std::cout << "dot executable: " << dotExecutable << std::endl;
					}
					else
					{
						std::cout << "Missing dot executable for -d option" << std::endl;
						std::cout << "Press Enter to exit." << std::endl;
						char exitWait = std::cin.get();
						return 0;
					}
					break;

				case 'p':
					if( argc > i+1 )
					{
						processCount = atoi( argv[++i] );
						if( processCount < 1 )
						{
							std::cout << "Invalid value for -p option" << std::endl;
							std::cout << "Press Enter to exit." << std::endl;
							char exitWait = std::cin.get();
							return 0;
						}
						std::cout << "process count: " << processCount << std::endl;
					}
					else
					{
						std::cout << "Missing process count for -p option" << std::endl;
						std::cout << "Press Enter to exit." << std::endl;
						char exitWait = std::cin.get();
						return 0;
					}
					break;
					
				case 'D':
					dotExecutable = "dot.exe";
					std::cout << "Use PATH for dot executable." << std::endl;
					break;

				case 'v':
					std::cout << "Version: " << VER_MAJOR << "." << VER_MINOR << "." << VER_PATCH << std::endl;
					return 0;
				default:
				printUsage();
				return 0;
			}
		}
	}

	CreateDirectoryA( "dot" , NULL );
	parser.parseFiles();

	std::cout << "Finished Generating Dot files" << std::endl;
	
	CreateDirectoryA( "png" , NULL );
	parser.drawGraphs( dotExecutable, processCount );

	std::cout << "Press Enter to exit." << std::endl;
	char exitWait = std::cin.get();

	return 0;
}

