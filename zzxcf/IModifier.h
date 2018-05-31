#pragma once
/*
Goals: OS portable, modifier plugable, C++ standard library perferred


C++ abstract class, interface
https://docs.microsoft.com/en-us/cpp/cpp/abstract-classes-cpp


Using virtual destructors, you can destroy objects without knowing their type
the correct destructor for the object is invoked using the virtual function mechanism.
https://docs.microsoft.com/en-us/cpp/cpp/destructors-cpp


Synonym Discussion of modify
change, alter, vary, modify mean to make or become different.

change implies making either an essential difference often amounting to a loss of original identity or a substitution of one thing for another.
changed the shirt for a larger size

alter implies a difference in some particular respect without suggesting loss of identity.
slightly altered the original design

vary stresses a breaking away from sameness, duplication, or exact repetition.
vary your daily routine

modify suggests a difference that limits, restricts, or adapts to a new purpose.
modified the building for use by the disabled
https://www.merriam-webster.com/dictionary/modifies


a group of things that come one after the other
https://www.merriam-webster.com/dictionary/sequence
*/
#include "..\common\TokenSequence.h"
class IModifier
{
public:
	virtual ~IModifier() {}
public:
	virtual TokenSequence&Modify(TokenSequence&) = 0;
};
class DummyModifier : public IModifier
{
public:
	TokenSequence&Modify(TokenSequence&tokenSequence)
	{
		return tokenSequence;
	}
};
/*
http://www.cplusplus.com/reference/stdexcept/
header <stdexcept>
Exception classes
This header defines a set of standard exceptions that both the library and programs can use to report common errors.
They are divided in two sets:

Logic errors

logic_error
Logic error exception (class )
domain_error
Domain error exception (class )
invalid_argument
Invalid argument exception (class )
length_error
Length error exception (class )
out_of_range
Out-of-range exception (class )

Runtime errors
runtime_error
Runtime error exception (class )
range_error
Range error exception (class )
overflow_error
Overflow error exception (class )
underflow_error
Underflow error exception (class )


http://www.cplusplus.com/reference/exception/
header <exception>
Standard exceptions
This header defines the base class for all exceptions thrown by the elements of the standard library: exception,
along with several types and utilities to assist handling exceptions:

Types:
exception
Standard exception class (class )
bad_exception
Exception thrown by unexpected handler (class )
nested_exception
Nested exception class (class )
exception_ptr
Exception pointer (type )
terminate_handler
Type of terminate handler function (type )
unexpected_handler
Type of unexpected handler function (type )

Functions
terminate
Function handling termination on exception (function )
get_terminate
Get terminate handler function (function )
set_terminate
Set terminate handler function (function )
unexpected
Function handling unexpected exceptions (function )
get_unexpected
Get unexpected handler function (function )
set_unexpected
Set unexpected handler function (function )
uncaught_exception
Return exception status (function )
current_exception
Get smart pointer to current exception (function )
rethrow_exception
Rethrow exception (function )
make_exception_ptr
Make exception_ptr (function template )
throw_with_nested
Throw with nested (function )
rethrow_if_nested
Rethrow if nested (function )
*/
#include <exception>
#include <stdexcept>
class IDecoratingModifer :public IModifier
{
protected:
	IModifier *decoratedModifier;
	virtual TokenSequence&ActionBeforeModify(TokenSequence&) = 0;
	virtual TokenSequence&ActionAfterModify(TokenSequence&) = 0;
public:
	IDecoratingModifer(IModifier *modifier)
	{
		if (nullptr == modifier)
		{
			throw std::invalid_argument("This is decorating modifier constructor, decorated modifier(argument) cannot be nullptr.");
		}
		this->decoratedModifier = modifier;
	}
	~IDecoratingModifer()
	{
		delete this->decoratedModifier;
	}
public:
	TokenSequence&Modify(TokenSequence&tokenSequence)
	{
		return this->ActionAfterModify(this->decoratedModifier->Modify(this->ActionBeforeModify(tokenSequence)));
	}
};

class DecoratingModifer_remove_empty_token : public IDecoratingModifer
{
private:
	void erase(TokenSequence&tokenSequence, TokenSequence::iterator&it)
	{
		TokenSequence::iterator temp = it;
		it--;
		tokenSequence.erase(temp);
		it++;
	}
	
	TokenSequence&ActionBeforeModify(TokenSequence&tokenSequence)
	{
		TokenSequence::iterator it = tokenSequence.begin();
		while (tokenSequence.end() != it)
		{
			if (it->empty())
			{
				this->erase(tokenSequence, it);
			}
			else
			{
				it++;
			}
		}
		return tokenSequence;
	}
	TokenSequence&ActionAfterModify(TokenSequence&tokenSequence)
	{
		return ActionBeforeModify(tokenSequence);
	}
public:
	DecoratingModifer_remove_empty_token(IModifier *modifier):IDecoratingModifer(nullptr == modifier ? new DummyModifier() : modifier)	{ }
};