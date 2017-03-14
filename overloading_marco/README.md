# Overloading Marco

* Its a `Overloading Marco`, when you typing different argument in marco, It will execute different function
* For example, enter 3 argument in marco "out", it will execute marco "in2", and enter 2 argument, it will execute marco "in1".
<pre>
	out(1, 2) -> mid(1, 2, in2, in1)(1, 2)
	mid(x, y, z, func, ...) func -> mid(1, 2, in2, in1) in1
	func = in1
	run : in1
</pre>


<pre>
	out(1, 2, 3) -> mid(1, 2, 3, in2, in1)(1, 2, 3)
	mid(x, y, z, func, ...) func -> mid(1, 2, 3, in2, in1) in2
	func = in2
	run : in2
</pre>
