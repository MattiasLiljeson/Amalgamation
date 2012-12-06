#include <igloo/igloo_alt.h>
#include <ResourceManager.h>
#include <DebugUtil.h>
#include <ToString.h>
#include <string>

using namespace igloo;
using namespace std;

Describe(A_Manager)
{
	It(Should_store_generic_data)
	{
		// test string
		ResourceManager<string> strmanager;
		// some keys
		string janitor = "Janitor";
		string gardener = "Gardener";
		// some name objects to store
		string* urban = new string("Urban");
		string* korvo = new string("Korvo");

		// add some data
		strmanager.addResource(janitor,urban);
		strmanager.addResource(gardener,korvo);
		// check data
		AssertThat(strmanager.getResource(janitor),Equals(urban));
		AssertThat(strmanager.getResource(gardener),Equals(korvo));
	}

	It(Should_be_able_to_access_data_from_given_index)
	{
		// test string
		ResourceManager<string> strmanager;
		// some keys
		string janitor = "Janitor";
		string gardener = "Gardener";
		string clown = "Clown";
		// some name objects to store
		string* urban = new string("Urban");
		string* korvo = new string("Korvo");
		string* boris = new string("Boris");
		// add some data
		unsigned int id1 = strmanager.addResource(janitor,urban);
		unsigned int id2 = strmanager.addResource(gardener,korvo);
		unsigned int id3 = strmanager.addResource(clown,boris);
		// check data
		AssertThat(strmanager.getResource(id1),Equals(urban));
		AssertThat(strmanager.getResource(id2),Equals(korvo));
		AssertThat(strmanager.getResource(id3),Equals(boris));
	}

	It(Should_be_able_to_access_data_from_string_key)
	{
		// test string
		ResourceManager<string> strmanager;
		// some keys
		string janitor = "Janitor";
		string gardener = "Gardener";
		string clown = "Clown";
		// some name objects to store
		string* urban = new string("Urban");
		string* korvo = new string("Korvo");
		string* boris = new string("Boris");

		// add some data
		strmanager.addResource(janitor,urban);
		strmanager.addResource(gardener,korvo);
		strmanager.addResource(clown,boris);
		// check data
		AssertThat(strmanager.getResource(janitor),Equals(urban));
		AssertThat(strmanager.getResource(gardener),Equals(korvo));
		AssertThat(strmanager.getResource(clown),Equals(boris));
		// test integer
		ResourceManager<int> imanager;
		// some keys
		string large = "large";
		string twelve = "twelve";
		string negative = "negative";
		// some name objects to store
		int* first = new int(31245645);
		int* second = new int(12);
		int* third = new int(-1234);

		// add some data
		imanager.addResource(large,first);
		imanager.addResource(twelve,second);
		imanager.addResource(negative,third);
		// check data
		AssertThat(imanager.getResource(large),Equals(first) );
		AssertThat(imanager.getResource(twelve),Equals(second) );
		AssertThat(imanager.getResource(negative),Equals(third) );
	}

	It(Should_be_able_to_keep_indices_constant_after_removal_of_element)
	{
		// test string
		ResourceManager<string> strmanager;
		// some keys
		string janitor = "Janitor";
		string gardener = "Gardener";
		string clown = "Clown";
		string soldier = "Soldier";
		// some name objects to store
		string* urban = new string("Urban");
		string* korvo = new string("Korvo");
		string* boris = new string("Boris");
		string* flingo = new string("Flingo");

		// add some data
		unsigned int id1 = strmanager.addResource(janitor,urban);
		unsigned int id2 = strmanager.addResource(gardener,korvo);
		unsigned int id3 = strmanager.addResource(clown,boris);
		unsigned int id4 = strmanager.addResource(soldier,flingo);
		// remove data in the middle using string
		AssertThat(strmanager.removeResource(gardener),IsTrue());
		// check data
		AssertThat(strmanager.getResource(gardener)==NULL,IsTrue());
		AssertThat(strmanager.getResource(janitor),Equals(urban));
		AssertThat(strmanager.getResource(clown),Equals(boris));
		AssertThat(strmanager.getResource(soldier),Equals(flingo));
		// make sure that indices are the same
		AssertThat(strmanager.getResource(id2)==NULL,IsTrue());
		AssertThat(strmanager.getResource(id1),Equals(urban));
		AssertThat(strmanager.getResource(id3),Equals(boris));
		AssertThat(strmanager.getResource(id4),Equals(flingo));
		// remove data in the middle using index
		AssertThat(strmanager.removeResource(clown),IsTrue());
		// check data
		AssertThat(strmanager.getResource(gardener)==NULL,IsTrue());
		AssertThat(strmanager.getResource(janitor),Equals(urban));
		AssertThat(strmanager.getResource(clown)==NULL,IsTrue());
		AssertThat(strmanager.getResource(soldier),Equals(flingo));
		// make sure that indices are the same
		AssertThat(strmanager.getResource(id2)==NULL,IsTrue());
		AssertThat(strmanager.getResource(id1),Equals(urban));
		AssertThat(strmanager.getResource(id3)==NULL,IsTrue());
		AssertThat(strmanager.getResource(id4),Equals(flingo));
	}

	It(Should_return_already_existing_data_on_duouble_insertion)
	{
		// test string
		ResourceManager<string> strmanager;
		// some keys
		string janitor = "Janitor";
		string clown = "Clown";
		string soldier = "Soldier";
		// some name objects to store
		string* urban = new string("Urban");
		string* korvo = new string("Korvo");
		string* boris = new string("Boris");

		// add some data
		unsigned int id1 = strmanager.addResource(janitor,urban);
		unsigned int id2 = strmanager.addResource(soldier,korvo);
		unsigned int id3 = strmanager.addResource(clown,boris);
		unsigned int id4 = strmanager.addResource(clown,boris);
		unsigned int id5 = strmanager.addResource(clown,boris);
		// check data
		AssertThat(strmanager.getResource(id3),Equals(boris));
		AssertThat(strmanager.getResource(clown),Equals(boris));
		AssertThat(id4==id3,IsTrue());
		AssertThat(id5==id3,IsTrue());
		AssertThat(strmanager.getResource(id4),Equals(boris));
		AssertThat(strmanager.getResource(id5),Equals(boris));
	}


};