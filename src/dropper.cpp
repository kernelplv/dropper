#define POCO_WIN32_UTF8

#include "Dropper.h"


using namespace Poco::Net;
using pJson    = Poco::AutoPtr<Poco::Util::JSONConfiguration>;
using oJson    = Poco::JSON::Object;
using pSession = Poco::AutoPtr<Poco::Net::HTTPSClientSession>;

Dropper::Dropper() 
{}

Dropper::Dropper(const std::string& appKey, const std::string& appSecret)
  : _appKey(appKey), _appSecret(appSecret)
{}

Dropper::~Dropper()
{}

std::shared_ptr<Poco::Net::HTTPSClientSession> Dropper::ssl_session_init(Poco::URI uri)
{
  //!!! needs to register a certificate, but for now I’ll leave it "as is"
  Context::Ptr ctx = new Context( 
    Context::CLIENT_USE, "", 
    Context::VerificationMode::VERIFY_NONE, 9, true );

  SSLManager::instance().initializeClient( 
    nullptr, new Poco::Net::AcceptCertificateHandler(true), 
    ctx );

  auto session = std::make_shared<Poco::Net::HTTPSClientSession>(uri.getHost(), uri.getPort());

  return session;
}

// OAuth2 
void Dropper::authorize()
{
  if ( !_tokenSecret.empty() ) return;
  
  Poco::URI uri( "https://www.dropbox.com/oauth2/authorize" );
  
  uri.addQueryParameter("client_id", _appKey);
  uri.addQueryParameter("response_type","code");
  
  /* по-хорошему, конечно, надо автоматизировать, но пока
     не допер как, т.к. нужны доп библиотеки чтобы открыть
     браузер или вручную создавать процесс(зависимо от платформы)..
     или громоздкий web-view 
     всё это не очень оправданно для редко вызываемой авторизации
                                                маленькой утилиты
     короче пока открываем браузер сами..
  */
  std::cout << "Please, open url: " + uri.toString();
  std::cout << "\n and write the received token here: ";
  std::cout.flush();
  
  std::cin >> _token; // badly ^-^... need some setter
}
// OAuth2 www-url-form-encoded
void Dropper::tokenize()
{
  if ( !_tokenSecret.empty() ) return;

  Poco::URI uri( "https://api.dropboxapi.com/oauth2/token" );

  HTMLForm htmlOrder;
    htmlOrder.add("code", _token);
    htmlOrder.add("grant_type", "authorization_code");
    htmlOrder.add("client_id", _appKey);
    htmlOrder.add("client_secret", _appSecret);
  

  auto session = ssl_session_init(uri);
  HTTPRequest request( HTTPRequest::HTTP_POST, 
                               uri.toString(),
                        HTTPMessage::HTTP_1_1 );

  // Sending request
  htmlOrder.prepareSubmit(request);
  std::ostream& out = session->sendRequest(request);
  htmlOrder.write(out);

  // Receive response
  HTTPResponse response;
  std::istream& in = session->receiveResponse(response);

  auto answer = new Poco::Util::JSONConfiguration(in);
  
  if (response.getStatus() == HTTPResponse::HTTP_OK)
  { 
    _tokenSecret = answer->getString("access_token");
  }
  else
    throw Poco::ApplicationException("Dropbox Error", \
      answer->getString("error"));

}
void Dropper::download_file(Poco::Path& file_path, Poco::Path& dst_path)
{
  if ( _tokenSecret.empty() )
    throw Poco::ApplicationException("Application Error", \
      "The application is not authorized");
  
  Poco::URI uri("https://content.dropboxapi.com/2/files/download");

  oJson api_request; 
    api_request.set("path", file_path.toString(Poco::Path::Style::PATH_UNIX));
  
  // Warning! ostringstrem.str() returns void!
  std::ostringstream str_api_request;
  api_request.stringify(str_api_request); // need check this

  // dropbox has two methods for accessing a file
  bool need_repeat = false;
  do
  {
    need_repeat = false; 

    uri.addQueryParameter("authorization","Bearer " + _tokenSecret);
    uri.addQueryParameter("arg", str_api_request.str());

    auto session = ssl_session_init(uri);
    HTTPRequest request( HTTPRequest::HTTP_POST,
                                 uri.toString(),
                         HTTPMessage::HTTP_1_1 );
    
    std::ostream& out = session->sendRequest(request);

    HTTPResponse response;
    std::istream& in = session->receiveResponse(response);

    if (response.getStatus() == HTTPResponse::HTTP_OK)
    {
      Poco::FileOutputStream savetofile(dst_path.toString(), std::ios::out);
      std::cout << "Downloading: " << dst_path.toString() \
                << " ..." << std::endl;
      Poco::StreamCopier::copyStream(in, savetofile); savetofile.flush();
      std::cout << "Download complete!" << std::endl;
      return;
    }
    else
    {
      auto answer = new Poco::Util::JSONConfiguration(in);
      if ( answer->getString("error_summary").find("unsupported_file") != std::string::npos )
      {
        std::cout << "Changing download method..." << std::endl;
        uri = Poco::URI("https://content.dropboxapi.com/2/files/export");
        need_repeat = true;
      }
      else
        throw Poco::ApplicationException("Dropbox Error", answer->getString("error_summary"));
    }
    
  } while(need_repeat);

}
// Upload smaller than 150Mb
void Dropper::upload_file(Poco::Path& file_path, Poco::Path& dst_path)
{
  if ( _tokenSecret.empty() )
    throw Poco::ApplicationException("Application Error", "The application is not authorized");
  
  Poco::URI uri("https://content.dropboxapi.com/2/files/upload");
  oJson api_request; 
    api_request.set("path", dst_path.toString(Poco::Path::PATH_UNIX));
    api_request.set("mode", "add");
    api_request.set("autorename", true);
    api_request.set("mute", false);
    api_request.set("strict_conflict", false);
  
  // Warning! ostringstrem.str() returns void!
  std::ostringstream str_api_request;
  api_request.stringify(str_api_request); // need check this

  //HTMLForm htmlOrder;
  //htmlOrder.add("Authorization","Bearer " + _tokenSecret);
  //htmlOrder.add("Dropbox-API-Arg", str_api_request.str());
  //htmlOrder.add("Content-Type","application/octet-stream");
  // uri.addQueryParameter("authorization","Bearer " + _tokenSecret);
  // uri.addQueryParameter("arg", str_api_request.str());
  // uri.addQueryParameter("Content-Type", "application/octet-stream");

  auto session = ssl_session_init(uri);
  HTTPRequest request( HTTPRequest::HTTP_POST,
                               uri.toString(),
                       HTTPMessage::HTTP_1_1 );

  request.add("Authorization","Bearer " + _tokenSecret);
  request.add("Dropbox-API-Arg", str_api_request.str());
  request.add("Content-Type","application/octet-stream");
  Poco::File attachment(file_path.toString());
  request.setContentType("application/octet-stream");
  request.setContentLength64(attachment.getSize());
  
  std::ostream& out = session->sendRequest(request);
  
  std::cout << "Uploading: " << file_path.toString() << " ..." << std::endl;
  Poco::FileInputStream savetoweb(file_path.toString());
  Poco::StreamCopier::copyStream(savetoweb, out);

  HTTPResponse response;
  std::istream& in = session->receiveResponse(response); 

  //Poco::FileOutputStream test("test.json", std::ios::out); char b{}; while(!in.eof()) { in>>b; test<<b;}; test.close();

  auto answer = new Poco::Util::JSONConfiguration(in);    

  if (response.getStatus() == HTTPResponse::HTTP_OK)
  {
    std::cout << "Upload complete!" << std::endl;
  }          
  else
  {
    throw Poco::ApplicationException("Dropbox Error", answer->getString("path"));
  }   
}
// needs to encrypt
void Dropper::save_config()
{
  Poco::File cfg("settings.json");
  Poco::FileOutputStream savecfg("settings.json", std::ios::out);
  oJson jsnSettings;
    jsnSettings.set("appKey", _appKey);
    jsnSettings.set("appSecret", _appSecret);
    jsnSettings.set("token", _token);
    jsnSettings.set("tokenSecret", _tokenSecret);
  
  // write json object to filestream
  jsnSettings.stringify(savecfg);
  savecfg.close();
}
void Dropper::load_config()
{
  Poco::File cfg("settings.json");
  Poco::FileInputStream loadcfg;

  if (cfg.exists())
  {
    loadcfg.open(cfg.path(), std::ios::in);

    // read json from stream and make json configuration
    Poco::Util::JSONConfiguration jsnSettings(loadcfg);
      _appKey      = jsnSettings.getString("appKey");
      _appSecret   = jsnSettings.getString("appSecret");
      _token       = jsnSettings.getString("token");
      _tokenSecret = jsnSettings.getString("tokenSecret"); 
    loadcfg.close();
    std::cout << "Configuration loaded." << std::endl;
  }
  else
  {
    cfg.createFile(); // atomic
    std::cout << "New settings file created." << std::endl;
  }
  

}