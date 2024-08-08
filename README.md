<h1>SIMPLE ECHO SERVER</h1>
<p>The Server uses only one process to server all the clients sounds silly but modelling a single process then spawning several to do same job and have master to monitor the can be quite roust<br/></p>
<div>Its a huge main functions but echo server logic is simple </div>
<h2>How it works</h2>
<p>
The server do its normal job, creat socket, bind and listen, while the client create socket and connect provided client is aware of servers listening port and address<br/>
After call to listen the server invokes select with listening file descriptor in read set, the read set also contains client connected file descriptors after accept returns<br>
On Clien FIN, appropriate connected file descriptor is ready for reading and return 0 which indicate client closed the connection and the server can close the file descriptor associated to free system resources<br/>
The client code also uses select to avoid blocking on standard input when the server process is terminated<br/>

Also i recently found out how to misuse C goto, used it in the code as a loop <br/>
<p>

<h3> Test </h3>
<p>I have hardcoded the server well known port in the code and IP<br/>
echo server well known port is 7 which requires sudo privileges change it in the defined macro,
Also the IP address only listening on localhost, You might wonna
keep it that way when you are runnning to prevent opening up your machine to the world <br/>
</p>
<code>git clone</code> 

<footer>own view -> SELECT CAN BE ANNOYING SINCE YOU WILL HAVE TO CHECK (fd_set) TO KNOW WHICH FILE DESCRIPTORS ARE READY AND THIS CAN REALLY SLOW THINGS</footer>
