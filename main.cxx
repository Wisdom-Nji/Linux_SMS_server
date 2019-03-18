/**
\mainpage Afkanerd OpenSMS
\author Sherlock Wisdom
\todo document namespace file

*/

#include "libs/mmcl1_5m5/src/SMS.hpp"
#include "libs/mmcl1_m0d3m/src/Modem.hpp"


using namespace std;
using namespace sms;

map<string, string>
SMS::configurations = SMS::config_file("/usr/local/etc/afkanerd_openos/confs/sms.conf");

map<string, string>
Modem::configurations = Modem::config_file("/usr/local/etc/afkanerd_openos/confs/modem.conf");

int main(int argc, char** argv) {
  string type, number, message, _class = "1", group;
  bool debug;

  if(argc > 1) {
    for(int i=1;i<argc;++i) {
      if((string)(argv[i]) == "--send") {
        type = "send";
      }
      else if((string)(argv[i]) == "--number") {
        number = argv[i+1];
        ++i;
      }
      else if((string)(argv[i]) == "--message") {
        message = argv[i+1];
        ++i;
        // sms::output::debug("Message: " + message);
      }
      else if((string)(argv[i]) == "--class") {
        _class = argv[i+1];
        ++i;
      }
      else if((string)(argv[i]) == "--group") {
        group = argv[i+1];
        ++i;
      }
      else if((string)(argv[i]) == "--debug") {
        debug = true;
        ++i;
      }
    }
  }
  else {
    sms::output::warning("Not enough arguments...");
    return 1;
  }

  if(type == "send") {
    Modem modemObject;
    modemObject.debug = true;
    auto modems = modemObject.list();
    if(!message.empty()) cout << "Message: " << message << endl;
    if(!group.empty()) cout << "Group: " << group << endl;
    // sms::output::debug("Found - " + toString((modems.size())) + " modem(s)");
    // cout << "Found " << modems.size() << " modem(s)" << endl;
    // output::debug("GROUP: " + group);
    for(auto modem : modems) {
      if(!group.empty()) {
        //Put global output function
      	// if(debug) {
        //   cout << "Modem's IMEI| " << modem.get_imei() << endl;
      	//   cout << "Modem's Group| " << modem.get_group() << endl;
        // }
        if(modem.get_group() == group) {
          cout << "FOUND MODEM FOR GROUP!";
          SMS sms(modem.get_index(), number, message);
          sms.set_class(_class);
          if(sms.prepared()) {
            cout << number << " | " << message << " | ";
            sms.send();
            sms.save();
            sms.remove();
            sms::output::debug("SMS sent!");
          } else {
            sms::output::warning("SMS not prepared");
          }
          break;
        }
      }
      else {
        sms::output::warning("Group not found!");
        SMS sms(modem.get_index(), number, message);
        sms.set_class(_class);
        if(sms.prepared()) {
          sms.send();
          // sms.save();
          sms::output::debug("SMS sent!");
        } else {
          sms::output::warning("SMS not prepared");
        }
        break;
      }
    }
  }

  else if(type == "receive") {

  }

  else {
    sms::output::warning("No valid command found");
  }

  return 0;
}
