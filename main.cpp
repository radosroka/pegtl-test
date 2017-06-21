#include <iostream>
#include <fstream>
#include <string>
#include <locale>

#include <tao/pegtl.hpp>

using namespace tao::pegtl;

const std::string property = "usbguard";

namespace usbguard
{
   namespace nsswitch
   {

      struct blanked
            : tao::pegtl::star< tao::pegtl::blank > {};

      struct prop_name
            : tao::pegtl::istring< 'u', 's', 'b', 'g', 'u', 'a', 'r', 'd' > {};

      struct value
            : tao::pegtl::plus< tao::pegtl::alpha > {};


      struct grammar
            : tao::pegtl::must< blanked, prop_name, blanked, tao::pegtl::one< ':' >, blanked, value, blanked, tao::pegtl::eof > {};

      template< typename Rule >
      struct action
            : tao::pegtl::nothing< Rule > {};

      template<> 
      struct action< value >
      {
         template < typename Input >
         static void apply(const Input & in, std::string & str )
         {
            std::locale loc;
            str = in.string();
            for (int i = 0; i < str.length(); ++i)
            {
               str[i] = std::tolower(str[i], loc);
            }
         }
      };
   }

}




int main(int argc, char const *argv[])
{
	std::ifstream file;

	file.open("/etc/nsswitch.conf");

	std::string line;

	while (std::getline(file, line)) {
		if (line[0] != '#') {
			std::cout << line << std::endl;
         std::string name;
			tao::pegtl::string_input<> in( line, "" );
         try {
			   tao::pegtl::parse< usbguard::nsswitch::grammar, usbguard::nsswitch::action >( in, name );
         } catch (tao::pegtl::parse_error &e) {
            continue;
         }
			std::cout << "Good bye, " << name << "!" << std::endl;
		}
	}


	file.close();

	return 0;
}