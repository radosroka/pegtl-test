#include <iostream>
#include <fstream>
#include <string>

#include <tao/pegtl.hpp>

using namespace tao::pegtl;

namespace hello
{
   // Parsing rule that matches a literal "Hello, ".

   struct prefix
         : tao::pegtl::string< 'H', 'e', 'l', 'l', 'o', ',', ' ' > {};

   // Parsing rule that matches a non-empty sequence of
   // alphabetic ascii-characters with greedy-matching.

   struct name
         : tao::pegtl::plus< tao::pegtl::alpha > {};

   // Parsing rule that matches a sequence of the 'prefix'
   // rule, the 'name' rule, a literal "!", and 'eof'
   // (end-of-file/input), and that throws an exception
   // on failure.

   struct grammar
         : tao::pegtl::must< prefix, name, tao::pegtl::one< '!' >, tao::pegtl::eof > {};

   // Class template for user-defined actions that does
   // nothing by default.

   template< typename Rule >
   struct action
         : tao::pegtl::nothing< Rule > {};

   // Specialisation of the user-defined action to do
   // something when the 'name' rule succeeds; is called
   // with the portion of the input that matched the rule.

   template<> 
   struct action< name >
   {
      template < typename Input >
      static void apply(const Input & in, std::string & str )
      {
         str = in.string();
      }
   };

} // hello




int main(int argc, char const *argv[])
{
	std::ifstream file;

	file.open("/etc/nsswitch.conf");

	std::string line;

	while (std::getline(file, line)) {
		if (line.find("Hello") != std::string::npos && line[0] != '#') {
			std::cout << line << std::endl;
         std::string name;
			tao::pegtl::string_input<> in( line, "" );

			tao::pegtl::parse< hello::grammar, hello::action >( in, name );
			std::cout << "Good bye, " << name << "!" << std::endl;
		}
	}


	file.close();

	return 0;
}