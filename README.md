- [Introduction](#org4d8f134)
  - [Compiling the library and executing the examples](#orgf90739b)
  - [Compile your program](#orgec5f67a)
- [Exercise](#org38866c5)



<a id="org4d8f134"></a>

# Introduction

The goal of the following exercise is to implement a statechart on the Linux OS.

We will use the `sc_minilib` library :

<https://gitlab-etu.fil.univ-lille.fr/cle-2023/sc_minilib>

which is already included in this repository as a submodule.

As always, fork this repository in your private namespace, and add the teacher as member of your project. Then clone it with the following command:

```sh
  git clone --recurse-submodules <the url of your project>
```


<a id="orgf90739b"></a>

## Compiling the library and executing the examples

To compile and execute the examples of the library, create a `build_sc` directory. Then:

```sh
   mkdir build_sc
   cd build_sc
   cmake ../sc_minilib
   make
```

To execute the examples, type:

```sh
   ./examples/fan/fandemo
   ./examples/hierarchy/hierarchy
   ./examples/coffee/coffee
```


<a id="orgec5f67a"></a>

## Compile your program

To compile and execute your program, create a directory `build_ac` and then:

```sh
   mkdir build_ac
   cd build_ac
   cmake ..
   make
```

This will create an executable `ac` that you can run with:

```sh
   ./ac 
```


<a id="org38866c5"></a>

# Exercise

The goal of the exercise is to implement the AC system that you designed in the previous TP, with the `sc_minilib` library.

To simulate the automatic mode, you can change the external temperature with an appropriate command. For example, you can use the following commands:

-   key "\*" to turn on/off the system;
-   key "m" to switch to manual mode and change between "Heating" and "Cooling";
-   key "+" and "-" to increase/decrease the desired temperature;
-   key "t" to enter a value of the current tempeature.

**Optional**: To better show the results on the screen, you can use the ncurses library:

-   <https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/helloworld.html>
-   <https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/helloworld.html#COMPILECURSES>

This allows you to interact with the system in a more natural way (e.g. by simple keypresses and output at fixed positions in the screen).

You can also use a graphical library like Gnome/GTK, if you feel competent enough to do it (but I cannot give you any help on that).