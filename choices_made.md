<!-- vim: set spell spelllang=en_us: -->
# Engineering choices

## Parser

### First Streak

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

I could have used exceptions to say that there was an error, this
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

Therefore I chose to first check what is the next object to
parse before trying to parse something, I believe that it would
lead to better code. That might give a slower syntax analyzer
but I am willing to give away a bit of speed to be able to give
a detailed error to the user.

I will first implement without giving any information but I will
extend it when I will see it working

While I'm coding the test to see which parsing I should do, I start
to notice that I will more or less write twice the same code which,
as much as the use of exception as condition, is ugly. I think
I should change the object return by the parsing functions,
maybe a structure in the style:
```cpp
style ParsingResult
{
	PasingInfo parse_info;
	ParseTree parse_tree;
}
```
I'm not to sure about this, when I think about it, it is quite
hard to find a way to do something clean when you need to
do the exact same thing for building the parse tree and knowing if you can actually
build it.

I think that I will create my language so that it is local, it
will be easier to program and might not be too much of a hindrance.
I will try to force myself into thinking that it is not an issue
since this is the first language that I am creating.

To represent the parse tree I am choosing to have all the nodes
to inherit from a base class `ParseTreeBase` and the terminal node
to be a template representing terminal nodes because I only want to
specialize the thing for some terminal and not writing a class
for every terminal nodes. I will do the exact same thing for non
terminal nodes

EDIT:
Instead of giving different names to the template classes
for terminal nodes and non terminal nodes, I will create
only one template class with one implementation enabled
if the Token is terminal and another implementation enabled
otherwise. This should produce a cleaner code without losing
information, since the fact that the node is terminal or not
is stored in the template argument.

Every template classes must inherit from the same base class,
since I need to store different types with the help
of polymorphism in the `std::vector` that
store the children of non terminal nodes: an arithmetic expression
might have (number) or (arithmetic expression, operator,
arithmetic expression) as children. This base class must have
a runtime token member to cast correctly the pointer from a
pointer to an object of type `ParseTreeBase` to an object
with the wanted parse tree type. We can't store a compile
time static member in each one of the instantiation of
the template since the static member won't be accessible
if the object is manipulated with a pointer to a base
class object.

It might be possible to
do something safer with the help of type computation: to have
a metafunction that return the possibles children types if given
a Token.


### Second Streak

I finished to code the first draft of the `ParseTree` a while ago
(probably 2-3 months). Before testing the my parse tree I wanted
to have a nice way to print the parse tree with indentation. Therefore
I coded a `std::streambuf` whose state can be change with IOManipulator,
I should have implement the indentation with a `std::facet` rather
than a `std::streambuf`. Stream buffer are not used to manipulate
the data passed to a stream but they are used to transfer data and
a facet is used to manipulate the data, therefore the facet is
a better choice to ease the use of indentation with streams.
I wanted to move on since I spent a lot of time on the stream buffer,
so I didn't try to implement the indentation with a `std::facet`.

To implement an overload of `<<` for parse trees I implemented
an overload of the operator for `std::vector` and then a
template overload for every Container, to avoid ambiguity in
the overload of `<<` the compiler chose:
```cpp
template<typename T>
std::ostream& operator<<(std::ostream& os, const T& t)
{
	os << "[" << *t.begin() << "]";
}
```
The compiler doesn't know how to chose between this overload and
the existing overload when `std::cout << "Hello!"` appear in
the program. I searched for a library that introduced concept,
Boost seemed fine but I didn't manage to make it work, most
of the other solutions were outdated.I started implementing
my own concepts following the standard definition of these concept.

After I had finished some concept checking metafunctions I came
back on the parse tree to look at the code and to reformat it, I
had changed a bit of my coding style during my metaprogramming
period. While looking at the code I had to rethink why I had
implemented the parse tree in such a way. Since I don't
want to have to rethink every time I look at my parse tree
I will put an EDIT in the First Streak to explain my choices.
