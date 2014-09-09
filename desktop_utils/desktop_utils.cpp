// Peter Vrabel (c) 2014
//
// Following http://utf8everywhere.org/ convention.

#include "stdafx.h"

#include "../version.h"

int version[] = {PRODUCTVER};

using namespace std;
namespace po = boost::program_options;
namespace bo = boost;

string headlessCmd,
       desktopName = "HeadlessDesktop",
       switchToDesktop,

       header = (bo::format(
         "Desktop utils v%1%.%2%, Peter Vrabel (c) 2014")
         % version[0] % version[1]).str();

enum class ParseStatus {
  OK = 1, FAILED, HELP_SHOWN, EXIT_0
};

ParseStatus parseCommandLine(int argc, _TCHAR *argv[]) {
  try {
    po::options_description cmdOptions(header);

    cmdOptions.add_options()
      ("help,h", "Feeling desperate?")
      ("run,r", po::value(&headlessCmd), "Command to run headlessly.")
      ("desktop,n", po::value(&desktopName), "Set the headless desktop name. Used with '--run'. Optional, default = HeadlessDesktop")
      ("list,l", "List available desktops of current Window station.")
      ("switch-to,s", po::value(&switchToDesktop), "Switch to a desktop. Takes a desktop name from the list of desktops.")
      ("switch-to-default,t", "Switch to the default desktop. Can be used if you are being stranded.");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, cmdOptions), vm);
    po::notify(vm);

    if (vm.count("help")) {
      cout << cmdOptions << endl;

      return ParseStatus::HELP_SHOWN;
    }

    if (vm.count("switch-to-default")) {
      Desktop::switchToDefault();

      return ParseStatus::EXIT_0;
    }

    if (vm.count("switch-to")) {
      Desktop::switchTo(switchToDesktop);

      return ParseStatus::EXIT_0;
    }

    if (vm.count("list")) {
      cout << header << endl << endl
           << "* Available desktops:" << endl;

      for (string desktop : Desktop::desktops())
        cout << desktop << endl;

      return ParseStatus::EXIT_0;
    }

    if (!vm.count("run"))
      throw runtime_error("--run option must be specified!");
  }
  catch (po::unknown_option &e) {
    cerr << e.what() << endl;

    return ParseStatus::FAILED;
  }

  return ParseStatus::OK;
}


int _tmain(int argc, _TCHAR* argv[]) {
  // Let's go.

  try {
    switch (parseCommandLine(argc, argv)) {
    case ParseStatus::FAILED:
      return 1;
      break;
    case ParseStatus::EXIT_0:
    case ParseStatus::HELP_SHOWN:
      return 0;
      break;
    };

    HDESK desktopHandle = Desktop::create(desktopName);
    Desktop::createProcess(
      desktopName,
      "C:\\Windows\\explorer.exe");

    Sleep(2000);

    Desktop::createProcess(
      desktopName,
      headlessCmd);
  }
  catch (runtime_error &e) {
    cerr << e.what() << endl;
    return 1;
  }
  catch (...) {
    cerr << "A bummer, unknown exception caught!" << endl;

    return 1;
  }

  return 0;
}

