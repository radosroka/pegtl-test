#include <iostream>
#include <fstream>
#include <string>

#include <pegtl.hh>

using namespace pegtl;

namespace hello
{
   // Parsing rule that matches a literal "Hello, ".

   struct prefix
         : pegtl::string< 'H', 'e', 'l', 'l', 'o', ',', ' ' > {};

   // Parsing rule that matches a non-empty sequence of
   // alphabetic ascii-characters with greedy-matching.

   struct name
         : pegtl::plus< pegtl::alpha > {};

   // Parsing rule that matches a sequence of the 'prefix'
   // rule, the 'name' rule, a literal "!", and 'eof'
   // (end-of-file/input), and that throws an exception
   // on failure.

   struct grammar
         : pegtl::must< prefix, name, pegtl::one< '!' >, pegtl::eof > {};

   // Class template for user-defined actions that does
   // nothing by default.

   template< typename Rule >
   struct action
         : pegtl::nothing< Rule > {};

   // Specialisation of the user-defined action to do
   // something when the 'name' rule succeeds; is called
   // with the portion of the input that matched the rule.

   template<> struct action< name >
   {
      static void apply( const pegtl::input & in, std::string & name )
      {
         name = in.string();
      }
   };

} // hello




int main(int argc, char const *argv[])
{


	std::ifstream file;

	file.open("/etc/nsswitch.conf");

	std::string line;

	while (std::getline(file, line)) {
		if (line.find("usbguard") != std::string::npos && line[0] != '#') {
			std::cout << line << std::endl;
			std::string name;

			pegtl::parse< hello::grammar, hello::action >( name );
			std::cout << "Good bye, " << name << "!" << std::endl;
		}
	}


	file.close();

	return 0;
}