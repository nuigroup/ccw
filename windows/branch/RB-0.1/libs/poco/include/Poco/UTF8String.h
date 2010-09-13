//
// UTF8String.h
//
// $Id: //poco/1.3/Foundation/include/Poco/UTF8String.h#2 $
//
// Library: Foundation
// Package: Text
// Module:  UTF8String
//
// Definition of the UTF8 string functions.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Foundation_UTF8String_INCLUDED
#define Foundation_UTF8String_INCLUDED


#include "Poco/Foundation.h"


namespace Poco {


struct Foundation_API UTF8
	/// This class provides static methods that are UTF-8 capable variants
	/// of the same functions in Poco/String.h.
	///
	/// The various variants of icompare() provide case insensitive comparison
	/// for UTF-8 encoded strings.
	///
	/// toUpper(), toUpperInPlace(), toLower() and toLowerInPlace() provide
	/// Unicode-based character case transformation for UTF-8 encoded strings.
{
	static int icompare(const std::string& str, std::string::size_type pos, std::string::size_type n, std::string::const_iterator it2, std::string::const_iterator end2);
	static int icompare(const std::string& str1, const std::string& str2);
	static int icompare(const std::string& str1, std::string::size_type n1, const std::string& str2, std::string::size_type n2);
	static int icompare(const std::string& str1, std::string::size_type n, const std::string& str2);
	static int icompare(const std::string& str1, std::string::size_type pos, std::string::size_type n, const std::string& str2);
	static int icompare(const std::string& str1, std::string::size_type pos1, std::string::size_type n1, const std::string& str2, std::string::size_type pos2, std::string::size_type n2);
	static int icompare(const std::string& str1, std::string::size_type pos1, std::string::size_type n, const std::string& str2, std::string::size_type pos2);
	static int icompare(const std::string& str, std::string::size_type pos, std::string::size_type n, const std::string::value_type* ptr);
	static int icompare(const std::string& str, std::string::size_type pos, const std::string::value_type* ptr);
	static int icompare(const std::string& str, const std::string::value_type* ptr);

	static std::string toUpper(const std::string& str);
	static std::string& toUpperInPlace(std::string& str);
	static std::string toLower(const std::string& str);
	static std::string& toLowerInPlace(std::string& str);
};


} // namespace Poco


#endif // Foundation_UTF8String_INCLUDED
