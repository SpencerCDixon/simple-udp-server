## UDP Server in C

This repo contains a simple UDP server/client written in C.  The primary purpose
was for education and this should not be used in anything real. [The server is
well documented with comments.](./server.c)

### Installation

If on MacOS:

```sh
$ brew install netcat
```

We need to use `netcat` instead of `telnet` since `telnet` does not support the
UDP protocol.

**Building the server** 

```sh
$ make server && ./server
```

Now our UDP server should be running on port `8888`.  We can test it out with
the `netcat` client by opening a new terminal and running:

```sh
$ netcat -v localhost 8888 -u
```

* `-v` will use verbose mode
* `-u` will use UDP protocol

Once the client is connected you can type random words and it will get sent back
to you.

```sh
$ hello
hello
```

## Using Our Own Client

Start the server in one screen:
```sh
$ make server && ./server
```

Open a new tab and start the client:
```sh
$ make client && ./client
```

Then start typing messages.

## Resources

* [Socket programming](http://beej.us/net2/html/intro.html)
