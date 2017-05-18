<!-- vim: set spell spelllang=en_us: -->
# Engineering choices

## Parser
The only way to parse the current object that came to my mind was to first
try to parse one of the beginning object of the current object
and see if it fails, if it fails I would try another beginning
object of the current object of if there was no more beginning object
to try I would tell the program that the parse failed, If the
parsing of the beginning object didn't failed I would go on to
the objects that follow the beginning object and if there was no
object following the beginning object I would have considered
the `ParseTree` generated and return it

I could have return an empty `ParseTree` that had as `token`
`UNKNOWN` but that would have prevented me to give information
on the syntax error that was in the user program, and I want to
give as much information to the user as I can since the
language is really obscure.

I could have use exceptions to say that there was an error, this
is exactly what they should be used for. But I had to use the
throwing of exception as condition which I believe to be quite awful:
```cpp
try {
	parseSomething(begin, end)
} catch (parsing_dont_work_exception) {
	try {
		parseSomethingElse(begin, end)
	} catch(parsing_dont_work_exception) {
		//A lot more ugly code
	}
}
```
Maybe there would be a way to make it prettier. But still using
exception as condition never seem to me to be the best way to
code something.

Therefore I chosen to first check what is the next object to
parse before trying to parse something, I believe that it would
lead to better code. That might give a slower syntax analyzer
but I am willing to give away a bit of speed to be able to give
a detailed error to the user.

I will first implement without giving any information but I will
extend it when I will see it working

While I'm coding the test to see which parsing I should do, I start
to notice that I will more or less write twice the same code which
as much as the use of exception as condition is ugly. I think
I should change the object return by the parsing functions,
maybe a structure in the style:
```cpp
style ParsingResult
{
	PasingInfo parse_info;
	ParseTree parse_tree;
}
```
I'm not to sure about this, when I think about it, it is quite an
hard thing to find a way to do something clean when you need to
do the exact same thing as building to know if you can actually
build and to know how to build it!

I think that I will create my program so that it is local, it
will be easier to program and might not be too much of a hindrance.
I will try to force myself into thinking that it is not an issue
since this is the first language that I am creating.

To represent the parse tree I am choosing to have all the nodes
to inherit from a base class `ParseTreeBase` and the terminal node
to be a template representing terminal nodes because I only want to
specialize the thing for some terminal and not writing a class
for every terminal nodes. I will do the exact same thing for non
terminal nodes
