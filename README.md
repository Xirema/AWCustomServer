# AWCustomServer

## What is this?

This is the Server for AWCustom. It is a C++ Server written principally using [boost.beast](https://www.boost.org/doc/libs/1_84_0/libs/beast/doc/html/index.html).

It uses a MySQL server to persist data.

## Build Environment

This server builds for Linux using GCC-14 or higher.

## Required Libraries

* boost (tested with 1.84.0)
* FreeImage (Not necessary for the server, but required for utilities)
* OpenSSL 1.1
* zlib (tested with 1.2.12)

## Build Instructions

1. Setup Libraries
  * Install Boost 1.84.0 or later
    * This will require downloading of the source and building for the target environment. The defaults are fine, so from the command line it's just `./bootstrap` and `./b2`.
  * Install OpenSSL
    * `sudo apt install openssl`
  * Install FreeImage and zLib
  * Install GCC-14 or later
2. Run Cmake to setup build targets
  * My usual setup is to make a directory `release` from the root of the project, and run `cmake -DCMAKE_BUILD_TYPE=Release ..` from that directory
  * For debug the command is `cmake -DCMAKE_BUILD_TYPE=Debug ..` (which should be done from a `debug` directory instead for disambiguation)
3. Run `make` to build the project
4. Create `app.properties` in the build directory, filled with the following properties:
  * serverPort=2001
  * dbHostName=localhost
  * dbPort=3306
  * dbUser=<dbname>
  * dbPass=<dbpass>
  * dbName=
5. Run the server. From the `release` directory the command is `./AWCustomServer/Server` (assuming `app.properties` is in the `release` folder)

The server will error when it tries to perform SQL operations because the server doesn't know how to setup the tables itself on the server. This will be fixed later.

