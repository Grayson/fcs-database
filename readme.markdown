# fcs::database

This repository is a collection of C++ files to represent saving structured data in a SQLite database.  It represents a collection of files (in `src`) that provide a C++ API abstraction over the [SQLite][] API.  It is packaged as an Xcode project and provides a basic setup script.

## Motivation

There are plenty of C++ abstractions on top of SQLite, so there's little reason to prefer it to other libraries.  I wrote `fcs::database` for one basic reasons:

I was disatisfied with the approach taken by other C++ libraries.  Many of them appeared to hew very closely to providing C++ objects around the C API.  This is fine, but I was looking for a more application-centric design.  This abstraction does not attempt to recreate the SQLite C API using C++ idioms.  Rather, it provides a more natural API for interacting with databases.

## Requirements and Setup

This project uses [SQLite][] as the database implementation and [Catch][] for unit testing.  Both should be downloaded into a folder called "external" at the root of the repository.  For convenience, a shell script (`setup.sh`) can be run to automate downloading these prerequisites.

## Usage

There is no documentation for using this code, either in the source or externally managed.  The best source for intended usage are the tests.  Please look to them for examples of how to use `fcs::database`.

## Contact

Email: [Grayson Hansard](grayson.hansard@gmail.com)  
Twitter: [@Grayson](http://twitter.com/Grayson)

[SQLite]: (https://www.sqlite.org)
[Catch]: (https://github.com/philsquared/Catch)