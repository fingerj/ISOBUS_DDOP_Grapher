ISOBUS Device Description Object Pool Grapher

The DdopGrapher is a utility to generate directed graphs from ISO-11783 xml DVC
elements. Output looks similar to many AEF implement diagrams. The source code specific
to DdopGrapher excluding the compiled in TinyXML code and excluding the external
software being used is licensed under the MIT license. A copy of the license agreement
is printed below.


The MIT License (MIT)

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



DDOP GRAPHER DISTRIBUTION


1. 3rd PARTY SOFTWARE
The DdopGrapher software makes use of TinyXML source code. A copy of 
the TinyXML source code is contained in this source distribution. 
TinyXML license:
   TinyXML is released under the zlib license:   
   This software is provided 'as-is', without any express or implied warranty.
   In no event will the authors be held liable for any damages arising from the 
   use of this software.
   Permission is granted to anyone to use this software for any purpose, 
   including commercial applications, and to alter it and redistribute it freely,
   subject to the following restrictions:
   1. The origin of this software must not be misrepresented; you must not claim 
      that you wrote the original software. If you use this software in a product,
 	  an acknowledgment in the product documentation would be appreciated but is 
	  not required.
   2. Altered source versions must be plainly marked as such, and must not be 
      misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.


The DdopGrapher software uses Graphviz's dot program to generate PNG images.
For convenience, this distribution contains a copy of the binary distribution of
the Graphviz software. You can download the full binary distribution from Graphviz's
website http://www.graphviz.org/Download_windows.php . License information is 
provided in the subdirectory containing the Graphviz copy.


2. CONTENT
The distribution contains the following sub directories
	compiler_prj: contains a VisualStudio 2012 solution to build the DdopGrapher
	              executable for Microsoft Windows 7 and above. 
	dot:          directory containing the required dynamic link libraries and
	              executables from Graphviz's binary distribution for Microsoft
				  Windows operating system (see above)
	output:       target directory for output files
	src:          source code files for DdopGrapher
	tinyxml:      source code files from TinyXML to be compiled into DdopGrapher 
	
	
3. USAGE
DdopGrapher is a simple to use command line program. I prefer to drag the file
containing DVC's I want drawn onto the excutable. This will execute the DdopGrapher
with default settings on the file. 

Usage: DdopGrapher file(1).xml file(2).xml ... file(N).xml
  Parses input list of files for ISO11783 DVC elements.
  For each DVC element an attempt will be made to make a dot graph and a PNG file with
  name 'file'_[AtrributeA].
  Output will be in sub directories dot and png.
 -D             Use dot.exe from PATH
 -d dot         Use 'dot' as dot exectuable, default is ../dot/dot.exe
 -p count       Use 'count' dot processes when drawing, default is 8
 -v             Print Version and exit
 

4. TRADEMARK INFORMATION
	Microsoft, Windows, Visual Studio and the Windows logo are trademarks or registered 
	trademarks of Microsoft Corporation.				  
	