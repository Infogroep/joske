Joske
=====

Joske is the experimental IRC bot for the Infogroep IRC channel.
It uses libircclient for IRC connectivity. For its language it would be nice
to use a packrat parser, but that's still future work. :-)

A note about libircclient
=========================
When the client is unable to connect to the server due to for example a
firewall blocking it, then there is no diagnostic message, and the library
treats this as an internal error. It would be nice to provide a patch to
upsteam. :-)

For example, at the time of writing, our little demo client will complain with:
ERROR: failure running main event loop (No error)
