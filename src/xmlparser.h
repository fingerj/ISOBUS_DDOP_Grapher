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

#pragma once

#include <string>
#include <vector>

class TiXmlNode;
class TiXmlElement;
class Graph;
class Node;

class XmlParser
{
public:
	XmlParser();
	~XmlParser(){}

	void addFile( const std::string& file ) { m_files.push_back(file); }

	void parseFiles();

	void drawGraphs( const std::string& dotExecutable, int processCount );

private:
	void checkElementForDvcs( TiXmlNode*, const std::string&  );
	void parseDvc( TiXmlNode*, const std::string& );
	void parseDors( TiXmlElement*, Node* );
	void parseDet( TiXmlElement*, Graph& );
	void parseDpd( TiXmlElement* dpd, Graph& graph );
	void parseDpt( TiXmlElement* dpt, Graph& graph );

	std::vector< std::string > m_files;
	std::vector< std::string > m_GraphFiles;
};
