# Programming Project - C++ EDAF50
This project is a newssystem that consists of a newsserver and a newsclient. The server holds information regarding newsgroups and articles. The client is text based and can list/create/delete newsgroups and articles.


## Installation

* Enter src and type the following commands to create all necessary files and executables. Make install copies the executables to bin

```
make all 
make install
```
## Dependencies
This program uses sqlite3 and therefore has the files sqlite3.c and sqlite3.h in the src folder. These were downloaded from the official [sqlite homepage](https://www.sqlite.org/download.html).

## Running the program

* Run the newsserver by specifying the port
* Example

```
./newsserver 3000
``` 
* Open a new terminal and run the newsclient by specifying hostname and portnumber, these need to be the same as the servers
* Example

```
./newsclient localhost 3000
```
