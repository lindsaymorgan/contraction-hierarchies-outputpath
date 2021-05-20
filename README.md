Contraction Hierarchies
=======================

Base on https://github.com/cetusteam/contraction-hierarchies

Introduction
------------

This program allows to create and test contraction hierarchies (CH), a
hierarchical, shortcut-based speedup technique for fast and simple
routing in road networks. The code was created as part of a diploma
thesis at Universitaet Karlsruhe (TH), Institute for Algorithmics II.

Papers: - R. Geisberger, P. Sanders, D. Schultes, D. Delling.
          Contraction Hierarchies: Faster and Simpler Hierarchical Routing
          in Road Networks. In 7th International Workshop on Efficient and
          Experimental Algorithms (WEA), 2008.

Required libraries
------------------

- STL
- Boost IO-streams & regular expressions (only for input/output)

How to build & run
------------------

use Clion

Sources of documentation
------------------------

In docs/index.html, there are some examples what command-line arguments
can be used to create a CH and perform tests.

Usage
------------------------

Use for fast many to many shortest path query - output path

In newexample folder

Input: graph.ddsg query.txt

Output:result.txt
3 lines for 1 query
1. Source Target Length
2. Path Nodes
3. Path Edges Weight
