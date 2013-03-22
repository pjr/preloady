preloady
========

Fun with LD_PRELOAD and embedded python

Allows you to override system calls via LD_PRELOAD and pass those up into a
python interface where the objects can be manipulated. You simply write a hook and
manipulate data.

You can choose to call the original system function (e.g. change some input
data and then call it), or ignore the system one completely.

Due to the C interfaces, we need to have actually written code for every function
you wish to override.
