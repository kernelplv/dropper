#ifndef Dropper_INCLUDED
#define Dropper_INCLUDED

#define POCO_WIN32_UTF8

// lib mingw64/mingw-w64-x86_64-poco 1.9.2-1
#include "Poco/Poco.h"
#include "Poco/URI.h"
#include "Poco/Format.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/FileStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/AutoPtr.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/AcceptCertificateHandler.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/MediaType.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/Util/JSONConfiguration.h"
// lib #

#include <memory>

class Dropper
{
  Dropper& operator= (const Dropper&) = delete;
  Dropper(const Dropper&) = delete;

  std::string _appKey{};
  std::string _appSecret{};
  std::string _token{};
  std::string _tokenSecret{};

 std::shared_ptr<Poco::Net::HTTPSClientSession> ssl_session_init(Poco::URI uri);

public:
  Dropper();
  Dropper(const std::string& appKey,const std::string& appSecret);
  ~Dropper();

  // Authorization
  void authorize();
  void tokenize();
  // Settings
  void save_config();  // todo: needs to encrypt
  void load_config();  // todo: needs to decrypt
  // Functional
  void download_file(Poco::Path& file_path, Poco::Path& dst_path);
  void upload_file(Poco::Path& file_path, Poco::Path& dst_path);
  
};


#endif // Dropper_INCLUDED