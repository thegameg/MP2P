#include <utils.hh>
#include "client.hh"

int main(int argc, const char *argv[])
{
  using namespace client; // Use this namespace inside the function only

  try
  {
    parse_options(argc, argv);
    utils::check_system();
  }
  catch (int i)
  {
    return i;
  }
  catch (std::exception& e)
  {
    std::cerr << "Initialization failed " << e.what() << std::endl;
    return 1;
  }

  try
  {
    Client client;

    if (client::conf.action == action::upload)
    {
      // Prepare file
      files::File file(client::conf.file_path);
      // FIXME redundancy value??? Dafuq??? uint8_t fail?
      client.request_upload(file, client::conf.redundancy - 48);
    }
    else if (client::conf.action == action::download)
    {
      client.request_download(client::conf.file_path);
    }
  }
  catch (std::exception &e)
  {
    utils::Logger::cerr() << "Client failed: " + std::string(e.what());
    return 1;
  }
  return 0;
}
