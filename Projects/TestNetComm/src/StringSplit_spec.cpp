#include <igloo/igloo_alt.h>

#include <queue>
#include <string>

#include "StringSplit.h"

using namespace igloo;
using namespace std;


Describe(a_string_splitter)
{
	It(recognizes_two_elements_if_there_us_one_delimiter_separating_them)
	{
		string text = "one,two";
		queue< string > splits;
		stringSplit( text, ",", splits );
		Assert::That(splits.size(), Equals(2));
	}

};