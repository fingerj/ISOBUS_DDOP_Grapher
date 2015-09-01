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

#include "graph.h"
#include "node.h"

#include <fstream>
#include <iostream>

#include <shlobj.h>
#include <shlwapi.h>
#include <objbase.h>

Graph::~Graph()
{
	for( unsigned i = 0; i < m_Nodes.size(); ++i )
	{
		delete m_Nodes[i];
	}
	for( unsigned i = 0; i < m_Ddis.size(); ++i )
	{
		delete m_Ddis[i];
	}
}

void
Graph::addNode( Node* node )
{
	m_Nodes.push_back( node );
}

void
Graph::addEdge( unsigned int child, unsigned int parent )
{
	m_Edges.push_back( std::pair< unsigned int, unsigned int >( child, parent ) );
}

void
Graph::addDdi( DDI* ddi )
{
	m_Ddis.push_back( ddi );
}

void
Graph::resolveNodeDdiDependencies()
{
	for( unsigned i = 0; i < m_Nodes.size(); ++i )
	{
		Node* node = m_Nodes[i];
		const std::vector<int>& refs = node->getRefs();
		for( unsigned j = 0; j < m_Ddis.size(); ++j )
		{
			for( unsigned k = 0; k < refs.size(); ++k )
			{
				if( refs[k] == m_Ddis[j]->objId() )
				{
					node->addDdi( m_Ddis[j] );
					break;
				}
			}
		}
	}
}

void
Graph::print()
{
	std::ofstream file;
	std::string fileName = "dot\\";
	fileName += m_Name + ".dot";
	file.open( fileName.c_str() );
	std::cout << "Print: " << fileName.c_str() << std::endl;

	file << "digraph {\n"
			<< "\tgraph [ranksep=0.25, fontname=Arial, nodesep=0.125];\n"
			<< "\tnode [color=\"#E8EEF7\"fontname=Arial, style=filled, height=0, shape=box, fontcolor=black];\n"
			<< "\tedge [fontname=Arial];" << std::endl;

	for( unsigned i = 0; i < m_Nodes.size(); ++i )
	{
		m_Nodes[i]->print( file );
	}
	
	for( unsigned i = 0; i < m_Edges.size(); ++i )
	{
		if( m_Edges[i].second != 0 )
			file << "\t" << m_Edges[i].second << " -> " << m_Edges[i].first << std::endl;
		//std::cout << m_Edges[i].first << " " << m_Edges[i].second << std::endl;
	}

	file << "}" << std::endl;
	file.close();
}
