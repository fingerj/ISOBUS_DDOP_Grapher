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

#include "xmlparser.h"
#include "graph.h"
#include "node.h"

#define TIXML_USE_STL
#include "../tinyxml/tinyxml.h"

#include <shlobj.h>
#include <shlwapi.h>

#include <sys/types.h>
#include <sys/stat.h>

#ifdef WIN32
#define sscanf sscanf_s
#endif

namespace
{
	int maxFailedWaits = 10;
}

XmlParser::XmlParser()
	: m_files()
{
}

void
XmlParser::parseFiles()
{
	//open each file and create a graph for each device found.
	
	TiXmlNode* root;
	for( unsigned i = 0; i < m_files.size(); ++i )
	{
		TiXmlDocument doc( m_files[i] );
		if( !doc.LoadFile() 
			|| !( root = doc.RootElement() ) )
		{
			std::cout << "Error loading " << m_files[i] << std::endl;
			continue;
		}

		std::cout << "loading " << m_files[i] << std::endl;

		while( root )
		{
			const char* name = root->Value();
			if( name
				&& 0 == strcmp( name, "DVC" ) )
			{
				parseDvc( root, m_files[i] );
			}
			else
			{
				checkElementForDvcs( root, m_files[i] );
			}
			root = root->NextSibling();
		}
	}
}

void 
XmlParser::checkElementForDvcs( TiXmlNode* node, const std::string& file )
{
	TiXmlNode* child = node->FirstChild();
	while( child )
	{
		const char* name = child->Value();
		if( name
			&& 0 == strcmp( name, "DVC" ) )
		{
			parseDvc( child, file );
		}
		else
		{
			checkElementForDvcs( child, file );
		}
		child = child->NextSibling();
	}
}

void
XmlParser::parseDvc( TiXmlNode* dvc, const std::string& file )
{
	//make a graph
	std::string name = file.substr(0, file.find_last_of( "." ) );
	name = name.substr( name.find_last_of( "/\\" ) + 1, std::string::npos );

	TiXmlElement* dvcElem = dvc->ToElement();
	if( !dvcElem )
	{
		std::cout << " convert Error " << std::endl;
	}

	const char* attr = dvcElem->Attribute( "A" );
	if( !attr 
		|| strlen( attr ) == 0 )
	{
		attr = dvcElem->Attribute( "D" );
		if( !attr )
		{
			std::cout << "Error parsing DVC in file: " << file << std::endl;
			return;
		}
	}

	name += "_";
	name += attr;

	Graph dvcGraph( name );

	TiXmlNode* child = dvc->FirstChild();
	while( child )
	{
		const char* name = child->Value();
		if( name )
		{
			if( 0 == strcmp( name, "DET" ) )
			{
				parseDet( child->ToElement(), dvcGraph );
			}
			else if( 0 == strcmp( name, "DPD" ) )
			{
				parseDpd( child->ToElement(), dvcGraph );
			}
			else if( 0 == strcmp( name, "DPT" ) )
			{
				parseDpt( child->ToElement(), dvcGraph );
			}
		}
		child = child->NextSibling();
	}

	dvcGraph.resolveNodeDdiDependencies();

	dvcGraph.print();
	m_GraphFiles.push_back( dvcGraph.name() );
	//dvcGraph.draw();
}

void
XmlParser::drawGraphs( const std::string& dotExecutable, int maxProcessCount )
{
	struct stat s_stat;
	if( ( stat( dotExecutable.c_str(), &s_stat ) != 0 )
		|| ( ( s_stat.st_mode & S_IFREG ) == 0 )
		|| ( ( s_stat.st_mode & S_IEXEC ) == 0 ) )
	{
		std::cout << "Failed to find, or not executable: " << dotExecutable << std::endl;
		return;
	}

	char commandBuffer[32768];
	STARTUPINFOA si;     
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	si.hStdError = NULL;
	si.hStdOutput = NULL;
	si.hStdInput = NULL;
	si.dwFlags |= STARTF_USESTDHANDLES;
	ZeroMemory( &pi, sizeof(pi) );

	int processCount = 0;
	int nextHandle = 0;
	HANDLE* handles = new HANDLE[maxProcessCount];
	int failCounter = 0;
	
	std::cout << "Creating PNGs:";
	for( unsigned i = 0; i < m_GraphFiles.size(); ++i )
	{
		std::string param = " -Tpng dot\\";
		param += m_GraphFiles[i] + ".dot";
		param += " -opng\\";
		param += m_GraphFiles[i] + ".png ";

		strncpy( commandBuffer, param.c_str(), param.length() );

		if( ( 0 != CreateProcessA( dotExecutable.c_str(), commandBuffer, NULL, NULL, false, 0, NULL, NULL, &si, &pi ) ) )
		{
			processCount++;
			handles[ nextHandle++ ] = pi.hProcess;
		}
		else
		{
			std::cout << "\nFailed to execute " << dotExecutable << " " << param << std::endl;
		}

		while( processCount >= maxProcessCount )
		{
			int waitRet;
			while( ( waitRet = WaitForMultipleObjects( maxProcessCount, handles, false, 500 ) ) == WAIT_TIMEOUT )
			{
				std::cout << ".";
			}

			if( waitRet != WAIT_FAILED )
			{
				std::cout << ".";
				processCount--;
				nextHandle = waitRet - WAIT_OBJECT_0;
			}
			else if( ++failCounter > maxFailedWaits )
			{
				std::cerr << "\nFailed to wait for dot process to execute " << maxFailedWaits << " times." << std::endl;
				delete[] handles;
				return;
			}
		}
	}
	
	//wait for remaining processes to finish
	while( processCount >= maxProcessCount )
	{
		int waitRet;
		while( ( waitRet = WaitForMultipleObjects( processCount, handles, true, 500 ) ) == WAIT_TIMEOUT )
		{
			std::cout << ".";
		}

		if( waitRet != WAIT_FAILED )
		{
			processCount--;
		}
		else if( ++failCounter >= maxFailedWaits )
		{
			std::cerr << "\nFailed to wait for dot process to execute " << maxFailedWaits << " times." << std::endl;
			delete[] handles;
			return;
		}
	}
	
	std::cout << "\nFinished Drawing Dot files" << std::endl;
	delete[] handles;
}

