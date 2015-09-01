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
#include <ostream>

class DDI
{
public:
	DDI( const std::string& name, unsigned int objId, unsigned int ddi, bool dpt, int value ) 
		: m_Name( name )
		, m_ObjId( objId )
		, m_DDI( ddi )
		, m_IsDpt( dpt )
		, m_Value( value )
	{};
	virtual ~DDI(){};

	unsigned int ddi() const { return m_DDI; };
	unsigned int objId() const { return m_ObjId; };
	const std::string& name() const { return m_Name; };

	void print( std::ostream& );
	void draw();

private:
	std::string m_Name;
	unsigned int m_ObjId;
	unsigned int m_DDI;
	bool m_IsDpt;
	int m_Value;
};


class Node
{
public:
	Node( const std::string& name, unsigned int objId, unsigned int parent, int type, unsigned int element ) 
		: m_Name( name )
		, m_ObjId( objId )
		, m_Parent( parent )
		, m_Type( type )
		, m_Element( element )
	{};
	virtual ~Node(){};

	void addRef( int );
	const std::vector<int>& getRefs() const { return m_Refs; };
	void addDdi( DDI* );

	unsigned int parent() const { return m_Parent; };
	unsigned int objId() const { return m_ObjId; };
	const std::string& name() const { return m_Name; };
	unsigned int element() const { return m_Element; };

	void print( std::ostream& );
	void draw();

private:
	const std::string& getShape();
	void printDdis( std::ostream& );

	std::string m_Name;
	unsigned int m_ObjId;
	unsigned int m_Parent;
	int m_Type;
	unsigned int m_Element;
	std::vector<DDI*> m_Ddis;
	std::vector<int> m_Refs;
};


