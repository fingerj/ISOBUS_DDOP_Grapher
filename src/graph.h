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

class Node;
class DDI;

class Graph
{
public:
	Graph( const std::string& name ) : m_Name( name ) {};
	virtual ~Graph();

	void addNode( Node* );
	void addDdi( DDI* );
	void addEdge( unsigned int child, unsigned int parent );
	void resolveNodeDdiDependencies();

	const std::string& name() const { return m_Name; };

	void print();

private:
	std::string m_Name;
	std::vector<Node*> m_Nodes;
	std::vector<DDI*> m_Ddis;
	std::vector< std::pair<unsigned int, unsigned int> > m_Edges;

};


