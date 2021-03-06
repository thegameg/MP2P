#include "boost/program_options.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#define DEFAULT_CONFIG_PATH "../config/master.conf"
#define DEFAULT_HOSTNAME "localhost"
#define DEFAULT_PORT 3727
#define DEFAULT_CONCURRENCY 4
#define DEFAULT_TIMEOUT 300


using c_ty = decltype(master::conf.config_path);
using lh_ty = decltype(master::conf.hostname);
using lp_ty = decltype(master::conf.port);
using l_ty = std::pair<lh_ty, lp_ty>;
using cy_ty = decltype(master::conf.concurrency);
using to_ty = decltype(master::conf.timeout);
using db_dh_ty = decltype(master::conf.db.hostname);
using db_pw_ty = decltype(master::conf.db.password);
using db_bu_ty = decltype(master::conf.db.bucket);

namespace std
{

  // Specifying >> operator for std::pair<lh_ty, lp_ty> to parse std::pair
  // CLI arg type.
  istream& operator>>(istream& is, l_ty& master)
  {
    string tmp;
    is >> tmp;
    const size_t sep = tmp.find_last_of(':');
    if (sep != string::npos) // If found
    {
      master.first = boost::lexical_cast<lh_ty, std::string>(tmp.substr(0, sep));
      master.second = boost::lexical_cast<lp_ty, std::string>(tmp.substr(sep + 1));
    }
    else
    {
      master.first = boost::lexical_cast<lh_ty, std::string>(tmp);
      master.second = 0;
    }
    return is;
  }
}

namespace master
{
  namespace po = boost::program_options;

  struct Conf conf;

  void
  parse_options_file(const std::string& config_path)
  {
    std::ifstream fs(config_path);
    if (!fs)
      throw std::runtime_error("File not found: " + config_path + ".");

    boost::property_tree::ptree pt;
    boost::property_tree::read_json(fs, pt);

    master::conf.concurrency = pt.get<cy_ty>("concurrency");
    master::conf.hostname = pt.get<lh_ty>("hostname");
    master::conf.port = pt.get<lp_ty>("port");
    master::conf.timeout = pt.get<to_ty>("timeout");
    master::conf.db.hostname = pt.get<db_dh_ty>("database.hostname");
    master::conf.db.password = pt.get<db_pw_ty>("database.password");
    master::conf.db.bucket = pt.get<db_bu_ty>("database.bucket");
  }

  void
  parse_options(int argc, const char *argv[])
  {
    try
    {
      // Declare the supported options.
      po::options_description desc("Allowed options");
      desc.add_options()
          ("help,h", "display this help and exit")
          ("version,v", "displays version of the master")
          ("config,c",  po::value<c_ty>(), "path to the configuration file")
          ("listen,l",  po::value<l_ty>(), "the host:ip to listen")
          ("concurrency,y", po::value<cy_ty>(), "the number of thread that listen")
          ("timeout,t", po::value<to_ty>(), "timeout in seconds")
          ;

      // Parse options.
      po::variables_map vm;
      po::store(po::parse_command_line(argc, argv, desc), vm);
      po::notify(vm);

      if (vm.count("help"))
      {
        std::cout << desc;
        throw 0;
      }

      if (vm.count("version"))
      {
        std::cout << "Master " << infos::version << std::endl << std::endl
        << infos::copyright << std::endl;
        throw 0;
      }

      // First define default values...
      master::conf.config_path = DEFAULT_CONFIG_PATH;
      master::conf.hostname = DEFAULT_HOSTNAME;
      master::conf.port = DEFAULT_PORT;
      master::conf.concurrency = std::thread::hardware_concurrency() * 2;
      master::conf.timeout = DEFAULT_TIMEOUT;

      // Second parse options specified in config file...
      if (vm.count("config"))
        master::conf.config_path = vm["config"].as<c_ty>();
      parse_options_file(master::conf.config_path);

      // ... then override them by specified CLI arguments if any...
      if (vm.count("listen"))
      {
        auto listen = vm["listen"].as<l_ty>();
        master::conf.hostname = listen.first;
        if (listen.second)
          master::conf.port = listen.second;
      }

      if (vm.count("concurrency"))
        master::conf.concurrency = vm["concurrency"].as<cy_ty>();

      if (vm.count("timeout"))
        master::conf.timeout = vm["timeout"].as<to_ty>();

      // ... and finally check results
      if (master::conf.hostname == "")
        master::conf.hostname = DEFAULT_HOSTNAME;
      if (master::conf.port == 0)
        master::conf.port = DEFAULT_PORT;
      if (master::conf.concurrency == 0)
        master::conf.concurrency = DEFAULT_CONCURRENCY;
      if (master::conf.timeout == 0)
        master::conf.timeout = DEFAULT_TIMEOUT;

      // Just try to resolve hostnames (throw if it does not work).
      network::get_ipv6(master::conf.hostname);
      network::get_ipv6(master::conf.db.hostname);
    }
    catch (int i)
    {
      throw i;
    }
    catch (std::exception &e)
    {
      std::cerr << e.what() << " Try --help." << std::endl;
      throw 1;
    }
    catch (...)
    {
      std::cerr << "Invalid option(s). Try --help." << std::endl;
      throw 1;
    }
  }
}