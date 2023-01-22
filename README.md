# ft_irc
<p>
  <b>Internet Relay Chat</b> or IRC is a text-based communication protocol on the Internet.
  It offers real-time messaging that can be either public or private. Users can exchange
  direct messages and join group channels.
</p>
The server is written according to the <a href="https://www.ietf.org/rfc/rfc1459.txt">rfc1459<a> standard.
<h3> Execute/Start Server </h3>

```
./ircserv <port> <password>
```

<ul>
  <li> port: The port number on which your IRC server will be listening to for incoming IRC connections. </li>
  <li> password: The connection password. It will be needed by any IRC client that tries to connect to your server. </li>
</ul>

<h3> Client </h3>
After the server is started, clients can connect to it. On a local server, you can use:

```
nc localhost <port>
```