void
XmlParser::parseDet( TiXmlElement* det, Graph& graph )
{
	if( !det )
	{
		std::cout << "Invalid DET " << std::endl;
	}

	static std::string noName = "NoName";
	int objectId, parent, element;
	int type;
	const std::string* name;
	if( !(name = det->Attribute( std::string("D"))) )
	{
		name = &noName;
	}
	if( !det->Attribute("C", &type) )
	{
		std::cout << "Error getting type of DET: " << name
			<< " in graph: " << graph.name() << std::endl;
		return;
	}
	if( !det->Attribute("B", &objectId) )
	{
		std::cout << "Error getting objId of DET: " << name
			<< " in graph: " << graph.name() << std::endl;
		return;
	}
	if( !det->Attribute("F", &parent) )
	{
		std::cout << "Error getting parent of DET: " << name
			<< " in graph: " << graph.name() << std::endl;
		return;
	}
	if( !det->Attribute("E", &element) )
	{
		std::cout << "Error getting element number of DET: " << name
			<< " in graph: " << graph.name() << std::endl;
		return;
	}

	Node* node = new Node( *name, objectId, parent, type, element );

	parseDors( det, node );

	graph.addNode( node );
	graph.addEdge( objectId, parent );

#ifdef DEBUG
	//std::cout << "Added DET: " << objectId << " to graph " << graph.name() << std::endl;
#endif
}

void
XmlParser::parseDors( TiXmlElement* elem, Node* node )
{
	TiXmlElement* child = elem->FirstChildElement();
	while( child )
	{
		const char* name = child->Value();
		if( name
			&& 0 == strcmp( name, "DOR" ) )
		{
			int attrA;
			if( child->Attribute( "A", &attrA ) )
			{
				node->addRef( attrA );
			}
			else
			{
				std::cout << "Error Reading DOR from DET: " << node->objId() << std::endl;
			}
		}
		child = child->NextSiblingElement();
	}
}

void
XmlParser::parseDpd( TiXmlElement* dpd, Graph& graph )
{
	if( !dpd )
	{
		std::cout << "Invalid DPD " << std::endl;
	}

	static std::string noName = "NoName";
	int objectId, ddi, type, methods;
	const std::string* name;

	if( !(name = dpd->Attribute( std::string("E"))) )
	{
		name = &noName;
	}
	if( !dpd->Attribute("C", &type) )
	{
		std::cout << "Error getting properties of DPD: " << name
			<< " in graph: " << graph.name() << std::endl;
		return;
	}
	if( !dpd->Attribute("A", &objectId) )
	{
		std::cout << "Error getting objId of DPD: " << name
			<< " in graph: " << graph.name() << std::endl;
		return;
	}

	const char* hexDdi; 
	if( !( hexDdi = dpd->Attribute("B"))
		|| ( sscanf( hexDdi, "%x", &ddi ) != 1 ) )
	{
		std::cout << "Error getting DDI of DPD: " << name
			<< " in graph: " << graph.name() << std::endl;
		return;
	}
	if( !dpd->Attribute("D", &methods) )
	{
		std::cout << "Error getting Methods of DPD: " << name
			<< " in graph: " << graph.name() << std::endl;
		return;
	}
	int value = ((type & 0x07) << 5) | (methods & 0x1f);

	DDI* ddiObj = new DDI( *name, objectId, ddi, false, value );
	graph.addDdi( ddiObj );

#ifdef DEBUG
	//std::cout << "Added DPD: " << objectId << " to graph " << graph.name() << std::endl;
#endif
}

void
XmlParser::parseDpt( TiXmlElement* dpt, Graph& graph )
{
	if( !dpt )
	{
		std::cout << "Invalid DPT " << std::endl;
	}

	static std::string noName = "NoName";
	int objectId, ddi, value;
	const std::string* name;

	if( !(name = dpt->Attribute( std::string("D"))) )
	{
		name = &noName;
	}
	if( !dpt->Attribute("C", &value) )
	{
		std::cout << "Error getting value of DPT: " << name
			<< " in graph: " << graph.name() << std::endl;
		return;
	}
	if( !dpt->Attribute("A", &objectId) )
	{
		std::cout << "Error getting objId of DPT: " << name
			<< " in graph: " << graph.name() << std::endl;
		return;
	}
	
	const char* hexDdi;
	if( !( hexDdi = dpt->Attribute("B"))
		|| ( sscanf( hexDdi, "%x", &ddi ) != 1 ) )
	{
		std::cout << "Error getting DDI of DPT: " << name
			<< " in graph: " << graph.name() << std::endl;
		return;
	}

	DDI* ddiObj = new DDI( *name, objectId, ddi, true, value );
	graph.addDdi( ddiObj );

#ifdef DEBUG
	//std::cout << "Added DPT: " << objectId << " to graph " << graph.name() << std::endl;
#endif
}

