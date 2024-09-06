
# 📁  FTP PROTOCOL



![App Screenshot](https://mac.eltima.com/images/upload/commander/articles/what-is-ftp/ftp-diagram.png)


## 📄Table of content
1. [Project Overview](#project-overview)
2. [Project Proposal](#project-proposal)
2. [Project Goal](#goal)
3. [File Structure](#file-structure)
## Project Overview
🧑🏽‍💻 For impelementation of this project i used c as  programming language . The 🗃️ FTP protocol operates on a client-server model where the client requests file operations and the server provides the requested services. This implementation covers the essential functionalities of FTP, including connecting to a server, transferring files, and managing directory structures.
### Key Components

### 1. Client-Server Architecture
   - **Client**: Sends requests to the server for file operations, such as uploading, downloading, listing files.
   - **Server**: Listens for incoming client connections, processes requests, and performs the required file operations.

### 2. Networking
   - **Socket Programming**: Utilizes sockets for network communication between the client and server.
   - **Protocols**:For the implementation of FTP, I used TCP as the protocol because it provides reliable data transfer through its handshake methodology

### 3. File Operations
   - **File Transfer**: Handles uploading  and downloading files between the client and server.
   - **Memory Reuction**: Before transferring data to the server, the file is zipped to reduce memory utilization during the transfer

### 4. Error Handling
   - **Connection Errors**: Manages issues related to network connectivity and socket operations.
   - **Protocol Errors**: Handles incorrect or unexpected commands and responses.

## Project Proposal
While selecting this project, my goal was to learn more about how sockets work in web or mobile applications and to deepen my understanding of the TCP protocol. By creating this project, I gained a better understanding of the need for multithreading, abstraction, and how multiple clients can communicate with each other.
## Goal
- **Client-Server Architecture**: Implementing a basic client-server architecture to manage communication between clients and the server.

- **File Operations**: Performing file creation and deletion using system calls to manage files on the server.

- **Multithreading**: Utilizing multiple threads on the server to handle simultaneous connections from multiple clients efficiently.

- **Storing Client Data**: Selecting optimal data structures to store details of connected clients, enabling efficient unicasting.

- **Abstraction and Modularity**: Designing folder structures and data flow to allow users to focus on implementation details without needing to understand the underlying workings.

# File Structure
```ssh
FTP_PROTOCOL/
├── /heaser_impelementation
│   ├── reuse_imp.c
│   ├── server_create_implemenatation.c
│   ├── store_user.c
│   └── zip_imp.c
├── headers/
│   ├── create_socket.h
│   ├── file_zip.h
│   ├── reuse_func.h
│   └── store_user.h
│   
├── src/
│   ├── client.c
│   ├── server.c
|   ├── client_interface.h
│   ├── server_interface.h
|   ├── create_client.sh
│   ├── craete_server.sh
│   ├── client_data/
│   ├── result/
│   └── server_data/
|         |──  send_file.txt
|         |__ user.txt
├── .gitignore
└── README.md
```




## Getting Started
### Prerequisites
  - **GCC Complier: Ensure you have the GCC Compile installed on your system**
  - **Linux the file used in this project are linux specific**
  - **Libraries to install: lzip, pthread**
### Cloning the Repository
1. #### clone the repository from git hub
```bash
git clone https://github.com/sameerkumar2704/FTP_PROTOCOL.git
```
2. #### Navigate into the project direactory:
```bash
cd FTP_PROTOCOL
```
### Compliation 
1. for client
```bash
gcc client.c -o client.out ../headers/*.h -lzip -pthread
```
or

```bash
src/create_server.h
```
2. for server 
```bash
gcc server.c -o server.out ../headers/*.h -lzip -pthread
```
or

```bash
src/create_client.h
```
### Start Program
```bash
client.out
server.out
```
# Tutorial
## Custom-Commands 
- **show-all-users**
- **select-client**
- **exit**
   

https://github.com/user-attachments/assets/67e0b63d-0dd2-4fb9-a766-33322f42a4d2



    
