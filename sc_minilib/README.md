SC MINILIB
==========

Version: 0.3 (beta)
Date   : 28/09/2023
Author : Giuseppe Lipari (glipari@univ-lille.fr)

Introduction
------------

A minimal C++ library for implementing State Machines (StateCharts) on
Linux / POSIX.

At the lowest level, the library uses pthreads to implement the basic
event engine of a state machine.

The highest level is implemented based on the "Composite Pattern" for
hierarchical states. The current version only supports a minimal
subset of the StateChart semantic:
- Hierarchical states
- Orthogonal (parallel) regions
- Shallow history
- Entry / exit actions to / from states (do() actions are not
  currently supported)

COMPILING 
---------

To compile the library and the examples:

	mkdir build
	cd build 
	cmake ..
	make 

If you want to run the internal tests for the library, you can instead type : 

	cmake -DWITH_TESTS .. 
	make 
	
This will download and install the Catch2 testing framework and
compile all the tests in the test/ directory (this may take some time ...).

To execute the fan example:

	./build/examples/fan/fandemo

TO BE DONE
----------
- No-event transitions based on the concept of internal TICK events
- do actions (with arbitrary periods)
- Join and synchronization
- final states


