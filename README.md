# AWCustomServer

## What is this?

This is the Server for AWCustom. It is a C++ Server written principally using [boost.beast](https://www.boost.org/doc/libs/1_82_0/libs/beast/doc/html/index.html).

It uses a MySQL server to persist data.

## Build Environment

This server builds for Windows using Visual Studio 2022. I'm happy to convert the project over to compile for Linux if someone can explain to me how to do that.

## Required Libraries

* boost (tested with 1.82.0)
* FreeImage (Not necessary for the server, but required for utilities)
* OpenSSL 1.1
* zlib (tested with 1.2.12)

## Build Instructions

1. Build all of the required libraries
   * I might try to add instructions here, because these libraries can be a bit finicky. The important part is that all of them should compile for the Multithreaded, Shared Runtime for C++. We compile both Debug and Release builds.
2. Place the include folders/files in Library/include
   * This is what my include directory looks like:![image](https://user-images.githubusercontent.com/1447761/232623968-c3a3f304-7804-4c06-8af4-c2e38539527e.png)
3. Copy the static library files into Library/lib
   * You'll need to make sure that they are separated into debug/release folders, respectively. The debug libraries go into the debug folder, and should include
      * FreeImaged.lib (FreeImage)
      * libcrypto.lib (OpenSSL)
      * libssl.lib (OpenSSL)
      * zlibstaticd.lib (zlib)
   * In the Release Folder, these are
      * FreeImage.lib (FreeImage)
      * libcrypto.lib (OpenSSL)
      * libssl.lib (OpenSSL)
      * zlibstatic.lib (zlib)
   * All Boost libraries should be copied into Library/lib/boost. All of them, release and debug; they do not need to be separated.
   * This is what my folders look like: 
      * ![image](https://user-images.githubusercontent.com/1447761/232624581-3fe34cfe-aee2-4c20-9961-e0e33b2f4845.png)
![image](https://user-images.githubusercontent.com/1447761/232624607-0322db11-3888-4466-bdfd-5745f2787c22.png)
![image](https://user-images.githubusercontent.com/1447761/232624628-8f9c3901-89c0-428e-a123-1be2af4db472.png)
4. Copy the Shared library files into Library/bin
   * For Debug:
      * FreeImaged.dll (FreeImage)
      * libcrypto-1_1-x64.dll (OpenSSL)
      * libssl-1_1-x64.dll (OpenSSL)
   * For Release:
      * FreeImage.dll (FreeImage)
      * libcrypto-1_1-x64.dll (OpenSSL)
      * libssl-1_1-x64.dll (OpenSSL)
   * This is what my folders look like:
      * ![image](https://user-images.githubusercontent.com/1447761/232625405-9f3fd99b-9ae3-490e-a22a-e6cbd43b9175.png)
![image](https://user-images.githubusercontent.com/1447761/232625438-b06cdfd8-002c-4bc2-9dc1-74dba6e92c01.png)
5. Open the AWCustomServer Solution, and Build the Solution, swapping between Debug and Release to make sure both compile
   * The only project that needs to compile is AWCustomServer, which will create an associated .exe in Build/[Debug|Release]/ that can be run. The other projects are useful utilities, but not necessary to run the server.
6. Place AWCustomServer.exe into a folder to deploy from, along with the shared libraries (use the debug or release versions, respectively, if you did a debug or release build)
7. Create app.properties in the folder with AWCustomServer.exe
8. Populate app.properties with the following properties:
   * serverPort=8167
   * dbHostName=[URL of your MySQL Server; use localhost if you're doing development]
   * dbPort=3306 (this is the default MySQL port; change to something else if you changed the port on your MySQL server)
   * dbUser=[User on your MySQL server with the appropriate permissions]
   * dbPass=[Password for the User on your MySQL server]
   * dbName= (leave it blank, unless your database has a name; in that case, supply that name)

The server will error when it tries to perform SQL operations because the server doesn't know how to setup the tables itself on the server. This will be fixed later.

