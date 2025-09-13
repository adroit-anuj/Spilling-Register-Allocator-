# Optimizing Register Allocator

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)](https://example.com)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Stars](https://img.shields.io/badge/stars-0-orange.svg)](https://example.com)

A C++ implementation of the Iterated Register Coalescing IRC algorithm, an optimised method for solving the register allocation problem in compilers.

## About the project

Computers provide a small number of very fast storage locations called registers. Typical programs use many more values than there are registers. The register allocator in a compiler decides which temporary or variable should live in a register at each program point. A naive approach can waste registers and generate slow code. This project implements Iterated Register Coalescing IRC which is used by production compilers to produce efficient register assignments. The allocator aims to reduce move instructions by coalescing compatible values and to handle register pressure by selecting good candidates for spilling to main memory.

## Key features

* Complete IRC implementation in C++
* Move coalescing to reduce move instructions
* Intelligent spilling strategy to minimize runtime overhead
* Interference graph representation and graph coloring approach
* Clear API for defining the interference graph and moves
* Small test harness to validate allocation results

## Technologies used

* C++
* Graph theory
* Compiler design concepts

## Project structure

* allocator.cpp  main implementation and entry point
* allocator.h  class definitions and public interfaces
* graph.h  interference graph utilities
* moves.h  move list and move utilities
* tests  small test cases and input files
* examples  sample graphs and usage examples
* LICENSE  project license
* README.md  this file

## Getting started

### Prerequisites

* A C++ compiler that supports C++11 or newer for example g++
* A POSIX like shell for build and run commands

### How To use

The allocator expects three inputs from the client code or from tests:

1. **The integer `K`** – number of available registers  
2. **An interference graph** – mapping each variable id to the set of variable ids it conflicts with  
3. **A list of moves** – the allocator should try to coalesce  

The typical flow in client code is:

- Construct the interference graph  
- Provide the move pairs  
- Create an `Allocator` instance  
- Run the allocation  
- Query the register assignment and the list of spilled variables  

