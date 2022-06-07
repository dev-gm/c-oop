# c-oop
OOP (interfaces, generics, methods, encapsulation), implemented in C using clang's blocks (-fblocks)

## Prerequisites:
- zig
- clang
- libblocksruntime-dev

## Constructs implemented:
- \[Y] vector.h

I don't know if anyone has done this, but it's pretty simple.
Clang's blocks (also known as closures) can be used as members to a struct.
That struct can be an interface, and classes can be implemented by creating functions
that return instances of that struct. These functions can create private variables, which
can only be accessed by the blocks. This allows for public/private variables (encapsulation),
proper method call syntax (obect.verb()), interfaces, and classes. Generics can also be
implemented using macros, which take in a type as one of their arguments and create a new
interface that uses that type (rather than void \*), and child classes (functions) that also
use that generic type. Unfortunately, C doesn't support anonymous structs, so you must
declare the specific types that you want to use as generics with macros, and then
implement them, also using a macro.

## DISCLAIMER
I have run some simple benchmarks, and my vector.h, implemented using closures, is
approxiamately 2-3 times slower than a traditional, C-style approach. If you want
to use OOP, just use C++.
