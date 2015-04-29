# Alpha Terrain Generation #

## Summary ##

This is a small hobby project experimenting procedurally generating simple terrain using fractional Brownian Motion and using OpenGL / SDL for rendering. Currently the program allows users to fly around a world which will keep generating procedurally generating terrain as the user travels around the world.

![http://3.bp.blogspot.com/-EAtQ8f70w7s/T81Oy1jMuvI/AAAAAAAAARw/nZv0zpmgsU8/s1600/example_01.png](http://3.bp.blogspot.com/-EAtQ8f70w7s/T81Oy1jMuvI/AAAAAAAAARw/nZv0zpmgsU8/s1600/example_01.png)

## About ##

Currently the software has the following features:

  * Dynamic Terrain generation
    * Generates new terrain when moving around
    * Dynamically removes distant terrain segments / garbage collection
  * Texture mapping on terrain
  * Coloured terrain based on heights and gradients
  * Fog

## Libraries / Prerequisites ##
The program uses SDL, OpenGL and FreeGlut for graphics and input.
Therefore it requires the following packages (tested on Windows):

  * SDL.dll
  * freeglut.dll
  * glut32.dll

And currently compiled using MinGW for Win32 and requires additionally:
  * libgcc\_s\_dw2-1.dll
  * libstdc++-6.dll
