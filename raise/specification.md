
# Raise Library

## Specificiation v1.0
## by codetorex

# Philosophy
Provide high-level stuff in both fast and safe manner in C++ environment to let 
people able to accomplish things with very few libraries.

If it will be too time consuming to write some algorithms, we can just use other
libraries that compatible with our licence and wrap them with our "style".

# Rules

## 1. Test-Driven development
You must be able to reimplement code if it got deleted, from just by looking up 
test cases. Also every single thing must be tested via testing environment provided.

## 2. Documentation
Every function and class must be documented.

## 3. Meaningful function, classname and overall code
Names for functions and classes should be self explaining and must not exceed 16 characters.


## 4. Cross-platform compilation support
Our primary targets are Windows and Ubuntu. Our library will be tested in these platform first.

Our secondary platforms are Mac OS and mobile platforms, like Android.

## 5. Git based source control
We will use Git as our source control solution. Because it is the best.

## 6. Code wrapping

# Design

## Try-Catch
Always throw a derivative of Exception class. Without using "new" keyword and catch it as like;

	try
	{
		throw Exception("Message");
		throw NotImplementedException();
	}
	catch(Exception& e)
	{
		Log.Output( LG_ERR , e.Message );
	}

## Tool Classes
Classes that provides static tool functions can be named like;


	class MemoryTools
	{
	public:
		static void Copy(void* src, void* dst, int count);
	}

	extern MemoryTools Memory;


So we can use this class like;

	Memory.Copy( src, dst, 64 );

## String
Our string class uses UTF-8 encoding internally for supporting full unicode character planes. Also string functions are optimized if string is only composed of ASCII characters.


## Safety vs Speed
So in programming sometimes we have to sacrifice one or other. Our main focus is speed, but we should provide safe versions of these classes or functions. So default classes will be optimized for performance. And if you going to provide safer alternative you may name it like ``SString``.

## Constructors
Constructors should prepare class for any work. But it shouldn't do any work by itself. It should just set internal state of object.

# MVC Internals

Currently MVC stack is working like this; Request is delivered over FastCGI.
We are parsing necessary stuff from request and building the Controller object.
(In future we can get it from cache or something for faster processing?)
Controller finds the view file and generates model. If view file is changed,
it parses it and compiles it. Then runs its main with generated Model.


# License

This piece of software will be dual licensed. GPL for producing open source software.
Temporary commerical license that allows release of software commerically for entities 
doesn't earned 10k$ yet from products produced with this library. 
Commerical license should be bought for producing commerical software with yield over
10k$.